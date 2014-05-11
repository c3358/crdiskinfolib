/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : Simplified BSD license
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "resource.h"
#include "DialogCx.h"
#include "GetOsInfo.h"
#include <Shlwapi.h>
#include "GetFileVersion.h"

#include <strsafe.h>

// defined by Windows 8.1/Windows 2012 R2
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif

typedef HRESULT(WINAPI *FuncGetDpiForMonitor) (HMONITOR hmonitor, UINT dpiType, UINT *dpiX, UINT *dpiY);

CDialogCx::CDialogCx(UINT dlgResouce, CWnd* pParent)
				:CDialogEx(dlgResouce, pParent)
{
	DebugPrint(L"CDialogCx::CDialogCx");

	m_FlagShowWindow = FALSE;
	m_FlagModelessDlg = FALSE;
	m_ParentWnd = NULL;
	m_DlgWnd = NULL;
	m_MenuId = 0;

	m_BackgroundName = L"background";

	m_ZoomRatio = 1.0;
	m_ZoomType = ZOOM_TYPE_AUTO;
}

CDialogCx::~CDialogCx()
{
}

void CDialogCx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CDialogCx::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_IsHighContrast = IsHighContrast();
	m_IsDrawFrame = FALSE;
	CDC *pDC = GetDC();
	m_Dpi = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
	ReleaseDC(pDC);

	FuncGetDpiForMonitor pGetDpiForMonitor = (FuncGetDpiForMonitor) GetProcAddress(GetModuleHandle(_T("Shcore.dll")), "GetDpiForMonitor");
	if (pGetDpiForMonitor != NULL)
	{
		UINT dpiX, dpiY;
		pGetDpiForMonitor(MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), 0, &dpiX, &dpiY);
		m_Dpi = dpiY;
	}

	m_hAccelerator = ::LoadAccelerators(AfxGetInstanceHandle(),
		                                MAKEINTRESOURCE(IDR_ACCELERATOR));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDialogCx, CDialog)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DPICHANGED, &CDialogCx::OnDpiChanged)
	ON_MESSAGE(WM_SYSCOLORCHANGE, &CDialogCx::OnSysColorChange)
	ON_MESSAGE(WM_DISPLAYCHANGE, &CDialogCx::OnDisplayChange)
END_MESSAGE_MAP()

BOOL CDialogCx::PreTranslateMessage(MSG* pMsg) 
{
	if(m_hAccelerator != NULL)
	{
		if(::TranslateAccelerator(m_hWnd, m_hAccelerator, pMsg) != 0)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

double CDialogCx::GetZoomRatio()
{
	return m_ZoomRatio;
}

DWORD CDialogCx::ChangeZoomType(DWORD zoomType)
{
	DWORD current = (DWORD)(m_Dpi / 96.0 * 100);

	if(zoomType == ZOOM_TYPE_AUTO)
	{
		if(current >= 300)
		{
			zoomType = ZOOM_TYPE_300;
		}
		else if(current >= 200)
		{
			zoomType = ZOOM_TYPE_200;
		}
		else if(current >= 150)
		{
			zoomType = ZOOM_TYPE_150;
		}
		else if(current >= 125)
		{
			zoomType = ZOOM_TYPE_125;
		}
		else
		{
			zoomType = ZOOM_TYPE_100;
		}
	}

	m_ZoomRatio = zoomType / 100.0;

	return zoomType;
}

void CDialogCx::SetClientRect(DWORD sizeX, DWORD sizeY, DWORD menuLine)
{
	CRect rc;
	CRect clientRc;
	CRect currentRc;
	rc.left = 0;
	rc.top = 0;
	rc.right = sizeX;
	rc.bottom = sizeY;
	int X = 0, Y = 0;

	GetWindowRect(&currentRc);
	GetClientRect(&clientRc);
	X = currentRc.left - (clientRc.Width() - sizeX) / 2;
	Y = currentRc.top;

	if(clientRc.Height() == sizeY && clientRc.Width() == sizeX)
	{
		return;
	}

	WINDOWINFO wi = {0};
	wi.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(&wi);

// 0x94CE004C

	AdjustWindowRect(&rc, wi.dwStyle, TRUE);
	SetWindowPos(&CWnd::wndTop, X, Y,
		rc.Width(), rc.Height() + GetSystemMetrics(SM_CYMENU) * menuLine, SWP_NOMOVE);

	GetClientRect(&clientRc);
	if(clientRc.Height() != sizeY)
	{
		SetWindowPos(&CWnd::wndTop, X, Y, rc.Width(),
			rc.Height() + GetSystemMetrics(SM_CYMENU) * menuLine + sizeY - clientRc.Height(), SWP_NOMOVE);
	}
}

BOOL CDialogCx::Create(UINT nIDTemplate, CWnd* pDlgWnd, UINT menuId, CWnd* pParentWnd)
{
	m_FlagModelessDlg = TRUE;
	m_ParentWnd = pParentWnd;
	m_DlgWnd = pDlgWnd;
	m_MenuId = menuId;

	if(m_MenuId != 0 && m_ParentWnd != NULL)
	{
		CMenu *menu = m_ParentWnd->GetMenu();
		menu->EnableMenuItem(m_MenuId, MF_GRAYED);
		m_ParentWnd->SetMenu(menu);
		m_ParentWnd->DrawMenuBar();
	}

	return CDialog::Create(nIDTemplate, pParentWnd);
}

void CDialogCx::OnCancel() 
{
	if(m_FlagModelessDlg)
	{
		if(m_MenuId != 0 && m_ParentWnd != NULL)
		{
			CMenu *menu = m_ParentWnd->GetMenu();
			menu->EnableMenuItem(m_MenuId, MF_ENABLED);
			m_ParentWnd->SetMenu(menu);
			m_ParentWnd->DrawMenuBar();
		}
		CWnd::DestroyWindow();
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CDialogCx::PostNcDestroy()
{
	if(m_FlagModelessDlg)
	{
		m_DlgWnd = NULL;
		delete this;
	}
}

void CDialogCx::OnOK()
{
}

void CDialogCx::ShowWindowEx(int nCmdShow)
{
	m_FlagShowWindow = TRUE;
	ShowWindow(nCmdShow);
	SetForegroundWindow();
}

CString CDialogCx::i18n(CString section, CString key, BOOL inEnglish)
{
	TCHAR str[256];
	CString cstr;

	if(inEnglish)
	{
		GetPrivateProfileString(section, key, _T(""), str, 256, m_DefaultLangPath);
		cstr = str;
	}
	else
	{
		GetPrivateProfileString(section, key, _T(""), str, 256, m_CurrentLangPath);
		cstr = str;
		if(cstr.IsEmpty())
		{
			GetPrivateProfileString(section, key, _T(""), str, 256, m_DefaultLangPath);
			cstr = str;
		}
	}

	return cstr;
}

// 2012/5/6
// http://msdn.microsoft.com/ja-jp/magazine/cc163834(en-us).aspx
void CDialogCx::OpenUrl(CString url)
{
	int result = 0;
	result = (int)ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
	if(result <= 32)
	{
		result = (int)ShellExecuteW(NULL, _T("open"), _T("iexplore.exe"), url, NULL, SW_SHOWNORMAL);
		if(result <= 32)
		{
			CString args;
			args.Format(_T("url.dll,FileProtocolHandler %s"), url);
			ShellExecuteW(NULL, _T("open"), _T("rundll32.exe"), args, NULL, SW_SHOWNORMAL);
		}
	}
}

void CDialogCx::SetupControl(int nIDDlgItem , int x, int y, int width, int height)
{
	GetDlgItem(nIDDlgItem)->MoveWindow(
		(int)(x * m_ZoomRatio),
		(int)(y * m_ZoomRatio),
		(int)(width * m_ZoomRatio),
		(int)(height * m_ZoomRatio)
		);
}

BOOL CDialogCx::IsHighContrast()
{
	HIGHCONTRAST hc;
	hc.cbSize = sizeof(HIGHCONTRAST);
	SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(HIGHCONTRAST), &hc, 0);
	return hc.dwFlags & HCF_HIGHCONTRASTON;
}

BOOL CDialogCx::IsDrawFrame()
{
	BOOL isDrawFrame = FALSE;
	CDC *pDC = GetDC();
	if (m_IsHighContrast || GetDeviceCaps(pDC->m_hDC, BITSPIXEL) * GetDeviceCaps(pDC->m_hDC, PLANES) < 24)
	{
		isDrawFrame = TRUE;
	}
	ReleaseDC(pDC);
	
	return isDrawFrame;
}

afx_msg LRESULT CDialogCx::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	static int preTime = 0;
	int currentTime = GetTickCount();
	if (preTime + 1000 > currentTime)
	{
		return 0;
	}
	else
	{
		preTime = currentTime;
	}


	m_Dpi = (INT)HIWORD(wParam);
	if(m_ZoomType == ZOOM_TYPE_AUTO)
	{
		DWORD oldZoomRatio = (DWORD)(m_ZoomRatio * 100);
		if (ChangeZoomType(m_ZoomType) != oldZoomRatio)
		{
			UpdateDialogSize();
		}
	}

	return 0;
}

afx_msg LRESULT CDialogCx::OnSysColorChange(WPARAM wParam, LPARAM lParam)
{
	m_IsHighContrast = IsHighContrast();
	UpdateDialogSize();
	return 0;
}

afx_msg LRESULT CDialogCx::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	UpdateDialogSize();
	return 0;
}

void CDialogCx::UpdateDialogSize()
{

}

void CDialogCx::UpdateBackground()
{
	HRESULT hr;

	m_ImageBg.Destroy();
	hr = m_ImageBg.Load(IP(m_BackgroundName));

	if(SUCCEEDED(hr))
	{
		m_BitmapBg.Detach();
		hr = m_BitmapBg.Attach((HBITMAP)m_ImageBg);
		if(SUCCEEDED(hr))
		{
			m_BrushDlg.DeleteObject();
			m_BrushDlg.CreatePatternBrush(&m_BitmapBg);

			return ;
		}
	}
	
	m_BrushDlg.DeleteObject();
	m_BrushDlg.CreateHatchBrush(HS_BDIAGONAL, RGB(0xF8, 0xF8, 0xF8));
}

CString CDialogCx::IP(CString imageName)
{
	CString imagePath;
	imagePath.Format(L"%s%s\\%s-%3d.png", m_CxThemeDir, m_CxCurrentTheme, imageName, (DWORD)(m_ZoomRatio * 100));
	if(IsFileExist(imagePath))
	{
		return imagePath;
	}
	imagePath.Format(L"%s%s\\%s-%3d.png", m_CxThemeDir, m_CxDefaultTheme, imageName, (DWORD)(m_ZoomRatio * 100));
	if(IsFileExist(imagePath))
	{
		return imagePath;
	}
	return L"";	
}

HBRUSH CDialogCx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CBrush nullb;

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		if (!m_IsHighContrast)
		{
			nullb.CreateStockObject(NULL_BRUSH);
			pDC->SetBkMode(TRANSPARENT);
			return nullb;
		}
		break;
	case CTLCOLOR_BTN:	//	�����߃{�^���ɕK�v�ȏ����B
		if (!m_IsHighContrast)
		{
			nullb.CreateStockObject(NULL_BRUSH);
			pDC->SetBkMode(TRANSPARENT);
			return nullb;
		}
		break;
	case CTLCOLOR_DLG:
		if (!m_IsHighContrast)
		{	
			pDC->SetBkColor(RGB(255, 255, 255));
			return (HBRUSH) m_BrushDlg;
		}
		break;
	default:
		break;
	}

	// TODO:  ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}
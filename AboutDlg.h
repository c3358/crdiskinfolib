/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : Simplified BSD license
//
//                           Copyright 2008-2009 hiyohiyo. All rights reserved.
/*---------------------------------------------------------------------------*/

#pragma once

#include "CompileOptions.h"

class CAboutDlg : public CDHtmlDialogEx
{
	DECLARE_DYNCREATE(CAboutDlg)



#ifdef SUISHO_SHIZUKU_SUPPORT
	static const int SIZE_X = 640;
	static const int SIZE_Y = 660;
#else
	static const int SIZE_X = 640;
	static const int SIZE_Y = 160;
#endif

public:
	CAboutDlg(CWnd* pParent = NULL);
	virtual ~CAboutDlg();

	enum { IDD = IDD_ABOUT, IDH = IDR_HTML_DUMMY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);

	CString m_Version;
	CString m_Edition;
	CString m_Release;
	CString m_Copyright;

	HRESULT OnCrystalDewWorld(IHTMLElement *pElement);
#ifdef SUISHO_SHIZUKU_SUPPORT
	HRESULT OnProjectShizuku(IHTMLElement *pElement);
#endif

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

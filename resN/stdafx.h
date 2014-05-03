// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#define _WIN32_DCOM 

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0500		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0500	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0500	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdhtml.h>			// HTML Dialogs
#include "DHtmlDialogEx.h"		// CDHtmlDialogEx by hiyohiyo
#include "DHtmlMainDialog.h"	// CDHtmlMainDialog by hiyohiyo

#include "DebugPrint.h"
#include <afxcontrolbars.h>

#include "CompileOptions.h"

// Version Information
#define PROJECT_NAME			_T("CrystalDiskInfo")
#define PRODUCT_NAME			_T("CrystalDiskInfo")

#ifdef SUISHO_SHIZUKU_SUPPORT
#define PRODUCT_SHORT_NAME		_T("CDI 5 SE")
#else
#define PRODUCT_SHORT_NAME		_T("CDI 5")
#endif

#ifdef SUISHO_SHIZUKU_SUPPORT
#ifdef _M_X64
#define PRODUCT_EDITION			_T("Shizuku Edition x64")
#else
#define PRODUCT_EDITION			_T("Shizuku Edition")
#endif
#else
#ifdef _M_X64
#define PRODUCT_EDITION			_T("x64")
#else
#define PRODUCT_EDITION			_T("")
#endif
#endif

#define PRODUCT_VERSION			_T("5.3.1")
#define PRODUCT_RELEASE			_T("2013/01/21")
#define PRODUCT_COPY_YEAR		_T("2008-2013")
#define PRODUCT_COPYRIGHT		_T("Copyright &copy; 2008-2013 hiyohiyo.")

#define URL_CRYSTAL_DEW_WORLD_JA	_T("http://crystalmark.info/")
#define URL_CRYSTAL_DEW_WORLD_EN 	_T("http://crystalmark.info/?lang=en")

#define	URL_PROJECT_SHIZUKU_JA		_T("http://crystalmark.info/Shizuku/")
#define	URL_PROJECT_SHIZUKU_EN		_T("http://crystalmark.info/Shizuku/index-en.html")
#define	URL_PROJECT_SHIZUKU_KARUTA	_T("https://sites.google.com/site/sgijinka/itkaruta")

#define URL_HTML_HELP_JA		_T("http://crystalmark.info/software/CrystalDiskInfo/manual-ja/")
#define URL_HTML_HELP_EN 		_T("http://crystalmark.info/software/CrystalDiskInfo/manual-en/")

// Command
static const int TRAY_TEMPERATURE_ICON_BASE			= WM_APP + 0x1200;
static const int SELECT_DISK_BASE					= WM_APP + 0x1300;
static const int AUTO_REFRESH_TARGET_BASE			= WM_APP + 0x1400;
static const int SHOW_GRAPH_BASE					= WM_APP + 0x1500;
static const int ALARM_SETTING_HEALTH_STATUS_BASE	= WM_APP + 0x1900;
static const int ALARM_SETTING_TEMPERATURE_BASE		= WM_APP + 0x2000; // Main Only
static const int GRAPH_DISK_INDEX					= WM_APP + 0x2000; // Graph Only

static const int RE_EXEC = 5963;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

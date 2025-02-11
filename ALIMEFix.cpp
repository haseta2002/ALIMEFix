#define _WIN32_IE 0x0300
#include "Alplug.h"
#include "Alapi.h"
#include "Resource.h"

#include <stdio.h>
#include <windowsx.h>
#include <richedit.h>
#include <crtdbg.h>
#include <commctrl.h>

#define PROFILE_SECTION	"ALIMEFix"
COLORREF crIMETargetConvColor, crPropIMETargetConvColor;
COLORREF crIMETargetNotConvColor, crPropIMETargetNotConvColor;

//////////////////////////////////////////////////////////////////////////////
// 必須の関数

LPCTSTR WINAPI APCGetPlugName(void)
{
	// プラグイン名を返してください
	return "ALIMEFix";
}

BOOL CALLBACK AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
void WINAPI APCVersionInfo(HWND hWnd)
{
	// バージョン情報を表示してください
	//MessageBox(hWnd, "ALIMEFix Version 1.0α１",
	//	"プラグイン情報", MB_OK);
	DialogBox(g_hInstance,
			MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hWnd, (DLGPROC)AboutProc);
}

BOOL WINAPI APCStartUp(LPCTSTR version)
{
	// AL-Mail32 が起動する時に、一度だけ呼ばれます
	//InitCommonControls();

	char profile[_MAX_PATH];

	// ALIMEFixの設定を読み込む
	lstrcpy(profile, APAGetMailbox());
	lstrcat(profile, "ALIMEFix.ini");
	crIMETargetConvColor = GetPrivateProfileInt("IME", "TargetConvColor",
										RGB(0, 255, 255), profile);
	crIMETargetNotConvColor = GetPrivateProfileInt("IME", "TargetNotConvColor",
										RGB(0, 0, 255), profile);
	return TRUE;
}

// ※上記２つの関数は必ず実装し、エクスポートしてください。
//	 それ以外の関数は必要なものだけ実装し、不用のものはソースから削除してくだ
//	 さい。

//////////////////////////////////////////////////////////////////////////////
// 設定
BOOL CALLBACK DialogProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

void WINAPI APCSetup(HWND hWnd)
{
	// 必要な場合は、ここでプラグインのオプション設定画面を開いてください
	crPropIMETargetConvColor = crIMETargetConvColor;
	crPropIMETargetNotConvColor = crIMETargetNotConvColor;
	DialogBox(g_hInstance,
			MAKEINTRESOURCE(IDD_SETUPDLG), hWnd, (DLGPROC)DialogProc);
}

void WINAPI APCCleanUp(void)
{
	// AL-Mailが終了する際に一度だけ呼び出されます。
}

HWND GetRichEditClass(HWND hWnd)
{
	HWND hChildWnd = hWnd;
	HWND hViewWnd = NULL;
	char class_name[28+1];
	while (1) {
		hChildWnd = GetWindow(hChildWnd, GW_CHILD);
		if (hChildWnd == NULL) {
			break;
		}
		GetClassName(hChildWnd, class_name, 28);
		if (strcmp(class_name, RICHEDIT_CLASS10A) == 0) {
			hViewWnd = hChildWnd;
			break;
		}
	}
	return hViewWnd;
}

void WINAPI APCSendCreate(HWND hWnd)
{
	HWND hSendWnd = GetRichEditClass(hWnd);
	if (hSendWnd) {
		// attribute for COMPOSITIONSTRING Structure( Imm.h)
		#define ATTR_INPUT                      0x00
		#define ATTR_TARGET_CONVERTED           0x01
		#define ATTR_CONVERTED                  0x02
		#define ATTR_TARGET_NOTCONVERTED        0x03

		COMPCOLOR aCompColor[ 4 ];
		SendMessage(hSendWnd, EM_GETIMECOLOR , 0 , (LPARAM)aCompColor );
		//aCompColor[ ATTR_INPUT ].crBackground = RGB( 255 , 0 , 255 );
		aCompColor[ ATTR_TARGET_CONVERTED ].crBackground = crIMETargetConvColor;
		//aCompColor[ ATTR_CONVERTED ].crBackground = RGB( 255 , 255 , 0 );
		aCompColor[ ATTR_TARGET_NOTCONVERTED ].crBackground = crIMETargetNotConvColor;
		SendMessage(hSendWnd, EM_SETIMECOLOR , 0 , (LPARAM)aCompColor );
	}
}

void WINAPI APCSendClose(HWND hWnd)
{
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// ダイアログ

void DrawButton(LPARAM lParam, BOOL bInactive)
{
	LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
	HBRUSH hBrush;
	switch(pdis->CtlID) {
	case IDC_BUTTON_TARGET_CONVERTED:
		hBrush = CreateSolidBrush(crPropIMETargetConvColor);
		break;
	case IDC_BUTTON_TARGET_NOTCONVERTED:
		hBrush = CreateSolidBrush(crPropIMETargetNotConvColor);
		break;
	default:
		return;
	}
	{
		HBRUSH hBrush;
		if (pdis->itemState & ODS_FOCUS || pdis->itemAction & ODA_FOCUS) {
			//DrawFocusRect(pdis->hDC, &pdis->rcItem);
			hBrush = CreateSolidBrush(RGB(0,0,0));
		} else {
			hBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		}
		FrameRect(pdis->hDC, &pdis->rcItem, hBrush);
		DeleteObject(hBrush);
	}
	pdis->rcItem.left+=1;
	pdis->rcItem.top+=1;
	pdis->rcItem.right-=1;
	pdis->rcItem.bottom-=1;
	DrawFrameControl(pdis->hDC, &pdis->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_ADJUSTRECT |
		(pdis->itemState & ODS_SELECTED ? DFCS_PUSHED : 0) | (bInactive ? DFCS_FLAT : 0));
	pdis->rcItem.left+=2;
	pdis->rcItem.top+=2;
	pdis->rcItem.right-=3;
	pdis->rcItem.bottom-=3;
	if (pdis->itemState & ODS_SELECTED) {
		pdis->rcItem.left++;
		pdis->rcItem.top++;
		pdis->rcItem.right++;
		pdis->rcItem.bottom++;
	}
    //DrawState(pdis->hDC, pdis->rcItem.top, captionRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), TRUE, 0, NULL);
	//DrawState(pdis->hDC, NULL, NULL, (LPARAM)"", 0, pdis->rcItem.left, pdis->rcItem.top, 0, 0, DSS_DISABLED);
	FillRect(pdis->hDC, &pdis->rcItem, hBrush);
	//SetBkColor(pdis->hDC, GetSysColor(COLOR_BTNFACE));

	DeleteObject(hBrush);
}

DWORD CustColor[16];
void SelectColor(HWND hwnd, COLORREF& crButtonColor)
{
	CHOOSECOLOR color;
	ZeroMemory(&color, sizeof(color));
	ZeroMemory(CustColor, sizeof(CustColor));
	CustColor[0] = crPropIMETargetConvColor;
	CustColor[1] = crPropIMETargetNotConvColor;
	color.lStructSize = sizeof(color);
	color.hwndOwner = hwnd;
	color.Flags =  CC_RGBINIT;
	color.rgbResult = crButtonColor;
	color.lpCustColors = CustColor;
	if (ChooseColor(&color) == TRUE) {
		crButtonColor = color.rgbResult;
	}
	InvalidateRect(hwnd, NULL, TRUE);
}

BOOL CALLBACK AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg){
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
				EndDialog(hwnd, IDOK);
				return TRUE;
			}
			break;
	}
	return FALSE;
}

BOOL CALLBACK DialogProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			break;
		case WM_DRAWITEM:
			DrawButton(lParam, FALSE);
			break;
		case WM_COMMAND:
			if( LOWORD(wParam) == IDOK) {
				crIMETargetConvColor = crPropIMETargetConvColor;
				crIMETargetNotConvColor = crPropIMETargetNotConvColor;

				char profile[_MAX_PATH];
				lstrcpy(profile, APAGetMailbox());
				lstrcat(profile, "ALIMEFix.ini");

				char color[20+1];
				sprintf(color, "%d", crIMETargetConvColor);
				WritePrivateProfileString("IME", "TargetConvColor", color, profile);
				sprintf(color, "%d", crIMETargetNotConvColor);
				WritePrivateProfileString("IME", "TargetNotConvColor", color, profile);

				EndDialog(hwnd, IDOK);
				return TRUE;
			} else if(LOWORD(wParam) == IDCANCEL){
				EndDialog(hwnd, IDCANCEL);
				return TRUE;
			} else if (LOWORD(wParam) == IDC_BUTTON_TARGET_CONVERTED) {
				SelectColor(hwnd, crPropIMETargetConvColor);
			} else if (LOWORD(wParam) == IDC_BUTTON_TARGET_NOTCONVERTED) {
				SelectColor(hwnd, crPropIMETargetNotConvColor);
			}
			break;
	}
	return FALSE;
}

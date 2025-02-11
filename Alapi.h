// AL-Mail32 API Version 1.10

#ifndef _ALAPI_H
#define _ALAPI_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

extern UINT (WINAPI *APARegistCallback)(void (WINAPI *)(HWND, DWORD), DWORD);
extern UINT (WINAPI *APASetTimer)(UINT, BOOL (WINAPI *)(UINT, DWORD), DWORD);
extern BOOL (WINAPI *APAKillTimer)(UINT);
extern LPCTSTR (WINAPI *APAGetMailbox)(void);
extern LPCTSTR (WINAPI *APAGetAccount)(HWND hWnd);
extern BOOL (WINAPI *APAImportFile)(LPCTSTR, LPCTSTR, DWORD=0);
extern BOOL (WINAPI *APAMoveMail)(LPCTSTR, LPCTSTR);
extern BOOL (WINAPI *APAMarkMail)(LPCTSTR, UINT, UINT);
extern BOOL (WINAPI *APADeleteMail)(LPCTSTR);
extern LPCTSTR (WINAPI *APAFolderGetCurrentEntry)(HWND);
extern LPCTSTR (WINAPI *APAFolderGetSelectEntry)(HWND, BOOL=FALSE);
extern LPCTSTR (WINAPI *APASendGetText)(HWND, BOOL=FALSE);
extern LPCTSTR (WINAPI *APASendGetHeader)(HWND);
extern BOOL (WINAPI *APASendSetText)(HWND, LPCTSTR);
extern BOOL (WINAPI *APASendSetHeader)(HWND, LPCTSTR);
extern LPCTSTR (WINAPI *APAViewGetText)(HWND);
extern LPCTSTR (WINAPI *APAViewGetHeader)(HWND);
extern BOOL (WINAPI *APAViewSetText)(HWND, LPCTSTR);
extern LPCTSTR (WINAPI *APAViewGetPath)(HWND);
extern int (WINAPI *APASjisToJis)(LPTSTR, LPCTSTR, int);
extern int (WINAPI *APAJisToSjis)(LPTSTR, LPCTSTR, int);

#ifdef __cplusplus
}  // end extern "C"
#endif

extern HINSTANCE g_hInstance;

#endif
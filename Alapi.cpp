#include <windows.h>


extern "C" {
	UINT (WINAPI *APARegistCallback)(void (WINAPI *)(HWND, DWORD), DWORD);
	UINT (WINAPI *APASetTimer)(UINT, BOOL (WINAPI *)(UINT, DWORD), DWORD);
	BOOL (WINAPI *APAKillTimer)(UINT);
	LPCTSTR (WINAPI *APAGetMailbox)(void);
	LPCTSTR (WINAPI *APAGetAccount)(HWND hWnd);
	BOOL (WINAPI *APAImportFile)(LPCTSTR, LPCTSTR, DWORD=0);
	BOOL (WINAPI *APAMoveMail)(LPCTSTR, LPCTSTR);
	BOOL (WINAPI *APAMarkMail)(LPCTSTR, UINT, UINT);
	BOOL (WINAPI *APADeleteMail)(LPCTSTR);
	LPCTSTR (WINAPI *APAFolderGetCurrentEntry)(HWND);
	LPCTSTR (WINAPI *APAFolderGetSelectEntry)(HWND, BOOL);
	LPCTSTR (WINAPI *APASendGetText)(HWND, BOOL=FALSE);
	LPCTSTR (WINAPI *APASendGetHeader)(HWND);
	BOOL (WINAPI *APASendSetText)(HWND, LPCTSTR);
	BOOL (WINAPI *APASendSetHeader)(HWND, LPCTSTR);
	LPCTSTR (WINAPI *APAViewGetText)(HWND);
	LPCTSTR (WINAPI *APAViewGetHeader)(HWND);
	BOOL (WINAPI *APAViewSetText)(HWND, LPCTSTR);
	LPCTSTR (WINAPI *APAViewGetPath)(HWND);
	int (WINAPI *APASjisToJis)(LPTSTR, LPCTSTR, int);
	int (WINAPI *APAJisToSjis)(LPTSTR, LPCTSTR, int);

	LPCTSTR WINAPI APCGetPlugName(void);
};

static HINSTANCE hLib = NULL;
HINSTANCE g_hInstance = NULL;

BOOL StartUpAPI(void)
{
	if(hLib != NULL)
		return TRUE;

	hLib = LoadLibrary("Almail.exe");
	if(hLib == NULL)
		return FALSE;

	(FARPROC&)APARegistCallback = GetProcAddress(hLib, "_APARegistCallback@8");
	if(APARegistCallback == NULL)
		goto Error;

	(FARPROC&)APASetTimer = GetProcAddress(hLib, "_APASetTimer@12");
	if(APASetTimer == NULL)
		goto Error;

	(FARPROC&)APAKillTimer = GetProcAddress(hLib, "_APAKillTimer@4");
	if(APAKillTimer == NULL)
		goto Error;

	(FARPROC&)APAGetMailbox = GetProcAddress(hLib, "_APAGetMailbox@0");
	if(APAGetMailbox == NULL)
		goto Error;

	(FARPROC&)APAGetAccount = GetProcAddress(hLib, "_APAGetAccount@4");
	if(APAGetAccount == NULL)
		goto Error;

	(FARPROC&)APAImportFile = GetProcAddress(hLib, "_APAImportFile@12");
	if(APAImportFile == NULL)
		goto Error;

	(FARPROC&)APAMoveMail = GetProcAddress(hLib, "_APAMoveMail@8");
	if(APAMoveMail == NULL)
		goto Error;

	(FARPROC&)APAMarkMail = GetProcAddress(hLib, "_APAMarkMail@12");
	if(APAMarkMail == NULL)
		goto Error;

	(FARPROC&)APADeleteMail = GetProcAddress(hLib, "_APADeleteMail@4");
	if(APADeleteMail == NULL)
		goto Error;

	(FARPROC&)APAFolderGetCurrentEntry = GetProcAddress(hLib, "_APAFolderGetCurrentEntry@4");
	if(APAFolderGetCurrentEntry == NULL)
		goto Error;

	(FARPROC&)APAFolderGetSelectEntry = GetProcAddress(hLib, "_APAFolderGetSelectEntry@8");
	if(APAFolderGetSelectEntry == NULL)
		goto Error;

	(FARPROC&)APASendGetText = GetProcAddress(hLib, "_APASendGetText@8");
	if(APASendGetText == NULL)
		goto Error;

	(FARPROC&)APASendGetHeader = GetProcAddress(hLib, "_APASendGetHeader@4");
	if(APASendGetHeader == NULL)
		goto Error;

	(FARPROC&)APASendSetText = GetProcAddress(hLib, "_APASendSetText@8");
	if(APASendSetText == NULL)
		goto Error;

	(FARPROC&)APASendSetHeader = GetProcAddress(hLib, "_APASendSetHeader@8");
	if(APASendSetHeader == NULL)
		goto Error;

	(FARPROC&)APAViewGetText = GetProcAddress(hLib, "_APAViewGetText@4");
	if(APAViewGetText == NULL)
		goto Error;

	(FARPROC&)APAViewGetHeader = GetProcAddress(hLib, "_APAViewGetHeader@4");
	if(APAViewGetHeader == NULL)
		goto Error;

	(FARPROC&)APAViewSetText = GetProcAddress(hLib, "_APAViewSetText@8");
	if(APAViewSetText == NULL)
		goto Error;

	(FARPROC&)APAViewGetPath = GetProcAddress(hLib, "_APAViewGetPath@4");
	if(APAViewGetPath == NULL)
		goto Error;

	(FARPROC&)APASjisToJis = GetProcAddress(hLib, "_APASjisToJis@12");
	if(APASjisToJis == NULL)
		goto Error;

	(FARPROC&)APAJisToSjis = GetProcAddress(hLib, "_APAJisToSjis@12");
	if(APAJisToSjis == NULL)
		goto Error;


	return TRUE;

Error:
	FreeLibrary(hLib);
	hLib = NULL;
	return FALSE;
}

void CleanUpAPI(void)
{
	if(hLib != NULL){
		FreeLibrary(hLib);
		hLib = NULL;
	}
}

BOOL WINAPI DllMain(HANDLE hModule, DWORD reason, LPVOID)
{

	if(reason == DLL_PROCESS_ATTACH){
		g_hInstance = (HINSTANCE)hModule;
		if(!StartUpAPI()){
			char buf[256];
			wsprintf(buf,
				"プラグイン「%s」でエラーが発生しました。\r\n"
				"AL-Mail32とプラグインのバージョンを確認してください",
				APCGetPlugName());
			MessageBox(NULL, buf, "プラグインエラー", MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	else if(reason == DLL_PROCESS_DETACH){
		CleanUpAPI();
	}
    return TRUE;
}

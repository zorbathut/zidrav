
#include <windows.h>
#include <commctrl.h>

#include "iface.h"
#include "resource.h"

HINSTANCE g_hInst;

int CALLBACK MainDlg(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	InitCommonControls();		// This makes things work :)
	g_hInst = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(MAIN_DIALOG), 0, MainDlg);	}	// Spawn main dialog and shoot thyself afterwards

int CALLBACK MainDlg(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	switch(Message)	{

	case WM_INITDIALOG:
 		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case MAKE_CHECKSUM:
			CreateDialog( g_hInst, MAKEINTRESOURCE(MCHECK_DIALOG), 0, MakeChecksum);
			return TRUE;

		case MAKE_PATCH:
			CreateDialog( g_hInst, MAKEINTRESOURCE(MPATCH_DIALOG), 0, MakePatch);
			return TRUE;

		case MAKE_REQUEST:
			CreateDialog( g_hInst, MAKEINTRESOURCE(MREQ_DIALOG), 0, MakeRequest);
			return TRUE;

		case FILL_REQUEST:
			CreateDialog( g_hInst, MAKEINTRESOURCE(FREQ_DIALOG), 0, FillRequest);
			return TRUE;

		case APPLY_PATCH:
			CreateDialog( g_hInst, MAKEINTRESOURCE(APATCH_DIALOG), 0, ApplyPatch);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

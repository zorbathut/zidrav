/*
ZIDRAV, file corruption repairer
Copyright (C) 1999  Ben Wilhelm

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include <windows.h>
#include <commctrl.h>

#include "gui.h"
#include "resource.h"

HINSTANCE g_hInst;

int CALLBACK MainDlg(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
int CALLBACK AboutBox(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

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
			CreateDialog( g_hInst, MAKEINTRESOURCE(MCHECK_DIALOG), 0, MakeChecksumGUI);
			return TRUE;

		case MAKE_BATCHCHECKSUM:
			CreateDialog( g_hInst, MAKEINTRESOURCE(MBCHECK_DIALOG), 0, MakeBatchChecksumGUI);
			return TRUE;

		case MAKE_PATCH:
			CreateDialog( g_hInst, MAKEINTRESOURCE(MPATCH_DIALOG), 0, MakePatchGUI);
			return TRUE;

		case MAKE_REQUEST:
			CreateDialog( g_hInst, MAKEINTRESOURCE(MREQ_DIALOG), 0, MakeRequestGUI);
			return TRUE;

		case FILL_REQUEST:
			CreateDialog( g_hInst, MAKEINTRESOURCE(FREQ_DIALOG), 0, FillRequestGUI);
			return TRUE;

		case APPLY_PATCH:
			CreateDialog( g_hInst, MAKEINTRESOURCE(APATCH_DIALOG), 0, ApplyPatchGUI);
			return TRUE;

		case ABOUT:
			CreateDialog( g_hInst, MAKEINTRESOURCE(ABOUT_DIALOG), 0, AboutBox);
			return TRUE;

		case HELP:
			MessageBox( hwnd, "Oops. No help yet. Heh.\nHow embarassing.\n\nYou may wish to check out \"About\", however.", "Oops.", MB_OK | MB_ICONINFORMATION );
			return TRUE;

		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK AboutBox(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	//char buffish[200];

	switch(Message)	{

	case WM_INITDIALOG:
/*		LoadString( g_hInst, ABOUTSTR, buffish, 200 );
		SetDlgItemText( hwnd, ABOUT_TEXT, buffish );*/
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}
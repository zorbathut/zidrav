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
#include <afxcmn.h>
#include <afxwin.h>
#include <commdlg.h>
#include <fstream.h>
#include <shlobj.h>

#include "gui.h"
#include "resource.h"
#include "utils.h"
#include "guiface.h"
#include "mcbatch.h"

int CALLBACK MakeChecksumGUI(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME	ofn;
	unsigned long	ThreadID;
	unsigned long	ECode;

	char	szFileName[MAX_PATH];
	HANDLE	CurThread = NULL;

	switch(Message)	{

	case WM_INITDIALOG:
		ActivateGenericDD( GetDlgItem( hwnd, MCHECK_INPUT_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, MCHECK_OUTPUT_FIELD ) );
		SetDlgItemInt( hwnd, MCHECK_BLOCKSIZE_FIELD, 16384, FALSE );
		SendDlgItemMessage(hwnd, MCHECK_BLOCKSIZE_SLIDER, TBM_SETRANGE, FALSE, MAKELONG(12, 16) );
		SendDlgItemMessage(hwnd, MCHECK_BLOCKSIZE_SLIDER, TBM_SETPOS, TRUE, 14 );
		return TRUE;

	case WM_HSCROLL:
		SetDlgItemInt( hwnd, MCHECK_BLOCKSIZE_FIELD,
			powerof2( SendDlgItemMessage( hwnd, MCHECK_BLOCKSIZE_SLIDER, TBM_GETPOS, 0, 0) ), FALSE );
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case MCHECK_INPUT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				FilenamePlacer( GetDlgItem( hwnd, MCHECK_INPUT_FIELD), GetDlgItem( hwnd, MCHECK_OUTPUT_FIELD ),
					NULL, ".cdt", szFileName );

			return TRUE;

		case MCHECK_OUTPUT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Checksum Files (*.cdt)\0*.cdt\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

			if(GetOpenFileName(&ofn))
				SetDlgItemText( hwnd, MCHECK_OUTPUT_FIELD, szFileName );

			return TRUE;

		case MCHECK_INPUT_FIELD:
			switch( HIWORD(wParam) ) {

			case EN_UPDATE:

				FilenamePlacer( GetDlgItem( hwnd, MCHECK_INPUT_FIELD), GetDlgItem( hwnd, MCHECK_OUTPUT_FIELD ),
					NULL, ".cdt", NULL );
				return TRUE; 
			
			}

			return FALSE;

		case MCHECK_GO:
			if( CurThread == NULL )
				CurThread = CreateThread( NULL, NULL, MakeChecksumGO, hwnd, NULL, &ThreadID );
			  else {
				CloseHandle( CurThread );
				CurThread = CreateThread( NULL, NULL, MakeChecksumGO, hwnd, NULL, &ThreadID );
			}
			return TRUE;

		case IDCANCEL:
			if( CurThread == NULL )
				EndDialog( hwnd, 0 );
			  else {
				GetExitCodeThread( CurThread, &ECode );
				if( ECode != STILL_ACTIVE ) {
					CloseHandle( CurThread );
					EndDialog( hwnd, 0 );
				}
			}
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK MakeBatchChecksumGUI(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	BROWSEINFO		bif;
	LPITEMIDLIST	lpIdl;
	LPMALLOC		pMalloc;
	unsigned long	ThreadID;
	unsigned long	ECode;

	char	szFileName[MAX_PATH];
	HANDLE	CurThread = NULL;

	switch(Message)	{

	case WM_INITDIALOG:
		ActivateGenericDD( GetDlgItem( hwnd, MBCHECK_INPUT_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, MBCHECK_OUTPUT_FIELD ) );
		SetDlgItemInt( hwnd, MBCHECK_BLOCKSIZE_FIELD, 16384, FALSE );
		SendDlgItemMessage(hwnd, MBCHECK_BLOCKSIZE_SLIDER, TBM_SETRANGE, FALSE, MAKELONG(12, 16) );
		SendDlgItemMessage(hwnd, MBCHECK_BLOCKSIZE_SLIDER, TBM_SETPOS, TRUE, 14 );
		return TRUE;

	case WM_HSCROLL:
		SetDlgItemInt( hwnd, MBCHECK_BLOCKSIZE_FIELD,
			powerof2( SendDlgItemMessage( hwnd, MBCHECK_BLOCKSIZE_SLIDER, TBM_GETPOS, 0, 0) ), FALSE );
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case MBCHECK_INPUT_BUTTON:

			ZeroMemory(&bif, sizeof(bif));

			bif.hwndOwner = hwnd;
			bif.pszDisplayName = szFileName;
			bif.ulFlags = NULL;

			if( lpIdl = SHBrowseForFolder(&bif) ) {
				SHGetPathFromIDList( lpIdl, szFileName );
				SetDlgItemText( hwnd, MBCHECK_INPUT_FIELD, szFileName );
				strcat( szFileName, "\\CDTs" );
				SetDlgItemText( hwnd, MBCHECK_OUTPUT_FIELD, szFileName );
				if (SHGetMalloc(&pMalloc) == NOERROR)
					pMalloc->Free(lpIdl); }

			return TRUE;

		case MBCHECK_OUTPUT_BUTTON:

			ZeroMemory(&bif, sizeof(bif));

			bif.hwndOwner = hwnd;
			bif.pszDisplayName = szFileName;
			bif.ulFlags = BIF_EDITBOX;

			if( lpIdl = SHBrowseForFolder(&bif) ) {
				SHGetPathFromIDList( lpIdl, szFileName );
				SetDlgItemText( hwnd, MBCHECK_OUTPUT_FIELD, szFileName );
				if (SHGetMalloc(&pMalloc) == NOERROR)
					pMalloc->Free(lpIdl); }

			return TRUE;

		case MBCHECK_GO:
			if( CurThread == NULL )
				CurThread = CreateThread( NULL, NULL, MakeBatchChecksumGO, hwnd, NULL, &ThreadID );
			  else {
				CloseHandle( CurThread );
				CurThread = CreateThread( NULL, NULL, MakeBatchChecksumGO, hwnd, NULL, &ThreadID );
			}
			return TRUE;

		case IDCANCEL:
			if( CurThread == NULL )
				EndDialog( hwnd, 0 );
			  else {
				GetExitCodeThread( CurThread, &ECode );
				if( ECode != STILL_ACTIVE ) {
					CloseHandle( CurThread );
					EndDialog( hwnd, 0 );
				}
			}
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK MakePatchGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
	
	OPENFILENAME	ofn;
	unsigned long	ThreadID;
	unsigned long	ECode;

	char	szFileName[MAX_PATH];
	HANDLE	CurThread = NULL;

	switch(Message)	{

	case WM_INITDIALOG:
		ActivateGenericDD( GetDlgItem( hwnd, MPATCH_CDT_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, MPATCH_VFILE_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, MPATCH_OUTPUT_FIELD ) );
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case MPATCH_CDT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Checksum Files (*.cdt)\0*.cdt\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				FilenamePlacer( GetDlgItem( hwnd, MPATCH_CDT_FIELD), GetDlgItem( hwnd, MPATCH_OUTPUT_FIELD ),
					".cdt", ".cdp", szFileName );

			return TRUE;

		case MPATCH_VFILE_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				SetDlgItemText( hwnd, MPATCH_VFILE_FIELD, szFileName );

			return TRUE;

		case MPATCH_OUTPUT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Patch Files (*.cdp)\0*.cdp\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdp";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

			if(GetSaveFileName(&ofn))
				SetDlgItemText( hwnd, MPATCH_OUTPUT_FIELD, szFileName );

			return TRUE;

		case MPATCH_CDT_FIELD:
			switch( HIWORD(wParam) ) {

			case EN_UPDATE:

				FilenamePlacer( GetDlgItem( hwnd, MPATCH_CDT_FIELD), GetDlgItem( hwnd, MPATCH_OUTPUT_FIELD ),
					".cdt", ".cdp", NULL );
				return TRUE;

			}
			return FALSE;

		case MPATCH_GO:
			if( CurThread == NULL )
				CurThread = CreateThread( NULL, NULL, MakePatchGO, hwnd, NULL, &ThreadID );
			  else {
				CloseHandle( CurThread );
				CurThread = CreateThread( NULL, NULL, MakePatchGO, hwnd, NULL, &ThreadID );
			}
			return TRUE;

		case IDCANCEL:
			if( CurThread == NULL )
				EndDialog( hwnd, 0 );
			  else {
				GetExitCodeThread( CurThread, &ECode );
				if( ECode != STILL_ACTIVE ) {
					CloseHandle( CurThread );
					EndDialog( hwnd, 0 );
				}
			}
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK MakeRequestGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
	
	OPENFILENAME	ofn;
	unsigned long	ThreadID;
	unsigned long	ECode;

	char	szFileName[MAX_PATH];
	HANDLE	CurThread = NULL;

	switch(Message)	{

	case WM_INITDIALOG:
		ActivateGenericDD( GetDlgItem( hwnd, MREQ_CDT_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, MREQ_DFILE_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, MREQ_OUTPUT_FIELD ) );
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case MREQ_CDT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Checksum Files (*.cdt)\0*.cdt\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				FilenamePlacer( GetDlgItem( hwnd, MREQ_CDT_FIELD), GetDlgItem( hwnd, MREQ_OUTPUT_FIELD ),
					".cdt", ".cdq", szFileName );

			return TRUE;

		case MREQ_DFILE_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				SetDlgItemText( hwnd, MREQ_DFILE_FIELD, szFileName );

			return TRUE;

		case MREQ_OUTPUT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Request Files (*.cdq)\0*.cdq\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdq";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

			if(GetSaveFileName(&ofn))
				SetDlgItemText( hwnd, MREQ_OUTPUT_FIELD, szFileName );

			return TRUE;

		case MREQ_CDT_FIELD:
			switch( HIWORD(wParam) ) {

			case EN_UPDATE:
				
				FilenamePlacer( GetDlgItem( hwnd, MREQ_CDT_FIELD), GetDlgItem( hwnd, MREQ_OUTPUT_FIELD ),
					".cdt", ".cdq", NULL );
				return TRUE;

			}
			return FALSE;

		case MREQ_GO:
			if( CurThread == NULL )
				CurThread = CreateThread( NULL, NULL, MakeRequestGO, hwnd, NULL, &ThreadID );
			  else {
				CloseHandle( CurThread );
				CurThread = CreateThread( NULL, NULL, MakeRequestGO, hwnd, NULL, &ThreadID );
			}
			return TRUE;

		case IDCANCEL:
			if( CurThread == NULL )
				EndDialog( hwnd, 0 );
			  else {
				GetExitCodeThread( CurThread, &ECode );
				if( ECode != STILL_ACTIVE ) {
					CloseHandle( CurThread );
					EndDialog( hwnd, 0 );
				}
			}
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK FillRequestGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
	
	OPENFILENAME	ofn;
	unsigned long	ThreadID;
	unsigned long	ECode;

	char	szFileName[MAX_PATH];
	HANDLE	CurThread = NULL;

	switch(Message)	{

	case WM_INITDIALOG:
		ActivateGenericDD( GetDlgItem( hwnd, FREQ_CDQ_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, FREQ_VFILE_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, FREQ_OUTPUT_FIELD ) );
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case FREQ_CDQ_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Checksum Files (*.cdq)\0*.cdq\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdq";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				FilenamePlacer( GetDlgItem( hwnd, FREQ_CDQ_FIELD), GetDlgItem( hwnd, FREQ_OUTPUT_FIELD ),
					".cdq", ".cdp", szFileName );

			return TRUE;

		case FREQ_VFILE_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				SetDlgItemText( hwnd, FREQ_VFILE_FIELD, szFileName );

			return TRUE;

		case FREQ_OUTPUT_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Patch Files (*.cdp)\0*.cdp\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdp";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

			if(GetSaveFileName(&ofn))
				SetDlgItemText( hwnd, FREQ_OUTPUT_FIELD, szFileName );
			return TRUE;

		case FREQ_CDQ_FIELD:
			switch( HIWORD(wParam) ) {

			case EN_UPDATE:
		
				FilenamePlacer( GetDlgItem( hwnd, FREQ_CDQ_FIELD), GetDlgItem( hwnd, FREQ_OUTPUT_FIELD ),
					".cdq", ".cdp", NULL );
				return TRUE;

			}
			return FALSE;

		case FREQ_GO:
			if( CurThread == NULL )
				CurThread = CreateThread( NULL, NULL, FillRequestGO, hwnd, NULL, &ThreadID );
			  else {
				CloseHandle( CurThread );
				CurThread = CreateThread( NULL, NULL, FillRequestGO, hwnd, NULL, &ThreadID );
			}
			return TRUE;

		case IDCANCEL:
			if( CurThread == NULL )
				EndDialog( hwnd, 0 );
			  else {
				GetExitCodeThread( CurThread, &ECode );
				if( ECode != STILL_ACTIVE ) {
					CloseHandle( CurThread );
					EndDialog( hwnd, 0 );
				}
			}
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK ApplyPatchGUI(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME	ofn;
	unsigned long	ThreadID;
	unsigned long	ECode;

	char	szFileName[MAX_PATH];
	HANDLE	CurThread = NULL;

	switch(Message)	{

	case WM_INITDIALOG:
		ActivateGenericDD( GetDlgItem( hwnd, APATCH_CDP_FIELD ) );
		ActivateGenericDD( GetDlgItem( hwnd, APATCH_PFILE_FIELD ) );
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {

		case APATCH_CDP_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Patch Files (*.cdp)\0*.cdp\0All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "cdp";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetOpenFileName(&ofn))
				SetDlgItemText( hwnd, APATCH_CDP_FIELD, szFileName );

			return TRUE;

		case APATCH_PFILE_BUTTON:

			ZeroMemory(&ofn, sizeof(ofn));
			szFileName[0] = '\0';

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if(GetSaveFileName(&ofn))
				SetDlgItemText( hwnd, APATCH_PFILE_FIELD, szFileName );

			return TRUE;

		case APATCH_GO:
			if( CurThread == NULL )
				CurThread = CreateThread( NULL, NULL, ApplyPatchGO, hwnd, NULL, &ThreadID );
			  else {
				CloseHandle( CurThread );
				CurThread = CreateThread( NULL, NULL, ApplyPatchGO, hwnd, NULL, &ThreadID );
			}
			return TRUE;

		case IDCANCEL:
			if( CurThread == NULL )
				EndDialog( hwnd, 0 );
			  else {
				GetExitCodeThread( CurThread, &ECode );
				if( ECode != STILL_ACTIVE ) {
					CloseHandle( CurThread );
					EndDialog( hwnd, 0 );
				}
			}
			return TRUE;

		}
		break;

	}
	return FALSE;

}


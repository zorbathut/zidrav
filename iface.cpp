
#include <afxcmn.h>
#include <afxwin.h>
#include <commdlg.h>
#include <fstream.h>

#include "iface.h"
#include "core.h"
#include "resource.h"
#include "utils.h"

int CALLBACK MakeChecksum(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME ofn;
	char szFileName[MAX_PATH];
	unsigned long ThreadDump;

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
		return FALSE;			// dunno what other processing it's gonna do, and this bit is not gonna conflict with anything

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
			CreateThread( NULL, NULL, MakeChecksumGO, hwnd, NULL, &ThreadDump );
			return TRUE;

		case IDCANCEL:
			if( IsWindowEnabled( GetDlgItem( hwnd, MCHECK_GO ) ) )
				EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK MakePatch( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
	
	unsigned long ThreadDump;

  	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

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
			CreateThread( NULL, NULL, MakePatchGO, hwnd, NULL, &ThreadDump );
			return TRUE;

		case IDCANCEL:
			if( IsWindowEnabled( GetDlgItem( hwnd, MPATCH_GO ) ) )
				EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK MakeRequest( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
	
  	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

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

		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK FillRequest( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
	
  	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

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


		case IDCANCEL:
			EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}

int CALLBACK ApplyPatch(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	OPENFILENAME ofn;
	char szFileName[MAX_PATH];
	unsigned long ThreadDump;

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
			CreateThread( NULL, NULL, ApplyPatchGO, hwnd, NULL, &ThreadDump );
			return TRUE;

		case IDCANCEL:
			if( IsWindowEnabled( GetDlgItem( hwnd, APATCH_GO ) ) )
				EndDialog(hwnd, 0);
			return TRUE;

		}
		break;

	}
	return FALSE;

}


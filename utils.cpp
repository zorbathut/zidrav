
#include <windows.h>

#include "utils.h"

int powerof2( int power ) {

	int static powertable[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384,
		32768, 65536, 131072, 262144, 524288 };

	return powertable[power];

}

void ActivateGenericDD( HWND hwnd ) {
	
	SetWindowLong( hwnd, GWL_USERDATA, (long)GetWindowLong( hwnd, GWL_WNDPROC ) );
	SetWindowLong( hwnd, GWL_WNDPROC, (long)GenericDD );

}

int CALLBACK GenericDD( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {

// Reflect WM_COMMAND messages to parent
	switch( message ) {
		
	case WM_COMMAND:
		return SendMessage( (HWND)GetWindowLong(hwnd, GWL_HWNDPARENT), message, wParam, lParam);

	case WM_DROPFILES:
		char filebuf[MAX_PATH];
		DragQueryFile( (HDROP)wParam, 0, filebuf, MAX_PATH );
		DragFinish( (HDROP)wParam );
		CallWindowProc( (WNDPROC)GetWindowLong( hwnd, GWL_USERDATA ), hwnd, WM_SETTEXT, 0, (long)filebuf );
		return FALSE;
		
	}

	return CallWindowProc( (WNDPROC)GetWindowLong( hwnd, GWL_USERDATA ), hwnd, message, wParam, lParam);
}

void FilenamePlacer( HWND first, HWND second, char origext[], char newext[], char fname[] ) {

	int loconf;
	int locone;

	BOOL fnameprov;

	if( !fname ) {
		fname = new char[MAX_PATH];
		SendMessage( first, WM_GETTEXT, MAX_PATH, (long)fname );
		fnameprov = FALSE;
	} else {
		SendMessage( first, WM_SETTEXT, 0, (long)fname );
		fnameprov = TRUE; }

	if( newext && second ) {
		if( origext ) {

			locone = strchr( origext, fname[loconf = strlen(fname) - 1] ) - origext;
			locone--; loconf--;

			while( locone > -1 && loconf > -1 && fname[loconf] == origext[locone] ) {
				locone--; loconf--; }

			if( locone == -1 )
				fname[ loconf + 1 ] = '\0';

		}

		strcat( fname, newext );
		SendMessage( second, WM_SETTEXT, 0, (long)fname );

	}

	if( !fnameprov )
		delete [] fname;

}

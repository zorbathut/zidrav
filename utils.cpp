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

void MakeNeededTree( char *path ) {

	char *floater;
	char temppath[MAX_PATH];

	floater = strchr( path, '\\' );
	while( floater = strchr( floater + 1, '\\' ) ) {
		strncpy( temppath, path, floater - path );
		temppath[ floater - path ] = '\0';
		if( GetFileAttributes( temppath ) == 0xFFFFFFFF )
			CreateDirectory( temppath, NULL );
	}

}
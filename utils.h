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
#ifndef _ZIDRAV_UTILS
#define _ZIDRAV_UTILS

#include <windows.h>

#define strncpyn( dest, source, len ) strncpy( dest, source, len ); dest[len] = '\0';

int powerof2( int power );		// Grabs a power of 2 from a table - be warned, only goes up to 65536

void ActivateGenericDD( HWND hwnd );
int CALLBACK GenericDD( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

void FilenamePlacer( HWND first, HWND second, char origext[], char newext[], char fname[] );

// make the command a li'l shorter
void FilenamePlacer( HWND first, HWND second, char newext[], char fname[] );	// omit origext[] for when there isn't
void FilenamePlacer( HWND first, HWND second, char newext[] );	// omit fname and getput for the "get" function
void FilenamePlacer( HWND first, char fname[] );	// for when the only thing that has to happen is a store

void MakeNeededTree( char *path );

#endif
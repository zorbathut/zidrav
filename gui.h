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
#ifndef _ZIDRAV_GUI
#define _ZIDRAV_GUI

#include <windows.h>

int CALLBACK MakeChecksumGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK MakeBatchChecksumGUI(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
int CALLBACK MakePatchGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK MakeRequestGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK FillRequestGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK ApplyPatchGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

#endif
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
#ifndef _ZIDRAV_GUIFACE
#define _ZIDRAV_GUIFACE

DWORD WINAPI MakeChecksumGO( LPVOID lpParameter );
void		 MakeChecksumStdGUIiface( int type, int param, void * data, void * edata );
void		 MakeChecksumLUL( HWND hwnd, BOOL lock );				// Lock/UnLock
void		 MakeChecksumPBT( HWND hwnd, BOOL pbvis );

DWORD WINAPI MakePatchGO( LPVOID lpParameter );
void		 MakePatchStdGUIiface( int type, int param, void * data, void * edata );
void		 MakePatchLUL( HWND hwnd, BOOL lock );
void		 MakePatchPBT( HWND hwnd, BOOL pbvis );

DWORD WINAPI MakeRequestGO( LPVOID lpParameter );
void		 MakeRequestStdGUIiface( int type, int param, void * data, void * edata );
void		 MakeRequestLUL( HWND hwnd, BOOL lock );
void		 MakeRequestPBT( HWND hwnd, BOOL pbvis );

DWORD WINAPI FillRequestGO( LPVOID lpParameter );
void		 FillRequestStdGUIiface( int type, int param, void * data, void * edata );
void		 FillRequestLUL( HWND hwnd, BOOL lock );

DWORD WINAPI ApplyPatchGO( LPVOID lpParameter );
void		 ApplyPatchStdGUIiface( int type, int param, void * data, void * edata );
void		 ApplyPatchLUL( HWND hwnd, BOOL lock );

#endif
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
#include <windows.h>

#include "guiface.h"
#include "iface.h"
#include "flayer.h"
#include "resource.h"

DWORD WINAPI MakeChecksumGO( LPVOID lpParameter ) {

	HWND hwnd = (HWND)lpParameter;

	char	inputFName[MAX_PATH];
	char	outputFName[MAX_PATH];

	int		blocksize;

	MakeChecksumLUL( hwnd, TRUE );

	GetDlgItemText( hwnd, MCHECK_INPUT_FIELD, inputFName, MAX_PATH );
	GetDlgItemText( hwnd, MCHECK_OUTPUT_FIELD, outputFName, MAX_PATH );
	blocksize = GetDlgItemInt( hwnd, MCHECK_BLOCKSIZE_FIELD, NULL, FALSE );

	MakeChecksumFLayer( inputFName, outputFName, blocksize, KStatIface( MakeChecksumStdGUIiface, hwnd ) );

	MakeChecksumLUL( hwnd, FALSE );

	return 1;

}

void MakeChecksumStdGUIiface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;
	char buffer[200];

	switch( type ) {

	case UPD_CURSTAT:
		if( IsWindowEnabled( GetDlgItem( hwnd, MCHECK_STATUS ) ) ) {
			CompileString( param, data, buffer, 200 );
			SetDlgItemText( hwnd, MCHECK_STATUS, buffer ); }
		break;
	
	case UPD_BIGSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MCHECK_STATUS, buffer );
		break;

	case UPD_PBRANGE:
		SendMessage( GetDlgItem( hwnd, MCHECK_PROGRESS_BAR ), PBM_SETRANGE32, 0, param );
		break;

	case UPD_PBSET:
		SendMessage( GetDlgItem( hwnd, MCHECK_PROGRESS_BAR ), PBM_SETPOS, param, NULL );
		break;

	case UPD_PBSHOW:
		MakeChecksumPBT( hwnd, TRUE );
		break;

	case UPD_PBHIDE:
		MakeChecksumPBT( hwnd, FALSE );
		break;

	}

}

void MakeChecksumLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, MCHECK_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, MCHECK_INPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MCHECK_INPUT_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MCHECK_OUTPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MCHECK_OUTPUT_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MCHECK_BLOCKSIZE_SLIDER ), !lock );
	EnableWindow( GetDlgItem( hwnd, MCHECK_BLOCKSIZE_FIELD ), !lock );

}

void MakeChecksumPBT( HWND hwnd, BOOL pbvis ) {

	ShowWindow( GetDlgItem( hwnd, MCHECK_STATUS ), !pbvis );
	ShowWindow( GetDlgItem( hwnd, MCHECK_PROGRESS_BAR ), pbvis );

}

DWORD WINAPI MakePatchGO( LPVOID lpParameter ) {

	HWND hwnd = (HWND)lpParameter;

	char	cdtFName[MAX_PATH];
	char	vfileFName[MAX_PATH];
	char	outputFName[MAX_PATH];

	MakePatchLUL( hwnd, TRUE );

	GetDlgItemText( hwnd, MPATCH_CDT_FIELD, cdtFName, MAX_PATH );
	GetDlgItemText( hwnd, MPATCH_VFILE_FIELD, vfileFName, MAX_PATH );
	GetDlgItemText( hwnd, MPATCH_OUTPUT_FIELD, outputFName, MAX_PATH );

	MakePatchFLayer( cdtFName, vfileFName, outputFName, KStatIface( MakePatchStdGUIiface, hwnd ) );

	MakePatchLUL( hwnd, FALSE );

	return 1;

}

void MakePatchStdGUIiface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;
	char buffer[200];

	switch( type ) {

	case UPD_CURSTAT:
		CompileString( param, data, buffer, 200 );
		if( IsWindowEnabled( GetDlgItem( hwnd, MPATCH_STATUS ) ) )
			SetDlgItemText( hwnd, MPATCH_STATUS, buffer );
		  else
			SetDlgItemText( hwnd, MPATCH_MINISTATUS, buffer );
		break;

	case UPD_MINISTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MPATCH_MINISTATUS, buffer );
		break;

	case UPD_BIGSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MPATCH_STATUS, buffer );
		break;

	case UPD_PBRANGE:
		SendMessage( GetDlgItem( hwnd, MPATCH_PROGRESS_BAR ), PBM_SETRANGE32, 0, param );
		break;

	case UPD_PBSET:
		SendMessage( GetDlgItem( hwnd, MPATCH_PROGRESS_BAR ), PBM_SETPOS, param, NULL );
		break;

	case UPD_PBSHOW:
		MakePatchPBT( hwnd, TRUE );
		break;

	case UPD_PBHIDE:
		MakePatchPBT( hwnd, FALSE );
		break;

	}

}

void MakePatchLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, MPATCH_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, MPATCH_CDT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MPATCH_CDT_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MPATCH_VFILE_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MPATCH_VFILE_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MPATCH_OUTPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MPATCH_OUTPUT_BUTTON ), !lock );

}

void MakePatchPBT( HWND hwnd, BOOL pbvis ) {

	ShowWindow( GetDlgItem( hwnd, MPATCH_PROGRESS_BAR ), pbvis );
	ShowWindow( GetDlgItem( hwnd, MPATCH_MINISTATUS ), pbvis );
	ShowWindow( GetDlgItem( hwnd, MPATCH_STATUS ), !pbvis );

}

DWORD WINAPI MakeRequestGO( LPVOID lpParameter ) {

	HWND hwnd = (HWND)lpParameter;

	char	cdtFName[MAX_PATH];
	char	dfileFName[MAX_PATH];
	char	outputFName[MAX_PATH];

	MakeRequestLUL( hwnd, TRUE );

	GetDlgItemText( hwnd, MREQ_CDT_FIELD, cdtFName, MAX_PATH );
	GetDlgItemText( hwnd, MREQ_DFILE_FIELD, dfileFName, MAX_PATH );
	GetDlgItemText( hwnd, MREQ_OUTPUT_FIELD, outputFName, MAX_PATH );

	MakeRequestFLayer( cdtFName, dfileFName, outputFName, KStatIface( MakeRequestStdGUIiface, hwnd ) );

	MakeRequestLUL( hwnd, FALSE );

	return 1;

}

void MakeRequestStdGUIiface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;
	char buffer[200];

	switch( type ) {

	case UPD_CURSTAT:
		CompileString( param, data, buffer, 200 );
		if( IsWindowEnabled( GetDlgItem( hwnd, MREQ_STATUS ) ) )
			SetDlgItemText( hwnd, MREQ_STATUS, buffer );
		  else
			SetDlgItemText( hwnd, MREQ_MINISTATUS, buffer );
		break;

	case UPD_MINISTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MREQ_MINISTATUS, buffer );
		break;

	case UPD_BIGSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MREQ_STATUS, buffer );
		break;

	case UPD_PBRANGE:
		SendMessage( GetDlgItem( hwnd, MREQ_PROGRESS_BAR ), PBM_SETRANGE32, 0, param );
		break;

	case UPD_PBSET:
		SendMessage( GetDlgItem( hwnd, MREQ_PROGRESS_BAR ), PBM_SETPOS, param, NULL );
		break;

	case UPD_PBSHOW:
		MakeRequestPBT( hwnd, TRUE );
		break;

	case UPD_PBHIDE:
		MakeRequestPBT( hwnd, FALSE );
		break;

	}

}

void MakeRequestLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, MREQ_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, MREQ_CDT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MREQ_CDT_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MREQ_DFILE_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MREQ_DFILE_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MREQ_OUTPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MREQ_OUTPUT_BUTTON ), !lock );

}

void MakeRequestPBT( HWND hwnd, BOOL pbvis ) {

	ShowWindow( GetDlgItem( hwnd, MREQ_PROGRESS_BAR ), pbvis );
	ShowWindow( GetDlgItem( hwnd, MREQ_MINISTATUS ), pbvis );
	ShowWindow( GetDlgItem( hwnd, MREQ_STATUS ), !pbvis );

}

DWORD WINAPI FillRequestGO( LPVOID lpParameter ) {

	HWND hwnd = (HWND)lpParameter;

	char	cdqFName[MAX_PATH];
	char	vfileFName[MAX_PATH];
	char	outputFName[MAX_PATH];

	FillRequestLUL( hwnd, TRUE );

	GetDlgItemText( hwnd, FREQ_CDQ_FIELD, cdqFName, MAX_PATH );
	GetDlgItemText( hwnd, FREQ_VFILE_FIELD, vfileFName, MAX_PATH );
	GetDlgItemText( hwnd, FREQ_OUTPUT_FIELD, outputFName, MAX_PATH );

	FillRequestFLayer( cdqFName, vfileFName, outputFName, KStatIface( FillRequestStdGUIiface, hwnd ) );

	FillRequestLUL( hwnd, FALSE );

	return 1;

}

void FillRequestStdGUIiface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;
	char buffer[200];

	switch( type ) {

	case UPD_CURSTAT:
	case UPD_BIGSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MREQ_STATUS, buffer );
		break;

	}

}

void FillRequestLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, FREQ_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, FREQ_CDQ_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, FREQ_CDQ_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, FREQ_VFILE_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, FREQ_VFILE_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, FREQ_OUTPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, FREQ_OUTPUT_BUTTON ), !lock );

}

DWORD WINAPI ApplyPatchGO( LPVOID lpParameter ) {

	HWND hwnd = (HWND)lpParameter;

	char	cdpFName[MAX_PATH];
	char	pfileFName[MAX_PATH];

	ApplyPatchLUL( hwnd, TRUE );

	GetDlgItemText( hwnd, APATCH_CDP_FIELD, cdpFName, MAX_PATH );
	GetDlgItemText( hwnd, APATCH_PFILE_FIELD, pfileFName, MAX_PATH );

	ApplyPatchFLayer( cdpFName, pfileFName, KStatIface( ApplyPatchStdGUIiface, hwnd ) );

	ApplyPatchLUL( hwnd, FALSE );

	return 1;

}

void ApplyPatchStdGUIiface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;
	char buffer[200];

	switch( type ) {

	case UPD_CURSTAT:
	case UPD_BIGSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, APATCH_STATUS, buffer );
		break;

	}

}

void ApplyPatchLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, APATCH_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_CDP_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_CDP_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_PFILE_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_PFILE_BUTTON ), !lock );

}

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

#include "iface.h"
#include "ghinst.h"

void CompileString( int param, void * data, char buffer[], int length ) {

	if( param ) {
		LoadString( g_hInst, param, buffer, length );
		return;
	} else {

		AlterStr *alter = (AlterStr *)data;

		switch( alter->type ) {
			
		case UPG_VFILESIZE:

			strcpy( buffer, "Error - verified file size is " );
			_itoa( (int)alter->varalpha, &buffer[strlen(buffer)], 10 );
			strcat( buffer, " instead of " );
			_itoa( (int)alter->varbeta, &buffer[strlen(buffer)], 10 );
			return;

		case UPG_DFILESIZE:

			strcpy( buffer, "Error - damaged file size is " );
			_itoa( (int)alter->varalpha, &buffer[strlen(buffer)], 10 );
			strcat( buffer, " instead of " );
			_itoa( (int)alter->varbeta, &buffer[strlen(buffer)], 10 );
			return;

		case UPG_PFILESIZE:

			strcpy( buffer, "Error - patch target file size is " );
			_itoa( (int)alter->varalpha, &buffer[strlen(buffer)], 10 );
			strcat( buffer, " instead of " );
			_itoa( (int)alter->varbeta, &buffer[strlen(buffer)], 10 );
			return;

		case UPG_VERSION:

			strcpy( buffer, "Error - file is v" );
			_itoa( ((char *)alter->varalpha)[0], &buffer[strlen(buffer)], 10 );
			strcat( buffer, "." );
			_itoa( ((char *)alter->varalpha)[1], &buffer[strlen(buffer)], 10 );
			strcat( buffer, ", program needs v" );
			_itoa( ((char *)alter->varbeta)[0], &buffer[strlen(buffer)], 10 );
			strcat( buffer, "." );
			_itoa( ((char *)alter->varbeta)[1], &buffer[strlen(buffer)], 10 );
			return;

		case UPG_CORR:

			strcpy( buffer, "Corruption found at " );
			_itoa( (int)alter->varalpha, &buffer[strlen(buffer)], 10 );
			return;

		case UPG_FILENAME:

			strcpy( buffer, (char *)alter->varalpha );
			return;

		}

	}

}
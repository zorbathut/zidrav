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
#ifndef _ZIDRAV_IFACE
#define _ZIDRAV_IFACE

class KStatIface {
public:
	void (*func)( int type, int param, void * data, void * edata );
	void * edata;

	KStatIface( void (*ifunc)( int, int, void *, void * ), void * iedata ) {
		func = ifunc;
		edata = iedata;
	};

	void operator()( int type, int param, void * data ) { func( type, param, data, edata ); };

};

class AlterStr {
public:
	int type;
	void * varalpha;
	void * varbeta;

};

enum {	UPD_NOACTION,

		UPD_CURSTAT,
		UPD_MINISTAT,
		UPD_BIGSTAT,
		UPD_UPPERSTAT,
		UPD_LOWERSTAT,

		UPD_PBRANGE,
		UPD_PBSET,

		UPD_PBSHOW,
		UPD_PBHIDE, };

enum {	UPG_NONE,
		UPG_VFILESIZE,
		UPG_DFILESIZE,
		UPG_PFILESIZE,
		UPG_VERSION,
		UPG_CORR,		
		UPG_FILENAME	};

void CompileString( int param, void * data, char buffer[], int length );

#endif
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
#ifndef _ZIDRAV_FLAYER
#define _ZIDRAV_FLAYER

#include "iface.h"

void MakeChecksumFLayer( char inputFName[], char outputFName[], int blocksize, KStatIface iface );
void MakePatchFLayer( char cdtFName[], char vfileFName[], char outputFName[], KStatIface iface );
void MakeRequestFLayer( char cdtFName[], char pfileFName[], char outputFName[], KStatIface iface );
void FillRequestFLayer( char cdqFName[], char vfileFName[], char outputFName[], KStatIface iface );
void ApplyPatchFLayer( char cdpFName[], char pfileFName[], KStatIface iface );

#endif
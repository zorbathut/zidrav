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
#ifndef _ZIDRAV_MCBATCH
#define _ZIDRAV_MCBATCH

#include <windows.h>

#include "iface.h"
#include "stack"

class KDirHeader;
class KDirTree;
class KFileHeader;
class KFileTree;

class KDTStackUnit;
class KFTStackUnit;

class KFileData;

class KHMaskChain;
class KMaskChain;

typedef std::stack< KDTStackUnit > KDTStack;
typedef std::stack< KFTStackUnit > KFTStack;

enum EBType;

class KDirHeader {
public:
	char				start[MAX_PATH];
	KDirTree			*down;
	KDTStack			kdtstack;

	KDirHeader( void );
	~KDirHeader( void );

	BOOL add( char *name, char *source, int size );
	BOOL traverse( KFileData *out );
};

class KDirTree {
public:
	KDirTree			*left;
	KDirTree			*right;
	KFileHeader			*down;
	int					branchtype;
	char				name[MAX_PATH];

	KDirTree( void );
	~KDirTree( void );
};

class KFileHeader {
public:
	KFileTree			*down;
	KFTStack			kftstack;

	KFileHeader( void );
	~KFileHeader( void );

	BOOL add( char *fname, char *source, int size );
	KFileTree* traverse( void );
};

class KFileTree {
public:
	KFileTree			*left;
	KFileTree			*right;
	int					branchtype;
	char				source[MAX_PATH];
	char				name[MAX_PATH];
	int					fsize;

	KFileTree( void );
	~KFileTree( void );
};

class KDTStackUnit {
public:
	KDirTree	*kdt;
	int			segment;
};

class KFTStackUnit {
public:
	KFileTree	*kft;
	int			segment;
};

class KFileData {
public:
	char				source[MAX_PATH];
	char				name[MAX_PATH];
	int					fsize;
	BOOL				newdir;
};

class KHMaskChain {
public:
	KMaskChain	*down;

	KHMaskChain( void );
	~KHMaskChain( void );

	int render( char *fullmask );
	int permit( char *fname );
};

class KMaskChain {
public:
	KMaskChain	*down;
	char		*mask;

	KMaskChain( void );
	~KMaskChain( void );

	int render( char *curmask );
	int permit( char *fname );
};

enum { ISHEAD, ISLEFT, ISRIGHT };
enum { DOINGREADY, DOINGLEFT, DOINGTHIS, DOINGRIGHT, DOINGBACK };

DWORD WINAPI MakeBatchChecksumGO( LPVOID lpParameter );
void		 MakeBatchChecksumMainIface( int type, int param, void * data, void * edata );
void		 MakeBatchChecksumMakerIface( int type, int param, void * data, void * edata );
void		 MakeBatchChecksumLUL( HWND hwnd, BOOL lock );
void		 MakeBatchChecksumPBT( HWND hwnd, BOOL pbvis );

int RCPrepare( char startdir[], char destdir[], int flatten, KDirHeader &start, int &count, long &totalsize, KHMaskChain &mchain, KStatIface iface );

int StarMatch(char *dat, char *pat, char *res[]);

#endif
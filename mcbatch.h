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

#include <string.h>

#include "mcbatch.h"
#include "stack"

KDirHeader::KDirHeader( void ) {
	start[0] = '\0';
	down = NULL;		}

KDirHeader::~KDirHeader( void ) {
	delete down;		}

BOOL KDirHeader::add( char *name, char *source, int size ) {

	KDirTree *kdtptr = down;
	KDirTree *nextlink = down;

	char	*fnameptr;
	char	fname[MAX_PATH];
	char	path[MAX_PATH];

	int		scomp;

	if( strnicmp( name, start, strlen( start ) ) )
		return 1;

	fnameptr = strrchr( name, '\\' ) + 1;
	strcpy( fname, fnameptr );
	strncpy( path, name, fnameptr - name );
	path[fnameptr - name] = '\0';

	while( nextlink != NULL ) {
		kdtptr = nextlink;
		if( !( scomp = stricmp( path, kdtptr->name ) ) )
			return kdtptr->down->add( fname, source, size );
		if( scomp < 0 )
			nextlink = kdtptr->left;
		  else
			nextlink = kdtptr->right;
	}							// nextlink == NULL, new directory entry needs to be made

	if( kdtptr == NULL ) {			// the first link doesn't exist
		down = new KDirTree;
		nextlink = down;
		nextlink->branchtype = ISHEAD;
	} else {
		if( scomp < 0 ) {
			kdtptr->left = new KDirTree;
			nextlink = kdtptr->left;
			nextlink->branchtype = ISLEFT;
		} else {
			kdtptr->right = new KDirTree;
			nextlink = kdtptr->right;
			nextlink->branchtype = ISRIGHT;
		}
	}								// whatever the entry is, it's now nextlink

	nextlink->down = new KFileHeader;
	strcpy( nextlink->name, path );
	return nextlink->down->add( fname, source, size );

}

BOOL KDirHeader::traverse( KFileData *out ) {

	KDTStackUnit	stacki;
	KFileTree*		kft;

	if( kdtstack.empty() ) {
		stacki.kdt = NULL;
		stacki.segment = NULL;
		kdtstack.push( stacki );
		stacki.kdt = down;
		stacki.segment = DOINGREADY;
	} else
		if( ( kft = kdtstack.top().kdt->down->traverse() ) != NULL ) {
			strcpy( out->source, kft->source );
			strcpy( out->name, kdtstack.top().kdt->name );
			strcat( out->name, kft->name );
			out->fsize = kft->fsize;
			out->newdir = FALSE;
			return TRUE;
		} else {
			stacki = kdtstack.top();
			kdtstack.pop();
		}
			

	while( ++stacki.segment != DOINGTHIS ) {	// . is higher precedance than ++

		switch( stacki.segment ) {

		case DOINGLEFT:
			if( stacki.kdt->left != NULL ) {
				kdtstack.push( stacki );
				stacki.kdt = stacki.kdt->left;
				stacki.segment = DOINGREADY;
			}
			break;

		case DOINGRIGHT:
			if( stacki.kdt->right != NULL ) {
				kdtstack.push( stacki );
				stacki.kdt = stacki.kdt->right;
				stacki.segment = DOINGREADY;
			}
			break;

		case DOINGBACK:
			stacki = kdtstack.top();
			kdtstack.pop();
			if( stacki.kdt == NULL )
				return FALSE;
			break;

		}

	}

	kdtstack.push( stacki );

	kft = stacki.kdt->down->traverse();
	strcpy( out->source, kft->source );
	strcpy( out->name, stacki.kdt->name );
	strcat( out->name, kft->name );
	out->fsize = kft->fsize;
	out->newdir = TRUE;
	return TRUE;

}

KDirTree::KDirTree( void ) {
	name[0] = '\0';
	left = right = NULL;
	down = NULL;		}

KDirTree::~KDirTree( void ) {
	delete left;
	delete right;
	delete down;		}

KFileHeader::KFileHeader( void ) {
	down = NULL;		}

KFileHeader::~KFileHeader( void ) {
	delete down;		}

BOOL KFileHeader::add( char *fname, char *source, int size ) {

	KFileTree *kftptr = down;
	KFileTree *nextlink = down;

	int		scomp;

	while( nextlink != NULL ) {
		kftptr = nextlink;
		if( !( scomp = stricmp( fname, kftptr->name ) ) )
			return 1;			// file already exists
		if( scomp < 0 )
			nextlink = kftptr->left;
		  else
			nextlink = kftptr->right;
	}							// nextlink == NULL, new file entry needs to be made

	if( kftptr == NULL ) {			// the first link doesn't exist
		down = new KFileTree;
		nextlink = down;
		nextlink->branchtype = ISHEAD;
	} else {
		if( scomp < 0 ) {
			kftptr->left = new KFileTree;
			nextlink = kftptr->left;
			nextlink->branchtype = ISLEFT;
		} else {
			kftptr->right = new KFileTree;
			nextlink = kftptr->right;
			nextlink->branchtype = ISRIGHT;
		}
	}								// whatever the entry is, it's now nextlink

	strcpy( nextlink->name, fname );
	strcpy( nextlink->source, source );
	nextlink->fsize = size;
	return 0;

}

KFileTree * KFileHeader::traverse( void ) {

	KFTStackUnit	stacki;

	if( kftstack.empty() ) {
		stacki.kft = NULL;
		stacki.segment = NULL;
		kftstack.push( stacki );
		stacki.kft = down;
		stacki.segment = DOINGREADY;
	} else {
		stacki = kftstack.top();
		kftstack.pop();
	}		

	while( ++stacki.segment != DOINGTHIS ) {	// . is higher precedance than ++

		switch( stacki.segment ) {

		case DOINGLEFT:
			if( stacki.kft->left != NULL ) {
				kftstack.push( stacki );
				stacki.kft = stacki.kft->left;
				stacki.segment = DOINGREADY;
			}
			break;

		case DOINGRIGHT:
			if( stacki.kft->right != NULL ) {
				kftstack.push( stacki );
				stacki.kft = stacki.kft->right;
				stacki.segment = DOINGREADY;
			}
			break;

		case DOINGBACK:
			stacki = kftstack.top();
			kftstack.pop();
			if( stacki.kft == NULL )
				return NULL;
			break;

		}

	}

	kftstack.push( stacki );

	return stacki.kft;

}

KFileTree::KFileTree( void ) {
	left = right = NULL;
	source[0] = name[0] = '\0';	}

KFileTree::~KFileTree( void ) {
	delete left;
	delete right;		}

/*class KHMaskChain {
public:
	KMaskChain	*down;

	KHMaskChain( void );
	~KHMaskChain( void );

	int render( char *fullmask );
	int permit( char *fname );
}*/

KHMaskChain::KHMaskChain( void ) {
	down = NULL; }

KHMaskChain::~KHMaskChain( void ) {
	delete down; }

int KHMaskChain::render( char *fullmask ) {
	if( strlen(fullmask) > 0 ) {
		down = new KMaskChain;
		return down->render( fullmask );
	} else
		return 0;
}

int KHMaskChain::permit( char *fname ) {
	if( down )
		return down->permit( fname );
	  else
		return 1;
}


/*class KMaskChain {
public:
	KMaskChain	*down;
	char		*mask;

	KMaskChain( void );
	~KMaskChain( void );

	int render( char *curmask );
	int permit( char *fname );
}*/

KMaskChain::KMaskChain( void ) {
	down = NULL;
	mask = NULL; }

KMaskChain::~KMaskChain( void ) {
	delete down;
	delete [] mask; }

int KMaskChain::render( char *curmask ) {
	
	char tempmask[MAX_PATH];
	char *mpoint;
	
	mpoint = strchr( curmask, ';' );

	if( mpoint == NULL )
		mpoint = curmask + strlen( curmask );

	strncpy( tempmask, curmask, mpoint - curmask );
	tempmask[ mpoint - curmask ] = '\0';

	if( strlen(tempmask) == 0 )
		return 1;
	
	mask = new char[ strlen(tempmask) + 1 ];
	strcpy( mask, tempmask );

	if( strlen(mpoint) > 0 ) {
		down = new KMaskChain;
		return down->render( mpoint + 1 );
	} else
		return 0;
}

int KMaskChain::permit( char *fname ) {
	if( StarMatch( fname, mask, NULL ) )
		return 0;
	  else
		if( down )
			return down->permit( fname );
		  else
			return 1;
}
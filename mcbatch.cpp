
#include <afxwin.h>
#include <windows.h>
#include <commdlg.h>
#include <winbase.h>
#include <io.h>

#include "mcbatch.h"
#include "resource.h"
#include "flayer.h"
#include "iface.h"

DWORD WINAPI MakeBatchChecksumGO( LPVOID lpParameter ) {

	HWND hwnd = (HWND)lpParameter;

	char	startdir[MAX_PATH];
	char	destdir[MAX_PATH];
	char	mask[MAX_PATH];

	char	temppath[MAX_PATH];
	char	*floater;

	int		flatten;
	int		count;

	int		blocksize;

	long	totalsize = 0;
	long	currentsize = 0;

	KFileData	filedat;
	AlterStr	astr;

	KDirHeader	start;		// MWAHAHAH! When the function ends, it ALL gets deallocated! KYAHAHAHAH!
	KHMaskChain	mchain;

	MakeBatchChecksumLUL( hwnd, TRUE );

	GetDlgItemText( hwnd, MBCHECK_INPUT_FIELD, startdir, MAX_PATH );
	GetDlgItemText( hwnd, MBCHECK_OUTPUT_FIELD, destdir, MAX_PATH );
	GetDlgItemText( hwnd, MBCHECK_MASK, mask, MAX_PATH );
	flatten = IsDlgButtonChecked( hwnd, MBCHECK_PRESERVE );
	if( flatten == BST_CHECKED ) flatten = 0; else flatten = 1;
	blocksize = GetDlgItemInt( hwnd, MBCHECK_BLOCKSIZE_FIELD, NULL, FALSE );

	if( startdir[strlen(startdir) - 1] != '\\' ) strcat( startdir, "\\");						// add a terminating backslash
	if( destdir[strlen(destdir) - 1] != '\\' ) strcat( destdir, "\\");

	strcpy( start.start, destdir );

	MakeBatchChecksumMainIface( UPD_UPPERSTAT, IDS_UPT_SCAN, NULL, hwnd );

	if( mchain.render( mask ) )
		MakeBatchChecksumMainIface( UPD_UPPERSTAT, IDS_UPT_INVMASK, NULL, hwnd );
	  else
		if( !RCPrepare( startdir, destdir, flatten, start, count, totalsize, mchain, KStatIface( MakeBatchChecksumMainIface, hwnd ) ) ) {
			MakeBatchChecksumMainIface( UPD_PBRANGE, totalsize, NULL, hwnd );
			MakeBatchChecksumMainIface( UPD_PBSET, 0, NULL, hwnd );
			MakeBatchChecksumMainIface( UPD_PBSHOW, NULL, NULL, hwnd );
			while( start.traverse( &filedat ) ) {
				astr.type = UPG_FILENAME;
				astr.varalpha = filedat.source;
				MakeBatchChecksumMainIface( UPD_LOWERSTAT, NULL, &astr, hwnd );
				MakeBatchChecksumMainIface( UPD_PBSET, currentsize, NULL, hwnd );
				if( filedat.newdir ) {
					floater = strchr( filedat.name, '\\' );
					while( floater = strchr( floater + 1, '\\' ) ) {
						strncpy( temppath, filedat.name, floater - filedat.name );
						temppath[ floater - filedat.name ] = '\0';
						if( access( temppath, 0 ) )
							CreateDirectory( temppath, NULL );
					}
				}
				MakeChecksumFLayer( filedat.source, filedat.name, blocksize, KStatIface( MakeBatchChecksumMakerIface, hwnd ) );
				currentsize += filedat.fsize;
			}
			MakeBatchChecksumMainIface( UPD_UPPERSTAT, IDS_UPT_COMPLETE, NULL, hwnd );
			MakeBatchChecksumMainIface( UPD_LOWERSTAT, IDS_UPT_CLEAR, NULL, hwnd );
		} else
			MakeBatchChecksumMainIface( UPD_UPPERSTAT, IDS_UPT_DUPENAMES, NULL, hwnd );
	
	MakeBatchChecksumMainIface( UPD_PBHIDE, NULL, NULL, hwnd );

	MakeBatchChecksumLUL( hwnd, FALSE );

	return 1;

}

int RCPrepare( char startdir[], char destdir[], int flatten, KDirHeader &start, int &count, long &totalsize, KHMaskChain &mchain, KStatIface iface ) {

	WIN32_FIND_DATA	curfile;
	HANDLE			findhandle;

	AlterStr		astr;

	char			tempstr[MAX_PATH];
	
	char			tempdeststr[MAX_PATH];
	char			tempsourstr[MAX_PATH];

	strcpy( tempstr, startdir );
	strcat( tempstr, "*.*" );

	if( ( findhandle = FindFirstFile( tempstr, &curfile ) ) != INVALID_HANDLE_VALUE ) {
	    do {

			if( ( curfile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
				   strcmp(curfile.cFileName, ".") && strcmp(curfile.cFileName, "..") ) {
				strcpy( tempdeststr, destdir );
				if( !flatten ) {
					strcat( tempdeststr, curfile.cFileName );
					strcat( tempdeststr, "\\" );
				}
				strcpy( tempsourstr, startdir );
				strcat( tempsourstr, curfile.cFileName );
				strcat( tempsourstr, "\\" );
				if( RCPrepare( tempsourstr, tempdeststr, flatten, start, count, totalsize, mchain, iface ) )
					return 1;		// flattening invalid
			} 
			if( !( curfile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
				if( mchain.permit( curfile.cFileName ) ) {
					strcpy( tempdeststr, destdir );
					strcat( tempdeststr, curfile.cFileName );
					strcat( tempdeststr, ".cdt" );
					strcpy( tempsourstr, startdir );
					strcat( tempsourstr, curfile.cFileName );
					if( !( count++ % 16 ) ) {
						astr.type = UPG_FILENAME;
						astr.varalpha = tempsourstr;
						iface( UPD_LOWERSTAT, NULL, &astr ); }
					if( start.add( tempdeststr, tempsourstr, curfile.nFileSizeLow / 1024 ) ) {
						astr.type = UPG_FILENAME;
						astr.varalpha = tempsourstr;
						iface( UPD_LOWERSTAT, NULL, &astr );
						return 1; }		// flattening invalid
					totalsize += curfile.nFileSizeLow / 1024;
				}
			}	

        } while( FindNextFile( findhandle, &curfile ) );

       FindClose( findhandle );
   }

   return 0;
}

void MakeBatchChecksumMainIface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;
	char buffer[200];

	switch( type ) {

	case UPD_UPPERSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MBCHECK_UPPER_STATUS, buffer );
		break;

	case UPD_LOWERSTAT:
		CompileString( param, data, buffer, 200 );
		SetDlgItemText( hwnd, MBCHECK_LOWER_STATUS, buffer );
		break;

	case UPD_PBRANGE:
		SendMessage( GetDlgItem( hwnd, MBCHECK_UPPER_PBAR ), PBM_SETRANGE32, 0, param );
		break;

	case UPD_PBSET:
		SendMessage( GetDlgItem( hwnd, MBCHECK_UPPER_PBAR ), PBM_SETPOS, param, NULL );
		break;

	case UPD_PBSHOW:
		MakeBatchChecksumPBT( hwnd, TRUE );
		break;

	case UPD_PBHIDE:
		MakeBatchChecksumPBT( hwnd, FALSE );
		break;

	}

}

void MakeBatchChecksumMakerIface( int type, int param, void * data, void * edata ) {

	HWND hwnd = (HWND)edata;

	switch( type ) {

	case UPD_PBRANGE:
		SendMessage( GetDlgItem( hwnd, MBCHECK_LOWER_PBAR ), PBM_SETRANGE32, 0, param );
		break;

	case UPD_PBSET:
		SendMessage( GetDlgItem( hwnd, MBCHECK_LOWER_PBAR ), PBM_SETPOS, param, NULL );
		break;

	}

}

void MakeBatchChecksumLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, MBCHECK_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_INPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_INPUT_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_OUTPUT_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_OUTPUT_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_BLOCKSIZE_SLIDER ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_BLOCKSIZE_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_MASK ), !lock );
	EnableWindow( GetDlgItem( hwnd, MBCHECK_PRESERVE ), !lock );

}

void MakeBatchChecksumPBT( HWND hwnd, BOOL pbstate ) {

	ShowWindow( GetDlgItem( hwnd, MBCHECK_UPPER_PBAR ), pbstate );
	ShowWindow( GetDlgItem( hwnd, MBCHECK_UPPER_STATUS ), !pbstate );
	ShowWindow( GetDlgItem( hwnd, MBCHECK_LOWER_PBAR ), pbstate );

}

int StarMatch(char *dat, char *pat, char *res[])
{ 
  char *star = NULL;
  char *starend;
  char *resp;
  int nres = 0;
  int c1;
  int c2;

  while (1) {
    if (*pat == '*') { 
      star = ++pat; 			         /* Pattern after * */
      starend = dat; 			         /* Data after * match */
      if (res) {
        nres++;
        resp = res[nres]; 		     /* Result string */
        *resp = 0;   			         /* Initially null */
      }
    }
    else {
      c1 = tolower(*dat);
      if (*pat == '\\') {          /* check for \ in pattern */
        pat++;
        c2 = tolower(*pat); 
      }
      else {
        c2 = tolower(*pat);
        if (c2 == '?')               /* check for ? in pattern */
          c2 = c1;
      }
      if (c1 == c2) {              /* Characters match */
        if (*pat == 0) 		           /* Pattern matches */
	        return 1;
        pat++; 				               /* Try next position */
        dat++;
      }
      else { 
        if (*dat == 0) 		           /* Pattern fails - no more */
	        return 0;                  /* data */
        if (star == 0) 			         /* Pattern fails - no * to */
	        return 0; 			           /* adjust */
        pat = star; 			           /* Restart pattern after * */
        if (res) {
          *resp++ = *starend; 		     /* Copy character to result */
          *resp = 0; 			             /* null terminate */
        }
        dat = ++starend; 			       /* Rescan after copied char */
      }
    }
  }
}


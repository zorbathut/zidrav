
#include <fstream.h>
#include <sys/stat.h>
#include <afxcmn.h>
#include <afxwin.h>
#include <commdlg.h>
#include <winbase.h>
#include <windows.h>

#include "core.h"
#include "utils.h"
#include "resource.h"

DWORD WINAPI MakeChecksumGO( LPVOID lpParameter ) {

	HWND	hwnd = (HWND)lpParameter;


	long		crc;	// the current CRC value
	int			cdtlen;	// length of the .CDT, used for the final reverification

	struct stat	filespecs;

	int			i;		// generic looping var

	int			blocksize;
	char		*dataBuffer;
	char		nameBuffer[MAX_PATH];

	ifstream	inFile;
	fstream		outFile;				// possibly a bit misleading, but it's easier than copying the data to a buffer

	MakeChecksumLUL( hwnd, TRUE );		// Don't let them leave! KYAHAHAAHAHAHAHAHAAH!

	SetDlgItemText( hwnd, MCHECK_STATUS, "Processing . . ." );

	blocksize = powerof2( SendDlgItemMessage( hwnd, MCHECK_BLOCKSIZE_SLIDER, TBM_GETPOS, 0, 0) );
	dataBuffer = new char[blocksize];

	GetDlgItemText( hwnd, MCHECK_INPUT_FIELD, nameBuffer, MAX_PATH );
	inFile.open( nameBuffer, ios::binary );
	if( !inFile ) {
		SetDlgItemText( hwnd, MCHECK_STATUS, "Error opening input file" );
		delete [] dataBuffer;
		MakeChecksumLUL( hwnd, FALSE );
		return 1; }					// Open input file

	stat( nameBuffer, &filespecs );	// Get the file size, while the nameBuffer is set properly

	GetDlgItemText( hwnd, MCHECK_OUTPUT_FIELD, nameBuffer, MAX_PATH );
	outFile.open( nameBuffer, ios::binary | ios::in | ios::out );
	if( !inFile ) {
		SetDlgItemText( hwnd, MCHECK_STATUS, "Error opening output file" );
		delete [] dataBuffer;
		inFile.close();
		MakeChecksumLUL( hwnd, FALSE );
		return 1; }					// Open output file

	outFile.write( ZC, 8 );
	outFile.write( CVER, 2 );
	outFile.write( (char *)&filespecs.st_size, 4 );
	outFile.write( (char *)&blocksize, 4 );

	SendMessage( GetDlgItem( hwnd, MCHECK_PROGRESS_BAR ), PBM_SETPOS, 0, 0 );
	SendMessage( GetDlgItem( hwnd, MCHECK_PROGRESS_BAR ), PBM_SETRANGE32, 0, filespecs.st_size );
	MakeChecksumPBT( hwnd, TRUE );		// Progress bar armed and ready, Captain

	for( filespecs.st_size % blocksize ? i = -1 : i = 0; i < filespecs.st_size / blocksize; i++ ) {

		if( i >= 0 && !( i % 32 ) )
			SendMessage( GetDlgItem( hwnd, MCHECK_PROGRESS_BAR ), PBM_SETPOS, i * blocksize, 0 );

		inFile.read( dataBuffer, blocksize );

		if( i == filespecs.st_size / blocksize - 1 )	// If it's the end of the file, only make the checksum for what's left
			CreateChecksum( dataBuffer, filespecs.st_size % blocksize, &crc );
		  else
			CreateChecksum( dataBuffer, blocksize, &crc );

		outFile.write( (char *)&crc, 4 );

	}

	inFile.close();

	SetDlgItemText( hwnd, MCHECK_STATUS, "Creating verification checksum . . ." );
	MakeChecksumPBT( hwnd, FALSE );

	delete [] dataBuffer;

	cdtlen = 18 + ( filespecs.st_size / blocksize + 1 ) * 4;		// time to calculate this

	dataBuffer = new char[ cdtlen ];

	outFile.flush();
	outFile.seekp( 0 );

	outFile.read( dataBuffer, cdtlen );

	CreateChecksum( dataBuffer, cdtlen, &crc );

	outFile.write( (char *)&crc, 4 );

	inFile.close();
	outFile.close();
	delete [] dataBuffer;

	SetDlgItemText( hwnd, MCHECK_STATUS, "Completed" );
	MakeChecksumLUL( hwnd, FALSE );	// Go on, get outa here

	Sleep( 4000 );

	dataBuffer = new char[120];

	GetDlgItemText( hwnd, MCHECK_STATUS, dataBuffer, 120 );

	if( !strcmp( dataBuffer, "Completed" ) )
		SetDlgItemText( hwnd, MCHECK_STATUS, "Ready" );

	return 0;

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

	HWND	hwnd = (HWND)lpParameter;

	char		minibuff[9];	// used for file verification purposes

	long		crc;		// the current CRC value
	long		templen;	// generic temporary length holder :)
	long		minitemp;	// another generic temp variable - look, I find them easy, OK?
	long		curbs;		// current blocksize
	long		curcdplen;	// current CDP length

	int			cfound = 0;		// corruption found?

	struct stat	cdspecs;
	struct stat	vspecs;

	int			i;		// generic looping var

	long		blocksize;
	char		*dataBuffer;
	char		*cdtPoint;		// moving pointer on the cdt buffer
	char		*cdtBuffer;		// the whole thing has to be read in anyway in order to check the final checksum - might
								// as well keep it there!
	char		nameBuffer[MAX_PATH];

	ifstream	cdtFile;
	ifstream	vFile;
	fstream		outFile;				// possibly a bit misleading, but it's easier than copying the data to a buffer

	MakePatchLUL( hwnd, TRUE );		// Don't let them leave! KYAHAHAAHAHAHAHAHAAH!

	SetDlgItemText( hwnd, MPATCH_STATUS, "Processing . . ." );

	GetDlgItemText( hwnd, MPATCH_CDT_FIELD, nameBuffer, MAX_PATH );
	cdtFile.open( nameBuffer, ios::binary );
	if( !cdtFile ) {
		SetDlgItemText( hwnd, MPATCH_STATUS, "Error opening .CDT file" );
		MakePatchLUL( hwnd, FALSE );
		return 1; }		// Open input file
	stat( nameBuffer, &cdspecs );

	if( cdspecs.st_size < 22 ) {	// 22 == minimum size - 8 for signature, 2 for version, 4 for blocksize, 4 for filesize,
		cdtFile.close();			// 4 for comparison checksum - who says it can't be a .cdt of a 0-size file? :)
		SetDlgItemText( hwnd, MPATCH_STATUS, ".CDT file is too small to be valid" );
		MakePatchLUL( hwnd, FALSE );
		return 1; }		

	cdtBuffer = new char[18];

	cdtFile.read( cdtBuffer, 18 );	// Get the header, don't want to accidentally load a 100 meg file unless we have to :)

	strncpyn( minibuff, cdtBuffer, 8 );	// Copy the signature in
	
	if( strcmp( minibuff, ZC ) ) {
		delete [] cdtBuffer;
		cdtFile.close();
		SetDlgItemText( hwnd, MPATCH_STATUS, ".CDT file signature invalid" );
		MakePatchLUL( hwnd, FALSE );
		return 1; }

	strncpyn( minibuff, &cdtBuffer[8], 2 ); // Version number comparison

	if( strcmp( minibuff, CVER ) ) {

		char	emsg[80] = "Invalid version number - file is v";		// error message
		int		cv;				// converter int

		cdtFile.close();
		delete [] cdtBuffer;

		cv = minibuff[0];
		_itoa( cv, &emsg[strlen(emsg)], 10 );
		strcat( emsg, "." );
		cv = minibuff[1];
		_itoa( cv, &emsg[strlen(emsg)], 10 );
		strcat( emsg, ", program needs v" );

		strcpy( minibuff, CVER);
		cv = minibuff[0];
		_itoa( cv, &emsg[strlen(emsg)], 10 );
		strcat( emsg, "." );
		cv = minibuff[1];
		_itoa( cv, &emsg[strlen(emsg)], 10 );

		SetDlgItemText( hwnd, MPATCH_STATUS, emsg );
		MakePatchLUL( hwnd, FALSE );

		return 1; }

	templen = *(long *)&cdtBuffer[10];
	blocksize = *(long *)&cdtBuffer[14];
	delete [] cdtBuffer;				// won't be needing it in this size anymore

	if( cdspecs.st_size != 22 + ( ( templen - 1 ) / blocksize + 1 ) * 4 ) {
		cdtFile.close();
		SetDlgItemText( hwnd, MPATCH_STATUS, ".CDT is incomplete" );
		MakePatchLUL( hwnd, FALSE );
		return 1; }

	GetDlgItemText( hwnd, MPATCH_VFILE_FIELD, nameBuffer, MAX_PATH );
	vFile.open( nameBuffer, ios::binary );
	if( !vFile ) {
		cdtFile.close();
		SetDlgItemText( hwnd, MPATCH_STATUS, "Error opening verified file" );
		MakePatchLUL( hwnd, FALSE );
		return 1; }		// Open verified file

	stat( nameBuffer, &vspecs );

	if( templen != vspecs.st_size ) {

		char emsg[80] = "Verified file size is ";

		cdtFile.close();
		vFile.close();

		_itoa( vspecs.st_size, &emsg[strlen(emsg)], 10 );
		strcat( emsg, " instead of " );
		_itoa( templen, &emsg[strlen(emsg)], 10 );

		SetDlgItemText( hwnd, MPATCH_STATUS, emsg );
		MakePatchLUL( hwnd, FALSE );
		return 1; }

	// Okay, everything that can go wrong has at this point, let's load the .cdt into memory and verify its checksum

	templen = cdspecs.st_size - 4;

	cdtBuffer = new char[ cdspecs.st_size ];		// want to load the whole thing

	cdtFile.seekg( 0 );								// go to the beginning
	cdtFile.read( cdtBuffer, cdspecs.st_size );	// and pull it all

	cdtFile.close();								// might as well close the .CDT now, it's all in memory

	CreateChecksum( cdtBuffer, templen, &crc );

	if( crc != *(int *)&cdtBuffer[ templen ] ) {
		vFile.close();
		delete [] cdtBuffer;
		SetDlgItemText( hwnd, MPATCH_STATUS, ".CDT internal checksum failed" );
		MakePatchLUL( hwnd, FALSE );
		return 1; }

	dataBuffer = new char[ blocksize ];

	// Now if we've gotten THIS far, we can actually get around to doing something useful :)

	SetDlgItemText( hwnd, MPATCH_MINISTATUS, "Scanning . . ." );
	SendMessage( GetDlgItem( hwnd, MPATCH_PROGRESS_BAR ), PBM_SETPOS, 0, 0 );
	SendMessage( GetDlgItem( hwnd, MPATCH_PROGRESS_BAR ), PBM_SETRANGE32, 0, vspecs.st_size );
	MakePatchPBT( hwnd, TRUE );		// Progress bar armed and ready, Captain

	cdtPoint = cdtBuffer + 18;

	for( vspecs.st_size % blocksize ? i = -1 : i = 0; i < vspecs.st_size / blocksize; i++ ) {

		if( i >= 0 && !( i % 32 ) )
			SendMessage( GetDlgItem( hwnd, MPATCH_PROGRESS_BAR ), PBM_SETPOS, i * blocksize, 0 );

		if( i == vspecs.st_size / blocksize - 1 )
			curbs = vspecs.st_size % blocksize;
		  else
			curbs = blocksize;

		if( curbs == 0 )
			curbs = blocksize;		// it's possible - if the file is a multiple of blocksize

		vFile.read( dataBuffer, curbs );

		CreateChecksum( dataBuffer, curbs, &crc );

		if( crc != *(long *)cdtPoint ) {

			char statbuf[80] = "Corruption found at ";
			
			minitemp = i * blocksize + blocksize;
			_itoa( minitemp, &statbuf[strlen(statbuf)], 10 );

			SetDlgItemText( hwnd, MPATCH_MINISTATUS, statbuf );

			if( !cfound ) { // file has to be created

				GetDlgItemText( hwnd, MPATCH_OUTPUT_FIELD, nameBuffer, MAX_PATH );
				outFile.open( nameBuffer, ios::binary | ios::in | ios::out | ios::trunc );

				outFile.write( ZP, 8 );
				outFile.write( PVER, 2 );
				outFile.write( (char *)&(vspecs.st_size), 4 );

				curcdplen = 14;

				cfound = 1;

			}				// file now exists, continue as normal

			outFile.write( (char *)&minitemp, 4 );	// write the current offset
			outFile.write( (char *)&curbs, 4 );
			outFile.write( dataBuffer, curbs );

			curcdplen += 8 + curbs;
		
		}
		
		cdtPoint += 4;

	}

	vFile.close();

	delete [] dataBuffer;
	delete [] cdtBuffer;

	if( cfound ) {

		SetDlgItemText( hwnd, MPATCH_STATUS, "Creating verification checksum . . ." );
		MakePatchPBT( hwnd, FALSE );

		minitemp = -1;

		outFile.write( (char *)&minitemp, 4 );

		curcdplen += 4;

		outFile.flush();

		dataBuffer = new char[ curcdplen ];

		outFile.seekp( 0 );

		outFile.read( dataBuffer, curcdplen );

		CreateChecksum( dataBuffer, curcdplen, &crc );

		outFile.write( (char *)&crc, 4 );

		outFile.close();
		delete [] dataBuffer;

		SetDlgItemText( hwnd, MPATCH_STATUS, "Completed, patch file generated" );

	} else {

		SetDlgItemText( hwnd, MPATCH_STATUS, "Completed, no corruption found" );
		MakePatchPBT( hwnd, FALSE );

	}

	MakePatchLUL( hwnd, FALSE );	// Go on, get outa here

	Sleep( 4000 );					// These next few lines are what separate good designers/programmers from bad ones :)

	dataBuffer = new char[120];	

	GetDlgItemText( hwnd, MPATCH_STATUS, dataBuffer, 120 );

	if( !strcmp( dataBuffer, "Completed, patch file generated" ) || !strcmp( dataBuffer, "Completed, no corruption found" ) )
		SetDlgItemText( hwnd, MPATCH_STATUS, "Ready" );		// I hope so, at least.

	return 0;

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

DWORD WINAPI ApplyPatchGO( LPVOID lpParameter ) {

	HWND	hwnd = (HWND)lpParameter;

	char		minibuff[9];	// used for file verification purposes
	char		mess[80];

	long		crc;		// the current CRC value
	long		templen;	// generic temporary length holder :)

	int			offset;

	struct stat	cdspecs;
	struct stat	pspecs;

	long		blocksize;
	char		*cdpPoint;		// moving pointer on the cdp buffer
	char		*cdpBuffer;		// the whole thing has to be read in anyway in order to check the final checksum - might
								// as well keep it there!
	char		nameBuffer[MAX_PATH];

	ifstream	cdpFile;
	fstream		pFile;

	ApplyPatchLUL( hwnd, TRUE );		// Don't let them leave! KYAHAHAAHAHAHAHAHAAH!

	SetDlgItemText( hwnd, APATCH_STATUS, "Processing . . ." );

	GetDlgItemText( hwnd, APATCH_CDP_FIELD, nameBuffer, MAX_PATH );
	cdpFile.open( nameBuffer, ios::binary );
	if( !cdpFile ) {
		SetDlgItemText( hwnd, APATCH_STATUS, "Error opening .CDP file" );
		ApplyPatchLUL( hwnd, FALSE );
		return 1; }		// Open input file
	stat( nameBuffer, &cdspecs );

	if( cdspecs.st_size < 22 ) {	// 22 == minimum size - 8 for signature, 2 for version, 4 for filesize, 4 for first blocksize,
		cdpFile.close();			// 4 for comparison checksum - old versions would create .cdps with no data, bah
		SetDlgItemText( hwnd, APATCH_STATUS, ".CDP file is too small to be valid" );
		ApplyPatchLUL( hwnd, FALSE );
		return 1; }		

	cdpBuffer = new char[14];

	cdpFile.read( cdpBuffer, 14 );	// Get the header, don't want to accidentally load a 100 meg file unless we have to :)

	strncpyn( minibuff, cdpBuffer, 8 );	// Copy the signature in
	
	if( strcmp( minibuff, ZP ) ) {
		delete [] cdpBuffer;
		cdpFile.close();
		SetDlgItemText( hwnd, APATCH_STATUS, ".CDP file signature invalid" );
		ApplyPatchLUL( hwnd, FALSE );
		return 1; }

	strncpyn( minibuff, &cdpBuffer[8], 2 ); // Version number comparison

	if( strcmp( minibuff, PVER ) ) {

		char	emsg[80] = "Invalid version number - file is v";		// error message
		int		cv;				// converter int

		cdpFile.close();
		delete [] cdpBuffer;

		cv = minibuff[0];
		_itoa( cv, &emsg[strlen(emsg)], 10 );
		strcat( emsg, "." );
		cv = minibuff[1];
		_itoa( cv, &emsg[strlen(emsg)], 10 );
		strcat( emsg, ", program needs v" );

		strcpy( minibuff, PVER);
		cv = minibuff[0];
		_itoa( cv, &emsg[strlen(emsg)], 10 );
		strcat( emsg, "." );
		cv = minibuff[1];
		_itoa( cv, &emsg[strlen(emsg)], 10 );

		SetDlgItemText( hwnd, APATCH_STATUS, emsg );
		ApplyPatchLUL( hwnd, FALSE );

		return 1; }

	templen = *(long *)&cdpBuffer[10];
	delete [] cdpBuffer;				// won't be needing it in this size anymore

	GetDlgItemText( hwnd, APATCH_PFILE_FIELD, nameBuffer, MAX_PATH );
	pFile.open( nameBuffer, ios::binary | ios::in | ios::out );
	if( !pFile ) {
		cdpFile.close();
		SetDlgItemText( hwnd, APATCH_STATUS, "Error opening file to patch" );
		ApplyPatchLUL( hwnd, FALSE );
		return 1; }		// Open verified file

	stat( nameBuffer, &pspecs );

	if( templen != pspecs.st_size ) {

		char emsg[80] = "Patch target size is ";

		cdpFile.close();
		pFile.close();

		_itoa( pspecs.st_size, &emsg[strlen(emsg)], 10 );
		strcat( emsg, " instead of " );
		_itoa( templen, &emsg[strlen(emsg)], 10 );

		SetDlgItemText( hwnd, APATCH_STATUS, emsg );
		ApplyPatchLUL( hwnd, FALSE );
		return 1; }

	// Okay, everything that can go wrong has at this point, let's load the .cdp into memory and verify its checksum

	templen = cdspecs.st_size - 4;

	cdpBuffer = new char[ cdspecs.st_size ];		// want to load the whole thing

	cdpFile.seekg( 0 );								// go to the beginning
	cdpFile.read( cdpBuffer, cdspecs.st_size );		// and pull it all

	cdpFile.close();								// might as well close the .CDP now, it's all in memory

	CreateChecksum( cdpBuffer, templen, &crc );

	if( crc != *(int *)&cdpBuffer[ templen ] ) {
		pFile.close();
		delete [] cdpBuffer;
		SetDlgItemText( hwnd, APATCH_STATUS, ".CDP internal checksum failed" );
		ApplyPatchLUL( hwnd, FALSE );
		return 1; }

	// Now if we've gotten THIS far, we can actually get around to doing something useful :)

	cdpPoint = cdpBuffer + 14;

	while( ( offset = *(int *)cdpPoint ) != -1 ) {

		strcpy( mess, "Patching block at offset " );
		_itoa( offset, &mess[strlen(mess)], 10 );

		SetDlgItemText( hwnd, APATCH_STATUS, mess );

		blocksize = *(int *)(cdpPoint + 4);

		cdpPoint += 8;

		pFile.seekg( offset );

		pFile.write( cdpPoint, blocksize );

		cdpPoint += blocksize;

	}

	pFile.close();

	delete [] cdpBuffer;

	SetDlgItemText( hwnd, APATCH_STATUS, "File patched" );

	ApplyPatchLUL( hwnd, FALSE );	// Go on, get outa here

	Sleep( 4000 );					// These next few lines are what separate good designers/programmers from bad ones :)

	cdpBuffer = new char[120];	

	GetDlgItemText( hwnd, APATCH_STATUS, cdpBuffer, 120 );

	if( !strcmp( cdpBuffer, "File patched" ) )
		SetDlgItemText( hwnd, APATCH_STATUS, "Ready" );		// I hope so, at least.

	return 0;

}

void ApplyPatchLUL( HWND hwnd, BOOL lock ) {

	EnableWindow( GetDlgItem( hwnd, APATCH_GO ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_CDP_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_CDP_BUTTON ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_PFILE_FIELD ), !lock );
	EnableWindow( GetDlgItem( hwnd, APATCH_PFILE_BUTTON ), !lock );

}

#define DO1( buffer ) *crc = crc_table[ ( *crc ^ *(buffer++) ) & 0xff ] ^ (*crc >> 8);
#define DO2( buffer )  DO1( buffer ); DO1( buffer );
#define DO4( buffer )  DO2( buffer ); DO2( buffer );
#define DO8( buffer )  DO4( buffer ); DO4( buffer );
#define DO16( buffer )  DO8( buffer ); DO8( buffer );

void CreateChecksum( char *buffer, long size, long *crc ) {

	*crc = 0;

    *crc ^= 0xffffffffL;

    while ( size >= 16 )
    {
      DO16( buffer );
      size -= 16;
    }

    while ( size ) {
      DO1( buffer );
	  size--;
    } 

    *crc ^= 0xffffffffL;
}

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
#include <fstream.h>
#include <sys\stat.h>

#include "flayer.h"
#include "core.h"
#include "iface.h"
#include "resource.h"

void MakeChecksumFLayer( char inputFName[], char outputFName[], int blocksize, KStatIface iface ) {

	struct stat	filespecs;

	ifstream	inFile;
	fstream		outFile;				// possibly a bit misleading, but it's easier than copying the data to a buffer

	iface( UPD_BIGSTAT, IDS_UPT_PROCESS, NULL );

	stat( inputFName, &filespecs );

	inFile.open( inputFName, ios::binary );
	if( !inFile ) {
		iface( UPD_BIGSTAT, IDS_UPT_ERROINP, NULL );
		return; }					// Open input file

	outFile.open( outputFName, ios::binary | ios::in | ios::out | ios::trunc );
	if( !outFile ) {
		iface( UPD_BIGSTAT, IDS_UPT_ERROOUT, NULL );
		inFile.close();
		return; }					// Open output file

	MakeChecksumCore( inFile, outFile, blocksize, filespecs.st_size, iface );

	inFile.close();
	outFile.close();

	iface( UPD_BIGSTAT, IDS_UPT_COMPLETE, NULL );

}

void MakePatchFLayer( char cdtFName[], char vfileFName[], char outputFName[], KStatIface iface ) {

	struct stat	cdspecs;
	struct stat	vspecs;

	int			efound;

	ifstream	cdtFile;
	ifstream	vFile;
	fstream		outFile;				// possibly a bit misleading, but it's easier than copying the data to a buffer

	iface( UPD_BIGSTAT, IDS_UPT_PROCESS, NULL );

	stat( cdtFName, &cdspecs );
	stat( vfileFName, &vspecs );

	cdtFile.open( cdtFName, ios::binary );
	if( !cdtFile ) {
		iface( UPD_BIGSTAT, IDS_UPT_ERROCDT, NULL );
		return; }		// Open input file

	vFile.open( vfileFName, ios::binary );
	if( !vFile ) {
		cdtFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERROVFILE, NULL );
		return; }		// Open verified file

	outFile.open( outputFName, ios::binary | ios::in | ios::out | ios::trunc );
	if( !outFile ) {
		cdtFile.close();
		vFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERROOUT, NULL );
		return; }		// Open output file

	MakePatchCore( cdtFile, vFile, outFile, cdspecs.st_size, vspecs.st_size, &efound, iface );

	cdtFile.close();
	vFile.close();
	outFile.close();

	if( efound )
		iface( UPD_BIGSTAT, IDS_UPT_CCDPGEN, NULL );
	  else {
		DeleteFile( outputFName );
		iface( UPD_BIGSTAT, IDS_UPT_CNOCORR, NULL );
	}

}

void MakeRequestFLayer( char cdtFName[], char dfileFName[], char outputFName[], KStatIface iface ) {

	struct stat	cdspecs;
	struct stat	dspecs;

	int			efound;

	ifstream	cdtFile;
	ifstream	dFile;
	fstream		outFile;				// possibly a bit misleading, but it's easier than copying the data to a buffer

	iface( UPD_BIGSTAT, IDS_UPT_PROCESS, NULL );

	stat( cdtFName, &cdspecs );
	stat( dfileFName, &dspecs );

	cdtFile.open( cdtFName, ios::binary );
	if( !cdtFile ) {
		iface( UPD_BIGSTAT, IDS_UPT_ERROCDT, NULL );
		return; }		// Open input file

	dFile.open( dfileFName, ios::binary );
	if( !dFile ) {
		cdtFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERRODFILE, NULL );
		return; }		// Open damaged file

	outFile.open( outputFName, ios::binary | ios::in | ios::out | ios::trunc );
	if( !outFile ) {
		cdtFile.close();
		dFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERROOUT, NULL );
		return; }		// Open output file

	MakeRequestCore( cdtFile, dFile, outFile, cdspecs.st_size, dspecs.st_size, &efound, iface );

	cdtFile.close();
	dFile.close();
	outFile.close();

	if( efound )
		iface( UPD_BIGSTAT, IDS_UPT_CCDQGEN, NULL );
	  else {
		DeleteFile( outputFName );
		iface( UPD_BIGSTAT, IDS_UPT_CNOCORR, NULL );
	}

}

void FillRequestFLayer( char cdqFName[], char vfileFName[], char outputFName[], KStatIface iface ) {

	struct stat	cdspecs;
	struct stat	dspecs;

	int			efound;

	ifstream	cdqFile;
	ifstream	vFile;
	fstream		outFile;				// possibly a bit misleading, but it's easier than copying the data to a buffer

	iface( UPD_BIGSTAT, IDS_UPT_PROCESS, NULL );

	stat( cdqFName, &cdspecs );
	stat( vfileFName, &dspecs );

	cdqFile.open( cdqFName, ios::binary );
	if( !cdqFile ) {
		iface( UPD_BIGSTAT, IDS_UPT_ERROCDQ, NULL );
		return; }		// Open input file

	vFile.open( vfileFName, ios::binary );
	if( !vFile ) {
		cdqFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERROVFILE, NULL );
		return; }		// Open damaged file

	outFile.open( outputFName, ios::binary | ios::in | ios::out | ios::trunc );
	if( !outFile ) {
		cdqFile.close();
		vFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERROOUT, NULL );
		return; }		// Open output file

	FillRequestCore( cdqFile, vFile, outFile, cdspecs.st_size, dspecs.st_size, &efound, iface );

	cdqFile.close();
	vFile.close();
	outFile.close();

	if( efound )
		iface( UPD_BIGSTAT, IDS_UPT_CCDPGEN, NULL );
	  else {
		DeleteFile( outputFName );
		iface( UPD_BIGSTAT, IDS_UPT_CNOCORR, NULL );
	}

}

void ApplyPatchFLayer( char cdpFName[], char pfileFName[], KStatIface iface ) {

	struct stat	cdspecs;
	struct stat	pspecs;

	ifstream	cdpFile;
	fstream		pFile;

	iface( UPD_BIGSTAT, IDS_UPT_PROCESS, NULL );

	stat( cdpFName, &cdspecs );
	stat( pfileFName, &pspecs );

	cdpFile.open( cdpFName, ios::binary );
	if( !cdpFile ) {
		iface( UPD_BIGSTAT, IDS_UPT_ERROCDP, NULL );
		return; }		// Open input file

	pFile.open( pfileFName, ios::binary | ios::in | ios::out );
	if( !pFile ) {
		cdpFile.close();
		iface( UPD_BIGSTAT, IDS_UPT_ERROPFILE, NULL );
		return; }		// Open output file

	ApplyPatchCore( cdpFile, pFile, cdspecs.st_size, pspecs.st_size, iface );

	cdpFile.close();
	pFile.close();

	iface( UPD_BIGSTAT, IDS_UPT_COMPATCH, NULL );

}

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

#include "core.h"
#include "utils.h"
#include "iface.h"
#include "resource.h"

void MakeChecksumCore( istream &input, iostream &output, int blocksize, int datalen, KStatIface iface ) {

	char	*buffer;
	int		i;
	long	crc;

	output.write( ZC, 8 );
	output.write( CVER, 2 );
	output.write( (char *)&datalen, 4 );
	output.write( (char *)&blocksize, 4 );

	iface( UPD_PBRANGE, datalen, NULL );
	iface( UPD_PBSET, 0, NULL );
	iface( UPD_PBSHOW, NULL, NULL );

	buffer = new char[blocksize];

	for( datalen % blocksize ? i = -1 : i = 0; i < datalen / blocksize; i++ ) {

		if( i >= 0 && !( i % 32 ) )
			iface( UPD_PBSET, i * blocksize, NULL );

		input.read( buffer, blocksize );

		if( i == datalen / blocksize - 1 )	// If it's the end of the file, only make the checksum for what's left
			CreateChecksum( buffer, datalen % blocksize, &crc );
		  else
			CreateChecksum( buffer, blocksize, &crc );

		output.write( (char *)&crc, 4 );

	}

	delete [] buffer;

	iface( UPD_BIGSTAT, IDS_UPT_CVERCHECK, NULL );
	iface( UPD_PBHIDE, NULL, NULL );

	MakeOverallChecksum( output, 18 + ( datalen / blocksize + 1 ) * 4 );

}

void MakePatchCore( istream &cdti, istream &vstr, iostream &output, int cdtlen, int vstrlen, int * efound, KStatIface iface ) {

	long filesize;
	long blocksize;

	long crc;
	long cdtcrc;

	long curbs;

	int i;

	long curolen;
	long temp;

	char *buffer;

	*efound = 0;

	if( cdtlen < 22 ) {	// 22 == minimum size - 8 for signature, 2 for version, 4 for blocksize, 4 for filesize,
						// 4 for comparison checksum - who says it can't be a .cdt of a 0-size file? :)
		iface( UPD_BIGSTAT, IDS_UPT_INVCDT, NULL );
		return; }

	if( VerifyStream( cdti, cdtlen, IDS_UPT_INVCDT, CSV, iface ) )
		return;

	cdti.seekg( 10 );

	cdti.read( (char *)&filesize, 4 );
	cdti.read( (char *)&blocksize, 4 );

	if( filesize != vstrlen ) {

		AlterStr alter;

		alter.type = UPG_VFILESIZE;
		alter.varalpha = (void *)vstrlen;
		alter.varbeta = (void *)filesize;

		iface( UPD_BIGSTAT, NULL, &alter );
		return;
	}

	output.write( ZP, 8 );
	output.write( PVER, 2 );
	output.write( (char *)&filesize, 4 );
	curolen = 14;

	buffer = new char[ blocksize ];

	iface( UPD_MINISTAT, IDS_UPT_SCAN, NULL);
	iface( UPD_PBRANGE, vstrlen, NULL );
	iface( UPD_PBSET, 0, NULL );
	iface( UPD_PBSHOW, NULL, NULL );

	for( vstrlen % blocksize ? i = -1 : i = 0; i < vstrlen / blocksize; i++ ) {

		if( i >= 0 && !( i % 32 ) )
			iface( UPD_PBSET, i * blocksize, NULL );

		if( i == vstrlen / blocksize - 1 )
			curbs = vstrlen % blocksize;
		  else
			curbs = blocksize;

		if( curbs == 0 )
			curbs = blocksize;		// it's possible - if the file is a multiple of blocksize

		vstr.read( buffer, curbs );
		cdti.read( (char *)&cdtcrc, 4 );

		CreateChecksum( buffer, curbs, &crc );

		if( crc != cdtcrc ) {

			AlterStr alter;

			alter.type = UPG_CORR;
			alter.varalpha = (void *)(temp = i * blocksize + blocksize);

			iface( UPD_MINISTAT, NULL, &alter );

			output.write( (char *)&temp, 4 );	// write the current offset
			output.write( (char *)&curbs, 4 );
			output.write( buffer, curbs );

			curolen += 8 + curbs;

			*efound = 1; 
		
		}

	}

	delete [] buffer;

	if( *efound ) {

		temp = -1;
		output.write( (char *)&temp, 4 );			// end flag

		iface( UPD_BIGSTAT, IDS_UPT_CVERCHECK, NULL );
		iface( UPD_PBHIDE, NULL, NULL );

		curolen += 4;

		MakeOverallChecksum( output, curolen );

	}

}

void MakeRequestCore( istream &cdti, istream &dstr, iostream &output, int cdtlen, int dstrlen, int * efound, KStatIface iface ) {

	long filesize;
	long blocksize;

	long crc;
	long cdtcrc;

	long curbs;

	int i;

	long curolen;
	long temp;

	char *buffer;

	*efound = 0;

	if( cdtlen < 22 ) {	// 22 == minimum size - 8 for signature, 2 for version, 4 for blocksize, 4 for filesize,
						// 4 for comparison checksum - who says it can't be a .cdt of a 0-size file? :)
		iface( UPD_BIGSTAT, IDS_UPT_INVCDT, NULL );
		return; }

	if( VerifyStream( cdti, cdtlen, IDS_UPT_INVCDT, CSV, iface ) )
		return;

	cdti.seekg( 10 );

	cdti.read( (char *)&filesize, 4 );
	cdti.read( (char *)&blocksize, 4 );

	if( filesize != dstrlen ) {

		AlterStr alter;

		alter.type = UPG_VFILESIZE;
		alter.varalpha = (void *)dstrlen;
		alter.varbeta = (void *)filesize;

		iface( UPD_BIGSTAT, NULL, &alter );
		return;
	}

	output.write( ZQ, 8 );
	output.write( QVER, 2 );
	output.write( (char *)&filesize, 4 );
	output.write( (char *)&blocksize, 4 );
	curolen = 18;

	buffer = new char[ blocksize ];

	iface( UPD_MINISTAT, IDS_UPT_SCAN, NULL);
	iface( UPD_PBRANGE, dstrlen, NULL );
	iface( UPD_PBSET, 0, NULL );
	iface( UPD_PBSHOW, NULL, NULL );
	
	for( dstrlen % blocksize ? i = -1 : i = 0; i < dstrlen / blocksize; i++ ) {

		if( i >= 0 && !( i % 32 ) )
			iface( UPD_PBSET, i * blocksize, NULL );

		if( i == dstrlen / blocksize - 1 )
			curbs = dstrlen % blocksize;
		  else
			curbs = blocksize;

		if( curbs == 0 )
			curbs = blocksize;		// it's possible - if the file is a multiple of blocksize

		dstr.read( buffer, curbs );
		cdti.read( (char *)&cdtcrc, 4 );

		CreateChecksum( buffer, curbs, &crc );

		if( crc != cdtcrc ) {

			AlterStr alter;

			alter.type = UPG_CORR;
			alter.varalpha = (void *)(temp = i * blocksize + blocksize);

			iface( UPD_MINISTAT, NULL, &alter );

			output.write( (char *)&temp, 4 );	// write the current offset

			curolen += 4;

			*efound = 1;
		
		}
		
	}

	delete [] buffer;

	if( *efound ) {

		temp = -1;
		output.write( (char *)&temp, 4 );			// end flag

		iface( UPD_BIGSTAT, IDS_UPT_CVERCHECK, NULL );
		iface( UPD_PBHIDE, NULL, NULL );

		curolen += 4;

		MakeOverallChecksum( output, curolen );

	}

}

void FillRequestCore( istream &cdqi, istream &vstr, iostream &output, int cdqlen, int vstrlen, int *efound, KStatIface iface ) {

	long filesize;
	long blocksize;

	long curbs;

	long curolen;
	long offset;

	char *buffer;

	*efound = 0;

	if( cdqlen < 22 ) {	// 22 == minimum size - 8 for signature, 2 for version, 4 for blocksize, 4 for filesize,
						// 4 for comparison checksum
		iface( UPD_BIGSTAT, IDS_UPT_INVCDQ, NULL );
		return; }

	if( VerifyStream( cdqi, cdqlen, IDS_UPT_INVCDQ, QSV, iface ) )
		return;

	cdqi.seekg( 10 );

	cdqi.read( (char *)&filesize, 4 );
	cdqi.read( (char *)&blocksize, 4 );

	if( filesize != vstrlen ) {

		AlterStr alter;

		alter.type = UPG_VFILESIZE;
		alter.varalpha = (void *)vstrlen;
		alter.varbeta = (void *)filesize;

		iface( UPD_BIGSTAT, NULL, &alter );
		return;
	}

	output.write( ZP, 8 );
	output.write( PVER, 2 );
	output.write( (char *)&filesize, 4 );
	curolen = 14;

	buffer = new char[ blocksize ];

	cdqi.read( (char *)&offset, 4 );

	while( offset != -1 ) {

		if( offset + blocksize > vstrlen )			// if it's the end of the file . . .
			curbs = vstrlen % blocksize;				// then the block will be teensy
		  else
			curbs = blocksize;								// otherwise it's normal :)

		vstr.seekg( offset );
		vstr.read( buffer, curbs );

		output.write( (char *)&offset, 4 );	// write the current offset
		output.write( (char *)&curbs, 4 );
		output.write( buffer, curbs );

		curolen += 8 + curbs;

		cdqi.read( (char *)&offset, 4 );

		*efound = 1;
		
	}

	delete [] buffer;

	if( *efound ) {

		offset = -1;
		output.write( (char *)&offset, 4 );			// end flag

		iface( UPD_BIGSTAT, IDS_UPT_CVERCHECK, NULL );

		curolen += 4;

		MakeOverallChecksum( output, curolen );

	}

}

void ApplyPatchCore( istream &cdpi, iostream &pstr, int cdplen, int pstrlen, KStatIface iface ) {

	long filesize;
	long blocksize;

	long curbs = 0;

	long offset;

	char *buffer;

	if( cdplen < 22 ) {	// 22 == minimum size - 8 for signature, 2 for version, 4 for blocksize, 4 for filesize,
						// 4 for comparison checksum
		iface( UPD_BIGSTAT, IDS_UPT_INVCDP, NULL );
		return; }

	if( VerifyStream( cdpi, cdplen, IDS_UPT_INVCDP, PSV, iface ) )
		return;

	cdpi.seekg( 10 );

	cdpi.read( (char *)&filesize, 4 );

	if( filesize != pstrlen ) {

		AlterStr alter;

		alter.type = UPG_PFILESIZE;
		alter.varalpha = (void *)pstrlen;
		alter.varbeta = (void *)filesize;

		iface( UPD_BIGSTAT, NULL, &alter );
		return;
	}

	cdpi.read( (char *)&offset, 4 );
	cdpi.read( (char *)&blocksize, 4 );

	while( offset != -1 ) {

		if( blocksize > curbs ) {
			if( curbs != 0 )
				delete [] buffer;
			buffer = new char[ blocksize ];
			curbs = blocksize;
		}

		cdpi.read( buffer, blocksize );

		pstr.seekp( offset );
		pstr.write( buffer, blocksize );

		cdpi.read( (char *)&offset, 4 );
		cdpi.read( (char *)&blocksize, 4 );

	}

	delete [] buffer;

}

int VerifyStream( istream &input, int datalen, int emsg, KSigver sigver, KStatIface iface ) {

	char *buffer;

	char minibuff[9];

	long crc;

	buffer = new char[10];

	input.read( buffer, 10 );	// Get the header, don't want to accidentally load a 100 meg file unless we have to :)

	strncpyn( minibuff, buffer, 8 );	// Copy the signature in
	
	if( strcmp( minibuff, sigver.signature ) ) {
		delete [] buffer;
		iface( UPD_CURSTAT, emsg, NULL );
		return 1; }

	strncpyn( minibuff, &buffer[8], 2 ); // Version number comparison

	if( strcmp( minibuff, sigver.version ) ) {
		AlterStr alter;

		delete [] buffer;

		alter.type = UPG_VERSION;
		alter.varalpha = minibuff;
		alter.varbeta = sigver.version;

		iface( UPD_CURSTAT, NULL, &alter );

		return 1; }

	delete [] buffer;

	buffer = new char[ datalen ];		// want to load the whole thing

	input.seekg( 0 );						// go to the beginning
	input.read( buffer, datalen );		// and pull it all

	CreateChecksum( buffer, datalen - 4, &crc );

	if( crc != *(int *)&buffer[ datalen - 4 ] ) {
		delete [] buffer;
		iface( UPD_CURSTAT, emsg, NULL );
		return 1; }

	delete [] buffer;

	return 0;

}


void MakeOverallChecksum( iostream &st, long size ) {

	char *buffer;
	long crc;

	buffer = new char[size];

	st.flush();
	st.seekg(0);

	st.read( buffer, size );

	CreateChecksum( buffer, size, &crc );

	st.write( (char *)&crc, 4 );

	delete [] buffer;

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

/*#define DO1( buffer ) tempcrc = (tempcrc >> 8) ^ crc_table[(tempcrc ^ *buffer) & 0xff]; \
					  *crc = (tempcrc >> 8) ^ crc_table[(*crc ^ *(buffer++)) & 0xff];

#define DO2( buffer )  DO1( buffer ); DO1( buffer );
#define DO4( buffer )  DO2( buffer ); DO2( buffer );
#define DO8( buffer )  DO4( buffer ); DO4( buffer );
#define DO16( buffer )  DO8( buffer ); DO8( buffer );

void CreateChecksum( char *buffer, long size, unsigned long *crc ) {

	*crc = 0;

    *crc ^= ~0;

	unsigned long tempcrc = ~0;

    while ( size >= 16 )
    {
      DO16( buffer );
      size -= 16;
    }

    while ( size ) {
      DO1( buffer );
	  size--;
    } 

    *crc ^= ~0;
}
      
*//*void CreateChecksum( char *buffer, long size, signed long *crc ) {
	CreateChecksum( buffer, size, (unsigned long *)crc ); }

void CreateChecksum( char *buffer, long size, unsigned long *main_val )
{

	unsigned long         crc = ~0, crc32_total = ~0;
	
    for ( ; size--; ++buffer) {
      crc = (crc >> 8) ^ crc_table[(crc ^ *buffer) & 0xff];
      crc32_total = (crc >> 8) ^ crc_table[(crc32_total ^ *buffer) & 0xff];
    }

  *main_val = ~crc;
}
*/
//#define LINICKS			1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef _MAX_PATH
#define _MAX_PATH 255
#endif

#define NAMETOP 64

#define FALSE	0
#define TRUE	1

#define ZORB			0x42526f5a
#define CRCC			0x43435243

#define ZC				"ZoRBCRCC"
#define ZP				"ZoRBCRCP"
#define CVER_MAJ		2
#define CVER_MIN		0
#define PVER_MAJ		2
#define PVER_MIN		0
/*#define RVER_MAJ		2
#define RVER_MIN		0*/

#define PROGRESSJUMP	1

#ifdef LINICKS
	#define SOLIDBLOCK	  '-'
	#define EMPTYBLOCK	  '.'
  #else
	#define SOLIDBLOCK	  178
	#define EMPTYBLOCK	  176
#endif

int count;

const long crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

void DumpFile( char file[], int blocksize);
void MakePatch( char file[], char crc[] );
void PatchFile( char file[], char crc[] );
//void MakeReq( char file[], char crc[] );
//void FillReq( char file[], char crc[] );

void CreateChecksum( char *buffer, long size, long *crc );
void MakeProgressIndicator( int total, int current );

void ShutdownSig( void );

void ShutdownSig( void ) {

	printf("This program is (c)1999 by Ben Wilhelm, kryten@halcyon.com.\n");
	printf("File version 2.0\n");
#ifdef LINICKS
	printf("Program version 2.3, Linux\n\n");
#else
	printf("Program version 2.3, Wintel9x\n\n");
#endif

	return;
}

void main( int argc, char *argv[] )
{
	int blocksize;

	char file[_MAX_PATH];
	char crc[_MAX_PATH];
	int  choice;

	FILE		*dump = NULL;

	if ( argc != 1 && argc != 4 ) {
		printf("\nInvalid commandline options, no help yet - argc == %i.\n", argc);
		return;
	}

	if ( argc == 4 ) {
		
		switch ( argv[1][0] ) {

		case '1':
			sscanf( argv[3], "%i", &blocksize );
			if ( blocksize < 1 ) {
				printf("\nInvalid blocksize\n");
				return;
			}
			DumpFile( argv[2], blocksize );
			ShutdownSig();
			return;

		case '2':
			MakePatch( argv[3], argv[2] );
			ShutdownSig();
			return;

		case '3':
			PatchFile( argv[3], argv[2] );
			ShutdownSig();
			return;

		default:
			printf("\nInvalid commandline options, no help yet - argv[1][0] == %c.\n", argv[1][0]);
			return;

		}

	}

	while ( 1 ) {

		printf("Mode? Options:\n");
		printf("1: Create checksum from file\n");
		printf("2: Make patch from checksum and verified file\n");
		printf("3: Patch file\n");
		printf("Mode: ");
		scanf("\n%i", &choice);
		switch ( choice ) {

		case 1:
			printf("File to profile? ");
			scanf("\n%[^\n]", file);
			printf("Block size (16384 standard)? ");
			scanf("\n%i", &blocksize);
			DumpFile( file, blocksize );
			printf("\rComplete.                                                                      \n\n");
			ShutdownSig();
			return;

		case 2:
			printf("Checksum file? ");
			scanf("\n%[^\n]", crc);
			printf("Verified file? ");
			scanf("\n%[^\n]", file);
			MakePatch(file, crc);
			printf("\rComplete.                                                                      \n\n");
			ShutdownSig();
			return;

		case 3:
			printf("Patch data file? ");
			scanf("\n%[^\n]", crc);
			printf("File to patch? ");
			scanf("\n%[^\n]", file);
			PatchFile(file, crc);
			ShutdownSig();
			return;
		
		default:
			printf("Dude, that's not an actual option. Try again.\n\n");
			break;
		}

	}

}


void DumpFile( char file[], int blocksize )
{
	
	struct stat			filespecs;
	long				read;
	long				progress = 0;
	char				crcname[_MAX_PATH];
	char				*buffer;
	long				cursize = 18;
	long				curstat = 0;
	long				crc;
	long				mfilesize;

	FILE				*infile;
	FILE				*outfile;

	buffer = (char *)calloc(blocksize, sizeof(char));

	if( stat( file, &filespecs ) ) {
		printf("File not found, exiting.\n\n");
		return;
	}

	mfilesize = filespecs.st_size;

	strcpy( crcname, file );
	strcat( crcname, ".cdt" );

	infile = fopen( file, "rb" );
	outfile = fopen( crcname, "w+b" );

	printf("\nFile opened, %i bytes.\n", mfilesize);

	fwrite( ZC, 1, 8, outfile );
	fputc( CVER_MAJ, outfile );
	fputc( CVER_MIN, outfile );
	fwrite( &mfilesize, 4, 1, outfile);
	fwrite( &blocksize, 4, 1, outfile);

	read = blocksize;

	while( read == blocksize ) {

		read = fread( buffer, 1, blocksize, infile );

		CreateChecksum( buffer, read, &crc );

		fwrite( &crc, 4, 1, outfile );

		cursize += 4;

		curstat += read;

#ifdef LINICKS
		if ( curstat % ( blocksize * 256 ) == 0 ) {
			MakeProgressIndicator( mfilesize, curstat );
			printf("\n"); }
#else
		if ( curstat % ( blocksize * 64 ) == 0 )
			MakeProgressIndicator( mfilesize, curstat );
#endif

	}

	fclose(infile);

	free(buffer);

	buffer = (char *)calloc(cursize, sizeof(char));

	fflush(outfile);

	rewind(outfile);

	read = fread( buffer, 1, cursize, outfile );

	CreateChecksum( buffer, read, &crc );

	fseek( outfile, 0, SEEK_END );

	fwrite( &crc, 4, 1, outfile );

	fclose(outfile);

	free(buffer);
}

void PatchFile( char file[], char patch[] )
{
	
	struct stat			filespecs;
	long				progress = 0;
	char				*buffer;
	char				sig[9];
	int 				ver_maj;
	int					ver_min;
	long				filesize;
	long				crcsize;
	long				offset = 0;
	long				chunksize;
	long				crc;
	long				checkcrc;
	long				mfilesize;

	FILE				*crcfile;
	FILE				*outfile;

	if( stat( patch, &filespecs ) ) {
		printf("File not found, exiting.\n\n");
		return;
	}

	crcsize = filespecs.st_size;
	
	if( stat( file, &filespecs ) ) {
		printf("File not found, exiting.\n\n");
		return;
	}

	mfilesize = filespecs.st_size;

	crcfile = fopen( patch, "rb" );
	outfile = fopen( file, "r+b" );

	fread( sig, 1, 8, crcfile );
	ver_maj = fgetc( crcfile );
	ver_min = fgetc( crcfile );
	fread( &filesize, 4, 1, crcfile);

	sig[8]='\0';

	if ( strcmp( sig, ZP ) ) {
		printf("Invalid file signature - may not be a valid .cdp file!");
		return;
	}

	if ( !( ver_maj == PVER_MAJ && ver_min == PVER_MIN ) ) {
		printf("Incorrect file version: %i.%i. This program handles file version %i.%i only.\n\n", ver_maj, ver_min, PVER_MAJ, PVER_MIN);
		return;
	}

	if ( !(filesize == mfilesize) ) {
		printf("Error - file sizes do not match.");
		return;
	}

	printf("Verifying checksum file . . . ");

	rewind( crcfile );

	buffer = (char *)calloc( crcsize - 4, sizeof(char));

	fread( buffer, 1, crcsize - 4, crcfile );

	fread( &checkcrc, 4, 1, crcfile );

	CreateChecksum( buffer, crcsize - 4, &crc );

	if ( crc != checkcrc ) {
		printf("Patch file corrupted! Aborting.\n");
		return;
	}
	
	printf("File verified.\n\n");

	rewind( crcfile );

	fread( buffer, 1, 14, crcfile );

	free( buffer );

	fread( &offset, 4, 1, crcfile );

	while( offset != 0xffffffff ) {

		fread( &chunksize, 4, 1, crcfile );

		buffer = (char *)calloc( chunksize, sizeof(char));

		if ( (signed)fread( buffer, 1, chunksize, crcfile ) != chunksize ) {
			printf("Error, abnormal file termination! chunk == %i, offset == %i\n", chunksize, offset);
			return;
		}

		fseek( outfile, offset, SEEK_SET );

		fwrite( buffer, 1, chunksize, outfile );

		free(buffer);

		printf("Patched %i byte block at offset %i.\n", chunksize, offset);

		fread( &offset, 4, 1, crcfile );

	}

	fclose(outfile);
	fclose(crcfile);

}

void MakePatch( char file[], char crcfname[] )
{
	
	struct stat			filespecs;
	long				read;
	long				progress = 0;
	char				patname[_MAX_PATH];
	char				*buffer;
	char				sig[9];
	int					ver_maj;
	int					ver_min;
	long				blocksize;
	long				filesize;
	long				crcsize;
	long				offset = 0;
	long				cursize;
	long				crc;
	long				checkcrc;
	long				mfilesize;

	FILE				*infile;
	FILE				*crcfile;
	FILE				*outfile;

	if( stat( crcfname, &filespecs ) ) {
		printf("File not found, exiting.\n\n");
		return;
	}

	crcsize = filespecs.st_size;
	
	if( stat( file, &filespecs ) ) {
		printf("File not found, exiting.\n\n");
		return;
	}

	mfilesize = filespecs.st_size;

	strcpy( patname, file );
	strcat( patname, ".cdp" );

	infile = fopen( file, "rb" );
	crcfile = fopen( crcfname, "rb" );
	outfile = fopen( patname, "w+b" );

	fread( sig, 1, 8, crcfile );
	ver_maj = fgetc( crcfile );
	ver_min = fgetc( crcfile );

	fread( &filesize, 4, 1, crcfile);
	fread( &blocksize, 4, 1, crcfile);

	sig[8]='\0';

	if ( strcmp( sig, ZC ) ) {
		printf("Invalid file signature - may not be a valid .cdt file!");
		return;
	}

	if ( !( ver_maj == PVER_MAJ && ver_min == PVER_MIN ) ) {
		printf("Incorrect file version: %i.%i. This program handles file version %i.%i only.\n\n", ver_maj, ver_min, CVER_MAJ, CVER_MIN);
		return;
	}

	if ( !(filesize == mfilesize) ) {
		printf("Error - file sizes do not match.");
		return;
	}

	printf("\nVerifying checksum file . . . ");

	rewind( crcfile );

	buffer = (char *)calloc( crcsize - 4, sizeof(char));

	fread( buffer, 1, crcsize - 4, crcfile );

	fread( &checkcrc, 4, 1, crcfile );

	CreateChecksum( buffer, crcsize - 4, &crc );

	if ( crc != checkcrc ) {
		printf("CRC file corrupted! Aborting.\n");
		return;
	}
	
	printf("File verified.\n");

	printf("Block size: %i bytes.\n\n", blocksize);

	rewind( crcfile );

	fread( buffer, 1, 18, crcfile );

	free( buffer );

	fwrite( ZP, 1, 8, outfile );
	fputc( PVER_MAJ, outfile );
	fputc( PVER_MIN, outfile );
	fwrite( &mfilesize, 4, 1, outfile );

	read = blocksize;

	buffer = (char *)calloc( blocksize, sizeof(char));

	cursize = 18;

	while( read == blocksize ) {

		read = fread( buffer, 1, blocksize, infile );

		fread( &checkcrc, 4, 1, crcfile );

		CreateChecksum( buffer, read, &crc );

		if ( crc != checkcrc ) {
			printf("\rCorrupted block found at offset %i.                                            \n", offset);
//			printf("0x%08x  0x%08x  0x%08x\n0x%08x  0x%08x  0x%08x", bytetotal, bytecount, funkycsum, crctotal, crccount, crcfunky);
			fwrite( &offset, 4, 1, outfile );
			fwrite( &read, 4, 1, outfile );
			fwrite( buffer, 1, read, outfile );
			cursize += 8;
			cursize += read;
			}

		offset += read;

#ifdef LINICKS
		if ( offset % ( blocksize * 256 ) == 0 ) {
			MakeProgressIndicator( mfilesize, offset );
			printf("\n"); }
#else
		if ( offset % ( blocksize * 64 ) == 0 )
			MakeProgressIndicator( mfilesize, offset );
#endif

	}

	fputc( 0xff, outfile );
	fputc( 0xff, outfile );
	fputc( 0xff, outfile );
	fputc( 0xff, outfile );

	cursize += 4;

	free(buffer);

	fclose(infile);
	fclose(crcfile);

	buffer = (char *)calloc(cursize, sizeof(char));

	fflush(outfile);

	rewind(outfile);

	read = fread( buffer, 1, cursize, outfile );

	CreateChecksum( buffer, read, &crc );

	fseek( outfile, 0, SEEK_END );

	fwrite( &crc, 4, 1, outfile );

	fclose(outfile);

	free(buffer);

}

#define DO1( buffer ) *crc = crc_table[ ( *crc ^ *(buffer++) ) & 0xff ] ^ (*crc >> 8);
#define DO2( buffer )  DO1( buffer ); DO1( buffer );
#define DO4( buffer )  DO2( buffer ); DO2( buffer );
#define DO8( buffer )  DO4( buffer ); DO4( buffer );

void CreateChecksum( char *buffer, long size, long *crc ) {

	*crc = 0;

    *crc ^= 0xffffffffL;

    while ( size >= 8 )
    {
      DO8( buffer );
      size -= 8;
    }

    while ( size ) {
      DO1( buffer );
	  size--;
    } 

    *crc ^= 0xffffffffL;
}


void MakeProgressIndicator( int total, int current ) {

	int		solidblocks;
	int		i;
	int		offset = 0;

	char	buff[81];

	solidblocks = (int)( ( (float)current / (float)total ) * 70 );

	for( i = 0; i < solidblocks; i++ )
		buff[offset++] = (char)SOLIDBLOCK;

	for( i = 0; i < ( 70 - solidblocks ); i++ )
		buff[offset++] = (char)EMPTYBLOCK;

	buff[offset++] = ' ';

	sprintf( &(buff[offset]), "%3i%%", (int)( ( (float)current / (float)total ) * 100 ) );

	printf("\r%s", buff );

}
#ifndef _ZIDRAV_UTILS
#define _ZIDRAV_UTILS

#include <windows.h>

#define strncpyn( dest, source, len ) strncpy( dest, source, len ); dest[len] = '\0';

int powerof2( int power );

void ActivateGenericDD( HWND hwnd );
int CALLBACK GenericDD( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

void FilenamePlacer( HWND first, HWND second, char origext[], char newext[], char fname[] );

// make the command a li'l shorter
void FilenamePlacer( HWND first, HWND second, char newext[], char fname[] );	// omit origext[] for when there isn't
void FilenamePlacer( HWND first, HWND second, char newext[] );	// omit fname and getput for the "get" function
void FilenamePlacer( HWND first, char fname[] );	// for when the only thing that has to happen is a store

#endif
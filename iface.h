#ifndef _ZIDRAV_IFACE
#define _ZIDRAV_IFACE

#include <windows.h>

int CALLBACK MakeChecksum( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK MakePatch( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK MakeRequest( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK FillRequest( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK ApplyPatch( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

#endif
#ifndef _ZIDRAV_GUI
#define _ZIDRAV_GUI

#include <windows.h>

int CALLBACK MakeChecksumGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK MakeBatchChecksumGUI(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
int CALLBACK MakePatchGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK MakeRequestGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK FillRequestGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );
int CALLBACK ApplyPatchGUI( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam );

#endif
#ifndef _ZIDRAV_GUIFACE
#define _ZIDRAV_GUIFACE

DWORD WINAPI MakeChecksumGO( LPVOID lpParameter );
void		 MakeChecksumStdGUIiface( int type, int param, void * data, void * edata );
void		 MakeChecksumLUL( HWND hwnd, BOOL lock );				// Lock/UnLock
void		 MakeChecksumPBT( HWND hwnd, BOOL pbvis );

DWORD WINAPI MakePatchGO( LPVOID lpParameter );
void		 MakePatchStdGUIiface( int type, int param, void * data, void * edata );
void		 MakePatchLUL( HWND hwnd, BOOL lock );
void		 MakePatchPBT( HWND hwnd, BOOL pbvis );

DWORD WINAPI MakeRequestGO( LPVOID lpParameter );
void		 MakeRequestStdGUIiface( int type, int param, void * data, void * edata );
void		 MakeRequestLUL( HWND hwnd, BOOL lock );
void		 MakeRequestPBT( HWND hwnd, BOOL pbvis );

DWORD WINAPI FillRequestGO( LPVOID lpParameter );
void		 FillRequestStdGUIiface( int type, int param, void * data, void * edata );
void		 FillRequestLUL( HWND hwnd, BOOL lock );

DWORD WINAPI ApplyPatchGO( LPVOID lpParameter );
void		 ApplyPatchStdGUIiface( int type, int param, void * data, void * edata );
void		 ApplyPatchLUL( HWND hwnd, BOOL lock );

#endif
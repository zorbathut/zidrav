#ifndef _ZIDRAV_FLAYER
#define _ZIDRAV_FLAYER

#include "iface.h"

void MakeChecksumFLayer( char inputFName[], char outputFName[], int blocksize, KStatIface iface );
void MakePatchFLayer( char cdtFName[], char vfileFName[], char outputFName[], KStatIface iface );
void MakeRequestFLayer( char cdtFName[], char pfileFName[], char outputFName[], KStatIface iface );
void FillRequestFLayer( char cdqFName[], char vfileFName[], char outputFName[], KStatIface iface );
void ApplyPatchFLayer( char cdpFName[], char pfileFName[], KStatIface iface );

#endif
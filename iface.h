#ifndef _ZIDRAV_IFACE
#define _ZIDRAV_IFACE

class KStatIface {
public:
	void (*func)( int type, int param, void * data, void * edata );
	void * edata;

	KStatIface( void (*ifunc)( int, int, void *, void * ), void * iedata ) {
		func = ifunc;
		edata = iedata;
	};

	void operator()( int type, int param, void * data ) { func( type, param, data, edata ); };

};

class AlterStr {
public:
	int type;
	void * varalpha;
	void * varbeta;

};

enum {	UPD_NOACTION,

		UPD_CURSTAT,
		UPD_MINISTAT,
		UPD_BIGSTAT,
		UPD_UPPERSTAT,
		UPD_LOWERSTAT,

		UPD_PBRANGE,
		UPD_PBSET,

		UPD_PBSHOW,
		UPD_PBHIDE, };

enum {	UPG_NONE,
		UPG_VFILESIZE,
		UPG_DFILESIZE,
		UPG_PFILESIZE,
		UPG_VERSION,
		UPG_CORR,		
		UPG_FILENAME	};

void CompileString( int param, void * data, char buffer[], int length );

#endif
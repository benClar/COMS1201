#define ERROR(PHRASE) { fprintf(stderr,	\
				"Fatal Error %s occured in %s, line %d\n",	\
				PHRASE, __FILE__, __LINE__); exit(2); }

#define R_BRACE	"{"
#define L_BRACE	"}"


#define ERROR(PHRASE) { fprintf(stderr,	\
				"Fatal Error %s occured in %s, line %d\n",	\
				PHRASE, __FILE__, __LINE__); exit(2); }

#define R_BRACE	"{"
#define L_BRACE	"}"

#define FORWARD "FD"
#define R_TURN "RT"
#define L_TURN "LT"

#define SET "SET"
#define DO "DO"
#define FROM "FROM"
#define TO "TO"

#define ZERO 	"0"

#define EQUALS	":="
#define ADD 	"+"
#define SUB		"-"
#define MULT	"*"
#define DIV		"/"
#define END_POLISH	";"


#ifndef __COLOURMOD__
#define __COLOURMOD__

#include <stdio.h> /* for prints */

#define PRINT(ADD_ON, COLOUR) (printf("\033[%d;%dm", ADD_ON, COLOUR))
#define RESET(X) (printf("\033[%d;m", X))

/* use example:

#include "colourmod.h"

int main (void)
{
	PRINT(BOLD, FG_RED);
	printf ("HELLO IN BOLD RED\n");
	RESET(0);
	printf ("HELLO RESET\n");
}

*/

typedef enum background
{
	BG_BLACK = 40,
	BG_RED = 41,
	BG_GREEN = 42,
	BG_YELLOW = 43,
	BG_BLUE = 44,
	BG_MAGENTA = 45,
	BG_CYAN = 46,
	BG_WHITE = 47
} background_t;

typedef enum foreground
{
	FG_BLACK = 30,
	FG_RED = 31,
	FG_GREEN = 32,
	FG_YELLOW = 33,
	FG_BLUE = 34,
	FG_MAGENTA = 35,
	FG_CYAN = 36,
	FG_WHITE = 37
} foreground_t;

typedef enum add_ons
{
	RESET = 0, /* back to normal */
	BOLD = 1,  /* could also use as a brighter shade */
	UNDERLINE = 4,
	INVERSE = 7, /* swap foreground and background colours */
	BOLD_OFF = 21,
	UNDERLINE_OFF = 24,
	INVERSE_OFF = 27
} add_ons_t;

#endif /* __COLOURMOD__ */
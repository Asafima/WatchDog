#include <stdio.h> /* printf */

#include "watchdog.h" /* watchdog funcs */

int main(int argc, char *argv[])
{
    is_wd = WD;
    WDStart(argv);
    (void)argc;
    return 0;
}

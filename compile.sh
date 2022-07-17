#!/bin/bash

gcc -ansi -I include -pedantic-errors -Wall -Wextra -g source/watchdog.c source/watchdog_main.c -ldsdebug -fPIC -L. -Wl,-rpath="\$ORIGIN" -lpthread -o watchdog.out

gcc -ansi -I include -pedantic-errors -Wall -Wextra -g source/watchdog.c test/watchdog_test.c -ldsdebug -fPIC -L. -Wl,-rpath="\$ORIGIN" -lpthread -o user_app.out

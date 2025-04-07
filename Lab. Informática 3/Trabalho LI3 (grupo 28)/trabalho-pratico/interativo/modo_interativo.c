#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>

#include "../include/menu.h"

int interativo()
{
    bool running = true;
    int selecao = menu();

        while (running)
        {
            int finish = 0;
            if(selecao == 0)
            {
                path();
                endwin();
                break;;

            }
            else if(selecao == 1)
            {
                 finish = 1;
                 running = false;
                 break;
            }
            
            if(finish == 1)
            {
                endwin();
            }
        }
    return 1;
}


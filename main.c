#include <locale.h>
#include <ncurses.h> //biblioteka z ktorej bedziemy korzystac
#include <time.h>
#include <stdio.h>
#include <wchar.h>

int plansza(){
    printw("╔═════════════════════════╦═╦═════════════════════════╗\n");
    printw("║                         ║ ║                         ║\n");
    printw("║   ╔═════╗   ╔═══════╗   ║ ║   ╔═══════╗   ╔═════╗   ║\n");
    printw("║   ║     ║   ║       ║   ║ ║   ║       ║   ║     ║   ║\n");
    printw("║   ╚═════╝   ╚═══════╝   ╚═╝   ╚═══════╝   ╚═════╝   ║\n");
    printw("║                                                     ║\n");
    printw("║   ╔═════╗   ╔═╗   ╔═════════════╗   ╔═╗   ╔═════╗   ║\n");
    printw("║   ╚═════╝   ║ ║   ╚═════╗ ╔═════╝   ║ ║   ╚═════╝   ║\n");
    printw("║             ║ ║         ║ ║         ║ ║             ║\n");
    printw("╚═════════╗   ║ ╚═════╗   ║ ║   ╔═════╝ ║   ╔═════════╝\n");
    printw("          ║   ║ ╔═════╝   ╚═╝   ╚═════╗ ║   ║\n");
    printw("          ║   ║ ║                     ║ ║   ║\n");
    printw("          ║   ║ ║   ╔════ --- ════╗   ║ ║   ║\n");
    printw("══════════╝   ╚═╝   ║             ║   ╚═╝   ╚══════════\n");
    printw("                    ║             ║\n");
    printw("══════════╗   ╔═╗   ║             ║   ╔═╗   ╔══════════\n");
    printw("          ║   ║ ║   ╚═════════════╝   ║ ║   ║\n");
    printw("          ║   ║ ║                     ║ ║   ║\n");
    printw("          ║   ║ ║   ╔═════════════╗   ║ ║   ║\n");
    printw("╔═════════╝   ╚═╝   ╚═════╗ ╔═════╝   ╚═╝   ╚═════════╗\n");
    printw("║                         ║ ║                         ║\n");
    printw("║   ╔═════╗   ╔═══════╗   ║ ║   ╔═══════╗   ╔═════╗   ║\n");
    printw("║   ╚═══╗ ║   ╚═══════╝   ╚═╝   ╚═══════╝   ║ ╔═══╝   ║\n");
    printw("║       ║ ║                                 ║ ║       ║\n");
    printw("╠═══╗   ║ ║   ╔═╗   ╔═════════════╗   ╔═╗   ║ ║   ╔═══╣\n");
    printw("╠═══╝   ╚═╝   ║ ║   ╚═════╗ ╔═════╝   ║ ║   ╚═╝   ╚═══╣\n");
    printw("║             ║ ║         ║ ║         ║ ║             ║\n");
    printw("║   ╔═════════╝ ╚═════╗   ║ ║   ╔═════╝ ╚═════════╗   ║\n");
    printw("║   ╚═════════════════╝   ╚═╝   ╚═════════════════╝   ║\n");
    printw("║                                                     ║\n");
    printw("╚═════════════════════════════════════════════════════╝\n");
    return 0;
}

int main()
{
    /* uruchomienie ncurses */
    setlocale(LC_ALL, "");

    initscr(); //synchornizuj terminal
    raw(); //synchornizuj klawiature
    noecho(); //synchornizuj wypisywanie
    //keypad(stdscr, TRUE); //dodaj obsluge klawiszy specjalnych


    int height=40;
    int width=60;
    int start_x = 0;
    int start_y=0;
    WINDOW * win = newwin(height, width, start_x, start_y);
    keypad(win, true);

    plansza();
    wrefresh(win);
    


    getch(); //wait for user's input (return int of char)
    refresh(); //refresh screen to match the memory
    


    endwin(); //zamknij ncurses  
    return 0;
}
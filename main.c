#include<ncurses.h> //biblioteka z ktorej bedziemy korzystac
#include<time.h>

int main()
{
        /* uruchomienie ncurses */
        initscr(); //synchornizuj terminal
        raw(); //synchornizuj klawiature
        noecho(); //synchornizuj wypisywanie
        keypad(stdscr, TRUE); //dodaj obsluge klawiszy specjalnych


        endwin(); //zamknij ncurses
        return 0;
}
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
    int x = 5;
    int y= 10;
    WINDOW * win = newwin(height, width, start_x, start_y);
    keypad(win, true);

    start_color();
    init_pair(1,COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    plansza();
    attroff(COLOR_PAIR(1));
    wrefresh(win);

    
    wmove(win,y,x);
    int ch;
    refresh();
    while((ch = wgetch(win)) != 'q')  //jako 'q' możemy dać wejście do menu
    {
        switch (ch)
        {
          case KEY_LEFT:
            x--;
            break;
          case KEY_RIGHT:
            x++;
            break;
          case KEY_UP:
            y--;
            break;
          case KEY_DOWN:
            y++;
            break;
          default:
            beep();
            break;
        }
        wprintw(win, " ");
        wmove(win,y,x);
        wprintw(win, "O"); //ᗣ
        wmove(win,y,x);
        if(x<=0 || x>width || y<=0 || y>=height){
            break;
        }
        refresh(); //refresh screen to match the memory
    }

//ඞ
    endwin(); //zamknij ncurses  
    return 0;
}

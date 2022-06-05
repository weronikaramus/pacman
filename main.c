#include <locale.h>
#include <ncurses.h> //biblioteka z ktorej bedziemy korzystac
#include <time.h>
#include <stdio.h>
#include <wchar.h>
    char map[55][31];
    int mapC[55][31] = { 0 };

char* blockTypes[] = {"-",  "║", "═", "╚", "╗", "╝", "╔", "╣", "╠", "╦"};

double desiredFPS = 6.0;

enum directions {
  dirNone,
  dirLeft,
  dirTop,
  dirRight,
  dirBottom
};

int plansza(){
    start_color();
    init_pair(1,COLOR_BLUE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    for(size_t j = 0; j < 31; j++) {
      for(size_t i = 0; i < 55; i++) {
        if(map[i][j] >= '0' && map[i][j] <= '9') {
          printw("%s", blockTypes[(int)map[i][j] - 48]);
        } else {
          printw(" ");
        }
      }
      printw("\n");
    }
    attroff(COLOR_PAIR(1));
    return 0;
}

int main()
{

    FILE* mapFile;

    if( ( mapFile = fopen("mapa.txt", "r")) == NULL) {
      fprintf(stderr, "plik jest pusty\n");
      return 1;
    }
    for(size_t i = 0; i < 55; i++) {
      for(size_t j = 0; j < 31; j++) {
        map[i][j] = ' ';
      }
    }


    size_t readX = 0;
    size_t readY = 0;

    char c = getc(mapFile);

    while( c != EOF ) {
      if(c == '\n') {
        readX = 0;
        readY++;
      } else {  
        if((c >= '1' && c <= '9') || c == 'X') {
          mapC[readX][readY] = 1;
        }
        map[readX][readY] = c; 
        readX++;
      }
      c = getc(mapFile);
    }


  /* uruchomienie ncurses */
    setlocale(LC_ALL, "");

    initscr(); //synchornizuj terminal
    raw(); //synchornizuj klawiature
    noecho(); //synchornizuj wypisywanie
    //keypad(stdscr, TRUE); //dodaj obsluge klawiszy specjalnych


    int height=31;
    int width=55;
    int start_x = 0;
    int start_y=0;
    int x = 27;
    int y= 23;
    WINDOW * win = newwin(height, width, start_x, start_y);
    keypad(win, true);
    nodelay(win, 1);


    
    wmove(win,y,x);
    int ch;
    refresh();
    int currentDir = dirNone;




    while(true)  //jako 'q' możemy dać wejście do menu
    {
    clock_t prevTime = clock();

        ch = wgetch(win);
        if(ch == 'q') {
          break;
        }
        switch (ch)
        {
          case KEY_LEFT:
            currentDir = dirLeft;
            break;
          case KEY_RIGHT:
            currentDir = dirRight;
            break;
          case KEY_UP:
            currentDir = dirTop;
            break;
          case KEY_DOWN:
            currentDir = dirBottom;
            break;
          default:
            break;
        }

        /* MOVE LOGIC */

        // variables for attempted moves

        int attemptedX = 0;
        int attemptedY = 0;

        // Check what direction we're supposed to be moving in

        switch(currentDir) {
          case dirLeft:
            attemptedX = -1;
            break;
          case dirRight:
            attemptedX = 1;
            break;
          case dirTop:
            attemptedY = -1;
            break;
          case dirBottom:
            attemptedY = 1;
            break;
          //do nothign if not set, it's likely the default dirNone
        }

        /* Check if we can move into the spaces dictated by our tried X & Y */
        if(mapC[x + attemptedX][y + attemptedY] != 0) {
          /* can't move! abort movement */
          currentDir = dirNone;
        } else {
          /* MOVE */
          x += attemptedX;
          y += attemptedY;
        }

        /* BEGIN DRAWING */
        move(0, 0);
        plansza();
        wrefresh(win);
        mvwprintw(win, y, x, "●"); //ᗣ
        wmove(win,y,x);
        if(x<=0 || x>=width || y<=0 || y>=height){
          if((x <= 0 || x >= width) && y == 14) {
            x = 55 - x; //loop
          } else {
            break;
          }
        }

        /* Hold the frame until we reach set amount of FPS */
        while(true) {
          clock_t curTime = clock();
          if( ( (double)curTime-(double)prevTime)/CLOCKS_PER_SEC > (1.0/desiredFPS))
            break;
        }
        refresh(); //refresh screen to match the memory
    }

//ඞ
    endwin(); //zamknij ncurses  
    return 0;
}

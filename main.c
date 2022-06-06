#include <locale.h>
#include <ncurses.h> //biblioteka z ktorej bedziemy korzystac
#include <time.h>
#include <stdio.h>
#include <wchar.h>

    char map[55][31];
    int mapC[55][31] = { 0 };

char* blockTypes[] = {"-",  "║", "═", "╚", "╗", "╝", "╔", "╣", "╠", "╦"};

double desiredFPS = 6.0; //jeżeli gra będzie za wolna to możemy sobie zwiększyć

enum directions {
  dirNone,
  dirLeft,
  dirTop,
  dirRight,
  dirBottom
};

enum gameState {
  stateMenu,
  statePlaying,
  stateHighScores,
  stateChangeName,
  stateQuit
};

int currentGameState = stateMenu;

bool isPlaying = true;
    int highscore=0;
    char username[]="anon";

WINDOW * win;
    int height=31;
    int width=55;
    int start_x = 0;
    int start_y=0;
    int x = 27;
    int y= 23;

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

int menu()
{
    wclear(win);
//    erase();
    refresh();
    box(win, 0, 0);
//    keypad(win, true);
    refresh();
    wrefresh(win);
    mvwprintw(win, 0, 16, " * * * PAC - MAN * * * ");
    mvwprintw(win, 2, 2, "Highscore: %d", highscore);
    mvwprintw(win, 3, 2, "Player: %s", username);
    int menuy=15;
    int menux=20;
    char* options[4] = {
      "CHANGE NICKNAME ",
      "PLAY ",
      "HIGH SCORES ",
      "QUIT "
    };

    int currentChoice = 0;

    int choiceResults[] = {
      stateChangeName,
      statePlaying,
      stateHighScores,
      stateQuit
    };

    bool isPicking = true;

    int c;
    move(menuy, menux-1);
    while(isPicking){
        for(size_t i = 0; i < 4; i++) {
          if(i == currentChoice) { 
            wattron(win, A_STANDOUT);
            mvwprintw(win, menuy + i, menux, "%s", options[i]);
            wattroff(win, A_STANDOUT);
          } else
            mvwprintw(win, menuy + i, menux, "%s", options[i]);
        }

        c=wgetch(win);
        switch (c)
        {
        case KEY_UP:
            if(currentChoice == 0)
              currentChoice + (4 - 1);
            else
              currentChoice--;
            break;
        case KEY_DOWN:
            if(currentChoice == (4 - 1))
              currentChoice - (4 - 1);
            else
              currentChoice++;            
            break;
        case 10: //enter
            currentGameState = choiceResults[currentChoice];
            isPicking = false;
            break;
        default:
            break;
        }
    
        wrefresh(win);
    }

//    getch(); //wait for user's input (return int of char)
    refresh(); //refresh screen to match the memory


    endwin(); //zamknij ncurses  
    return 0;
}
      int currentDir = dirNone;

void mainGame() {
          int ch;
          ch = wgetch(win);
          if(ch == 'q') {
            currentGameState = stateMenu;
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

          /* cały movement */

          // variables for attempted moves

          int attemptedX = 0;
          int attemptedY = 0;

          // sprawdzamy w jakim kierunku się poruszamy

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
            //do nothing if not set, it's likely the default dirNone
          }

          /* sprawdzamy czy możemy się poruszać w spaces dictated by our X i Y */
          if(mapC[x + attemptedX][y + attemptedY] != 0) {
            /*are we moving into an X'd corridor?*/
            if(mapC[x + attemptedX][y + attemptedY] == 2) {
              /* check left and right sides of X for a corridor, if yes move there */
              if(mapC[x + attemptedX + 1][y + attemptedY] == 0) {
                x += attemptedX + 1;
                y += attemptedY;
              } else if(mapC[x + attemptedX - 1][y + attemptedY] == 0) {
                x += attemptedX - 1;
                y += attemptedY;
              }
            } else {
              /* halt, abort, nie wolno sie ruszać!!!!!!!!!! */
              currentDir = dirNone;
            }
          } else {
            /* MOVE */
            x += attemptedX;
            y += attemptedY;
          }

          /* rysowańsko */
          init_pair(2, COLOR_YELLOW, COLOR_BLACK);
          move(0, 0);
          plansza();
          wrefresh(win);
          attron(COLOR_PAIR(2));
          mvwprintw(win, y, x, "●"); //ᗣ?
          attroff(COLOR_PAIR(2));
          wmove(win,y,x);


          /* Portal */
          if (y==14){
            if (x<=0){
            x = 55-x;
            } else if ((x>=54) && attemptedX==1){
              x = 0;
            }
          }

}

int main()
{

    //menu();

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
        if((c >= '0' && c <= '9')) {
          mapC[readX][readY] = 1;
        } else if(c == 'X') {
          mapC[readX][readY] = 2;
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


    win = newwin(height, width, start_x, start_y);
    keypad(win, true);
    nodelay(win, 1);


    
    wmove(win,y,x);
    refresh();
          curs_set(0);




    while(isPlaying) 
    {
      clock_t prevTime = clock();  //clock_t jest z biblioteki <time.h>

      switch(currentGameState) {
        case stateMenu:
          menu();
          break;
        case statePlaying:
          mainGame();
          break;
        case stateQuit:
          isPlaying = false;
          break;
        }

        /* czekamy aż max będzie dana amount of fps*/
        while(true) {
          clock_t curTime = clock();
          if( ( (double)curTime-(double)prevTime)/CLOCKS_PER_SEC > (1.0/desiredFPS))
            break; //liczenie klatek na sekunde
        }
        refresh(); //refresh screen to match the memory
    }

//ඞ
    endwin(); //zamknij ncurses  
    return 0;
}

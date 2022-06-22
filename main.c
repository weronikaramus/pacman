#include <locale.h>
#include <ncurses.h> //biblioteka z ktorej bedziemy korzystac
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#define MENU_CHOICE_AMT 3

    char map[55][31]; //ustawiamy rozmiar planszy
    int mapC[55][31] = { 0 };  //ustawiamy rozmiar planszy z kolizjami
    char mapG[55][31];

char* blockTypes[] = {"-",  "║", "═", "╚", "╗", "╝", "╔", "╣", "╠", "╦"};

double desiredFPS = 6.0; //jeżeli gra będzie za wolna to możemy sobie zwiększyć

enum directions {  //mamy 5 przypadków ruchu, bezruch, w lewo, w górę, w prawo i w dół
  dirNone,
  dirLeft,
  dirTop,
  dirRight,
  dirBottom
};

enum gameState {  //mamy 5 stanów gry, menu, gra, highscores, zmiana nicku i wyjście
  stateMenu,
  statePlaying,
  stateHighScores,
  stateChangeName,
  stateQuit
};

int currentGameState = stateMenu;  //ustawiamy na początku stan gry na menu, żeby się wyświetlało po otwarciu

bool isPlaying = true;
    int highscore=0;
    int score = 0;
    char username[16];

WINDOW * win;        //parametry okna
    int height=31;
    int width=55;
    int start_x = 0;
    int start_y=0;
    int x = 27;
    int y= 23;

typedef struct Duszek_s {
    int x;
    int y;
    int attemptedX;
    int attemptedY;
    short kolor;
    int color_pair;
    int dir;
    int dirInit;
    int wait;
  } Duszek;

Duszek red = {
        .x = 23,
        .y = 14,
        .kolor = COLOR_RED,
        .color_pair = 4,
        .dir = dirRight,
        .dirInit = dirRight,
        .wait = 0};

Duszek green = {
        .x = 23,
        .y = 15,
        .kolor = COLOR_GREEN,
        .color_pair = 5,
        .dir = dirRight,
        .dirInit = dirRight,
        .wait = 5};

Duszek cyan = {
        .x = 26,
        .y = 15,
        .kolor = COLOR_CYAN,
        .color_pair = 6,
        .dir = dirLeft,
        .dirInit = dirLeft,
        .wait = 15};

Duszek magenta = {
        .x = 26,
        .y = 15,
        .kolor = COLOR_MAGENTA,
        .color_pair = 7,
        .dir = dirLeft,
        .dirInit = dirLeft,
        .wait = 25};

Duszek* duszki[] = {&red, &green, &cyan, &magenta};

int plansza(){
  
    start_color();
    init_pair(1,COLOR_BLUE, COLOR_BLACK);  
    init_pair(2,COLOR_WHITE, COLOR_BLACK);       //ustawiamy kolor planszy na niebieski
    
 
    for(size_t j = 0; j < 31; j++) {           //w podwójnej pętli rysujemy planszę
      for(size_t i = 0; i < 55; i++) {
        if((map[i][j] >= '0' && map[i][j] <= '9' )) {           //jeżeli na danym koordynacie będzie liczba od 0 do 9,
          attron(COLOR_PAIR(1));
          printw("%s", blockTypes[(int)map[i][j] - 48]); 
//          printw("%c", mapG[i][j]); 
          attroff(COLOR_PAIR(1));    //to wtedy drukujemy jej odpowiednik z tablicy blockTypes
        } else if(map[i][j] == 'Y'){
            attron(COLOR_PAIR(2));
            printw(".");
            attroff(COLOR_PAIR(2));
          }else if(map[i][j] == 'Z'){
            attron(COLOR_PAIR(2));
            printw("★");
            attroff(COLOR_PAIR(2));
          }else{
            attron(COLOR_PAIR(1));
            printw(" ");
            attroff(COLOR_PAIR(1));    //w przeciwnym razie drukuj whitespace
          }
      }
      printw("\n");      //po każdym wierszu new line
    }
    return 0;
}

int menu()
{
    wclear(win);
    refresh();
    box(win, 0, 0);  //rysujemy border wokół menu
    refresh();
    
    
   
    if (score>highscore){
      highscore = score;
    }
    start_color();
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 0, 16, " * * * PAC - MAN * * * ");     //wypisujemy elementy menu
    mvwprintw(win, 2, 2, "Highscore: %d", highscore);
    mvwprintw(win, 3, 2, "Player: %s", username);
    wrefresh(win);

    int menuy=(height/2)-2;    //położenie menu
    int menux=20;
    
    char* options[MENU_CHOICE_AMT] = {      //MENU_CHOICE_AMT jest #defined jako 4, przyda się to później przy
      "PLAY ",
      "CHANGE NICKNAME ",                   //przeklikiwaniu przez opcje w menu
      "QUIT "
    };

    int currentChoice = 0;

    int choiceResults[] = {
      statePlaying,
      stateChangeName,
      stateQuit
    };

    bool isPicking = true;    //zmienna oznaczająca że wybieramy coś z menu

    int c;
    move(menuy, menux-1);
    while(isPicking){       //wszystko związane z wypisywaniem wyborów jest w pętli
        for(size_t i = 0; i < MENU_CHOICE_AMT; i++) {       //wypisujemy opcje z tablicy menu options
          if(i == currentChoice) {       //jeżeli coś jest obecnie wybierane 
            wattron(win, A_STANDOUT);     //to podświetlamy
            mvwprintw(win, menuy + i, menux, "%s", options[i]);  //okno, wypisz w takim y (po kolei coraz niżej), takim x, takiego stringa z takiej tablicy
            wattroff(win, A_STANDOUT);
          } else  
            mvwprintw(win, menuy + i, menux, "%s", options[i]);  //reszta się wypisuje normalnie
        }

        c=wgetch(win);
        switch (c)
        {
        case KEY_UP:
            if(currentChoice == 0)     //jeżeli chcemy przejść w górę (KEY_UP) a jesteśmy już na najwyższym (pierwszym) stringu to przejdź na koniec, 
              currentChoice = MENU_CHOICE_AMT - 1;  //a jak nie jesteśmy jeszcze na górze to przejdź do góry o jeden
            else
              currentChoice--;
            break;
        case KEY_DOWN:
            if(currentChoice == (MENU_CHOICE_AMT - 1))  //analogicznie do key_up, sprawdza czy jesteśmy na najniższym
              currentChoice = 0;
            else
              currentChoice++;   //jeżeli nie jesteśmy na dole to przejdź na dół o jeden         
            break;

        case 10: //enter
        currentGameState = choiceResults[currentChoice];
            isPicking = false;
            break;
        default:
          break;
        }
    wattroff(win, COLOR_PAIR(2));
        wrefresh(win);
    }

//    getch(); //wait for user's input (return int of char)
    refresh(); //refresh screen to match the memory

    score=0;
    endwin(); //zamknij ncurses  
    return 0;
}
      int currentDir = dirNone;



int changeName(){
  start_color();
  
  wclear(win);
  box(win, 0, 0);
  refresh();
  cbreak();
  mvwprintw(win, 0, 16, " * * * PAC - MAN * * * ");
  mvwprintw(win, (height/2)-2, 20, "WRITE YOUR NAME:");
  
  mvwprintw(win, height-4, 4, "press enter to accept");
  mvwprintw(win, height-3, 4, "then press enter or esc to exit");
  
  move((height/2-1), 20);
  noecho();
  curs_set(1); //widoczny kursor
  for(int i=0; i<15;i++)username[i]='\0'; //usuwamy starą nazwę
  wgetstr(win, username);
  for (int i=0; i <15; i++){
    
    char c = getch(); //pobieramy znak po znaku nazwę
    if((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)){ //poprawny nick to tylko litery i cyfry
      username[i] = c;
      curs_set(1);
      printw("%c", username[i]); //drukujemy znak, jeśli jest poprawny
    }else if (c == '\n') { //enter zatwierdza nazwę
      username[i+1]='\0';
      currentGameState = stateMenu;
      break;
    } else if(c == 27) {     //escape
      currentGameState = stateMenu;
      break;
    }else if(c == 127) {     //backspace
      username[i-1]=' ';
      username[i]=' ';
      i-=2;
    }
    
    
    curs_set(0);
    
  }
  
  wrefresh(win);
  return 0;
}

void ghostMove(Duszek* g) {
          if(g->wait > 0) {
            g->wait--;
            return;
          }

          g->attemptedX = 0;
          g->attemptedY = 0;

        switch(g->dir) {          //każdy z kierunków ma swoją wartość
            case dirLeft:
              g->attemptedX = -1;
              break;
            case dirRight:
              g->attemptedX = 1;
              break;
            case dirTop:
              g->attemptedY = -1;
              break;
            case dirBottom:
              g->attemptedY = 1;
              break;
        }


      if(mapG[g->x][g->y] == 'O'){
          g->x += g->attemptedX;
          g->y += g->attemptedY; 
        
      } else if(mapG[g->x][g->y] == 'Q'){
           
        if(g->dir == dirLeft){
          g->y++;
          g->dir = dirBottom;
        } else {
          g->x++;
          g->dir = dirRight;
        }
      }else if(mapG[g->x][g->y] == 'W'){
            
        if(g->dir == dirLeft){
          g->y--;
          g->dir = dirTop;
        } else {
          g->x++;
          g->dir = dirRight;
        }

      }else if(mapG[g->x][g->y] == 'E'){
            
        if(g->dir == dirRight){
          g->y++;
          g->dir = dirBottom;
        } else {
          g->x--;
          g->dir = dirLeft;
        }
        
      }else if(mapG[g->x][g->y] == 'R'){
            
        if(g->dir == dirRight){
          g->y--;
          g->dir = dirTop;
        } else {
          g->x--;
          g->dir = dirLeft;
        }

      }else if(mapG[g->x][g->y] == 'T'){
        int dirr = rand() % 2;
        switch(g->dir) {
          case dirTop:
            if(dirr) {
              g->dir = dirLeft;
              g->x--;
            } else {
              g->dir = dirTop;
              g->y--;
            }
            break;
          case dirRight:
            if(dirr) {
              g->dir = dirTop;
              g->y--;
            } else {
              g->dir = dirBottom;
              g->y++;
            }
            break;
          case dirBottom:
            if(dirr) {
              g->dir = dirLeft;
              g->x--;
            } else {
              g->dir = dirBottom;
              g->y++;
            }
            break;
            
        }
      }else if(mapG[g->x][g->y] == 'Y'){
        int dirr = rand() % 2;
        switch(g->dir) {
          case dirTop:
            if(dirr) {
              g->dir = dirRight;
              g->x++;
            } else{
              g->dir = dirTop;
              g->y--;
            }
            break;
          case dirLeft:
            if(dirr) {
              g->dir = dirTop;
              g->y--;
            } else {
              g->dir = dirBottom;
              g->y++;
            }
            break;
          case dirBottom:
            if(dirr) {
              g->dir = dirRight;
              g->x++;
            } else {
              g->dir = dirBottom;
              g->y++;
            }
            break;
            
        }

      }else if(mapG[g->x][g->y] == 'U'){
         int dirr = rand() % 2;
        switch(g->dir) {
          case dirBottom:
            if(dirr) {
              g->dir = dirLeft;
              g->x--;
            } else {
              g->dir = dirRight;
              g->x++;
            }
            break;
          case dirLeft:
            if(dirr) {
              g->dir = dirTop;
              g->y--;
            }
            break;
          case dirRight:
            if(dirr) {
              g->dir = dirTop;
              g->y--;
            }
            break;
        }
            
       
      }else if(mapG[g->x][g->y] == 'I'){
        int dirr = rand() % 2;
        switch(g->dir) {
          case dirRight:
            if(dirr) {
              g->dir = dirBottom;
              g->y++;
            }
            break;
          case dirLeft:
            if(dirr) {
              g->dir = dirBottom;
              g->y++;
            }
            break;
          case dirTop:
            if(dirr) {
              g->dir = dirLeft;
              g->x--;
            } else {
              g->dir = dirRight;
              g->x++;
            }
            break;
        }
            

      }else if(mapG[g->x][g->y] == 'P'){
        int dirr = rand() % 3;
        switch(g->dir){
        case dirRight:
          if(dirr){
            g->dir = dirTop;
            g-> y--;
          }else {
          g->dir = dirBottom;
          g->y++;}
          break;

        case dirLeft:
          if(dirr){
            g->dir = dirTop;
            g-> y--;
          }else {
          g->dir = dirBottom;
          g->y++;}
          break;

        case dirTop:
        if(dirr){
            g->dir = dirLeft;
            g-> x++;
          }else {
          g->dir = dirRight;
          g->x--;}
          break;

        case dirBottom:
        if(dirr){
            g->dir = dirLeft;
            g-> x++;
          }else {
          g->dir = dirRight;
          g->x--;}
          break;
        }
      }else if(mapG[g->x][g->y] == 'L'){
        switch(g->dir){
        case dirLeft:
            g->dir = dirRight;
            g->x++;
            break;

        case dirRight:
            g-> dir = dirLeft;
            g->x--;
            break;
        }
      }

/* mapa literki for future reference
Q - right or down
W - right or top
E - left or down
R - left or top
T - not right
Y - not left
U - not bottom
I - not top
P - all (doesnt exist)
L - odbijanie sie od ściany
*/

          init_pair(g->color_pair, g->kolor, COLOR_BLACK); 
          wrefresh(win);
          wattron(win, COLOR_PAIR(g->color_pair));
          mvwprintw(win, g->y, g->x, "ඞ"); //ඞ?
          wattroff(win, COLOR_PAIR(g->color_pair));
          wmove(win,g->y,g->x);
          wrefresh(win);

}

void mainGame() {
          int ch;
          ch = wgetch(win);
          if(ch == 27) {     //escape
            currentGameState = stateMenu;
          }
          switch (ch)
          {
            case KEY_LEFT:          //po wciśnięciu odpowiedniego klawisza, obecny kierunek nam się zmienia
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

          int attemptedX = 0;    //zmienne pomocnicze do liczenia ruchu
          int attemptedY = 0;

          // sprawdzamy w jakim kierunku się poruszamy

          switch(currentDir) {          //każdy z kierunków ma swoją wartość
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

          /* sprawdzamy czy możemy się ruszyć w miejsca określone przez aktualny kierunek ruchu, 
          (obecne) x + attemptedX wskazuje na przyszłą zamierzoną pozycję w x, analogicznie dla y
          jeżeli kolizja w przyszłych koordynatach x i y jest różna od 0, to znaczy, że mamy do czynienia
          ze ścianą. wtedy sprawdza, czy ściana to X (niewidzialna ściana), który
          zawsze znajduje się tylko po bokach. jeżeli trafia na X, to najpierw sprawdza czy może
          iść w prawo, a jeśli nie, to sprawdza czy może iść w lewo.
          jeżeli żadna z tych opcji nie działa to zatrzymujemy movement.
          ten sposób pozwala nam na wybranie kierunku ciągłego ruchu, zamiast poruszaniu się o jedną
          kratkę wraz z każdym wciśnięciem klawisza. aktualizacja pozycji odbywa się w linijce z 
          printowaniem pacmana.
           */
           if(mapC[x + attemptedX][y + attemptedY] == 3){
                  x+= attemptedX;
                  y+= attemptedY;
                  map[x][y] = ' ';
                  mapC[x][y] = 0;
                  score += 10;
                  if(score == 2600){
                    currentGameState = stateMenu;
                  }
            } else if(mapC[x + attemptedX][y + attemptedY] == 4){
                  x+= attemptedX;
                  y+= attemptedY;
                  map[x][y] = ' ';
                  mapC[x][y] = 0;
                  score += 50;
                  if(score == 2600){
                    currentGameState = stateMenu;
                  }
            } else if(mapC[x + attemptedX][y + attemptedY] != 0) {
              if(mapC[x + attemptedX][y + attemptedY] == 2) {
                if(mapC[x + attemptedX + 1][y + attemptedY] == 0) {
                  x += attemptedX + 1;
                  y += attemptedY;
                } else if(mapC[x + attemptedX - 1][y + attemptedY] == 0) {
                  x += attemptedX - 1;
                  y += attemptedY;
                }
            } else{
              /* halt, abort, nie wolno sie ruszać!!!!!!!!!! */
              currentDir = dirNone;
            }
          } else {
            /* ruch bez żadnych kolizji */
            x += attemptedX;
            y += attemptedY;
          }

          /* rysowańsko */ 
          init_pair(3, COLOR_YELLOW, COLOR_BLACK);      //nadajemy pacmanowi żółty kolor
          move(0, 0);
          plansza();
          wrefresh(win);
          wattron(win, COLOR_PAIR(3));
          mvwprintw(win, y, x, "●"); //ඞ?
          wattroff(win, COLOR_PAIR(3));
          wmove(win,y,x);
          mvwprintw(win, 0, 2, " Score: %d ", score);


          /* Portal ඞ*/
          if (y==14){
            if (x<=0){
            x = 55-x;
            } else if ((x>=54) && attemptedX==1){
              x = 0;
            }
          }

          /*Duszki funkcjaඞ*/ 
          for(size_t i = 0; i < 4; i++) {
            ghostMove(duszki[i]);
          }

        
          if (red.y == y && red.x == x){
            currentGameState = stateMenu;
          }
          if (cyan.y == y && cyan.x == x){
            currentGameState = stateMenu;
          }
          if (green.y == y && green.x == x){
            currentGameState = stateMenu;
          }
          if (magenta.y == y && magenta.x == x){
            currentGameState = stateMenu;
          }
        

}

int main()
{

    //menu();

    srand(time(NULL));

    FILE* mapFile;

    if( ( mapFile = fopen("mapa.txt", "r")) == NULL) {      //otwieramy plik w którym jest napisana plansza w cyfrach
      //fprintf(stderr, "Musisz jeszcze pobrać plik z planszą i umieścić go w folderze gry!\n");
      return 1;
    }
    for(size_t i = 0; i < 55; i++) {
      for(size_t j = 0; j < 31; j++) {
        map[i][j] = ' ';    //rysujemy na razie pustą mape kolizji
      }
    }

    FILE* ghostFile;

    if((ghostFile = fopen("ghostmap.txt", "r")) == NULL){
      //fprintf(stderr, "Musisz jeszcze pobrać plik dla duszków i umieścić go w folderze gry!\n");
      return 1;
    }
    for(size_t i = 0; i < 55; i++) {
      for(size_t j = 0; j < 31; j++) {
        mapG[i][j] = ' ';
      }
    }

    char c = getc(ghostFile);

    size_t readX = 0;
    size_t readY = 0;

    while( c != EOF ) {        
      if(c == '\n') {
        readX = 0;      
        readY++;
      } else {
        mapG[readX][readY] = c; 
        readX++;
      }
      c = getc(ghostFile);
    }

    int punkty = 0;

    c = getc(mapFile);

    readX = 0;
    readY = 0;

    while( c != EOF ) {        //ustalamy dla całego pliku czy dany znak jest ścianą czy nie
      if(c == '\n') {
        readX = 0;      //0 oznacza brak ściany = można się ruszać
        readY++;
      } else {  
        if((c >= '0' && c <= '9')) {
          mapC[readX][readY] = 1;      //1 oznacza ścianę
        } else if(c == 'X') {
          mapC[readX][readY] = 2;       //2 oznacza również ścianę, ale X nie ma własnego odpowiednika w tablicy blockTypes, dlatego jest niewidzialny
        } else if(c == 'Y'){
          mapC[readX][readY] = 3;
        } else if(c == 'Z'){
          mapC[readX][readY] = 4;
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


    for(size_t i = 0; i < 4; i++) {
      duszki[i]->color_pair = 4 + i;
    }


    while(isPlaying) 
    {
      /*
        typ zmiennej clock_t, funkcja clock() i makro CLOCKS_PER_SEC pochodzą z biblioteki time
        funkcja clock() zwraca czas procesorowy programu, który z dobrym
        przybliżeniem określa całkowity rzeczywisty czas działania programu
      */
      clock_t prevTime = clock();

      switch(currentGameState) {         //tutaj mamy switcha który nam łatwo przeskakuje na różne stany gry
        case stateMenu:
          menu();
          break;
        case statePlaying:
          mainGame();
          break;
        case stateChangeName:
          changeName();
          break;
        case stateQuit:
          isPlaying = false;
          break;
        }

        /*
          czekamy aż max będzie dany amount of fps
          pętla rozpoczyna się od zebrania aktualnego czasu procesorowego
          kolejnym krokiem jest znalezienie upływu (różnicy) czasu, przy czym
          dzielenie przez CLOCKS_PER_SEC służy do zamiany jednostki czasu na sekundy
          upływ czasu jest następnie porównywany z czasem potrzebym na wyświetlenie
          pojedynczej klatki (dla częstotliwości f upływ czasu na pojedyncze zdarzenie
          wynosi 1/f)
          w momencie, w którym upłynie wystarczająco dużo czasu warunek if zostaje spełniony,
          więc kod dochodzi do break i opuszcza pętlę czekającą pozwalając na refresh
        */
        while(true) {
          clock_t curTime = clock();  
          if( ( (double)curTime-(double)prevTime)/CLOCKS_PER_SEC > (1.0/desiredFPS))
            break;
        }
        refresh(); //refresh screen to match the memory
    }
        fclose(mapFile);
        fclose(ghostFile);
//ඞ
    endwin(); //zamknij ncurses  
    return 0;
}

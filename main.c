#include <locale.h>
#include <ncurses.h> //biblioteka z ktorej bedziemy korzystac
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#define MENU_CHOICE_AMT 4

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
    int dir;
    int dirInit;
    int wait;
  } Duszek;

Duszek red = {
        .x = 23,
        .y = 14,
        .kolor = COLOR_RED,
        .dir = dirNone,
        .dirInit = dirRight,
        .wait = 1};

Duszek green = {
        .x = 19,
        .y = 19,
        .kolor = COLOR_GREEN,
        .dir = dirNone,
        .dirInit = dirTop,
        .wait = 2};

Duszek cyan = {
        .x = 21,
        .y = 21,
        .kolor = COLOR_CYAN,
        .dir = dirNone,
        .dirInit = dirTop,
        .wait = 3};

Duszek magenta = {
        .x = 18,
        .y = 18,
        .kolor = COLOR_MAGENTA,
        .dir = dirNone,
        .dirInit = dirTop,
        .wait = 4};

int plansza(){
  
    start_color();
    init_pair(1,COLOR_BLUE, COLOR_BLACK);  
    init_pair(2,COLOR_WHITE, COLOR_BLACK);       //ustawiamy kolor planszy na niebieski
    
 
    for(size_t j = 0; j < 31; j++) {           //w podwójnej pętli rysujemy planszę
      for(size_t i = 0; i < 55; i++) {
        if(map[i][j] >= '0' && map[i][j] <= '9') {           //jeżeli na danym koordynacie będzie liczba od 0 do 9,
          attron(COLOR_PAIR(1));
          printw("%s", blockTypes[(int)map[i][j] - 48]); 
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
    
    start_color();
    wattron(win, COLOR_PAIR(2));
    if (score>highscore){
      highscore = score;
    }
    
    mvwprintw(win, 0, 16, " * * * PAC - MAN * * * ");     //wypisujemy elementy menu
    mvwprintw(win, 2, 2, "Highscore: %d", highscore);
    mvwprintw(win, 3, 2, "Player: %s", username);
    wrefresh(win);

    int menuy=(height/2)-2;    //położenie menu
    int menux=20;
    
    char* options[MENU_CHOICE_AMT] = {      //MENU_CHOICE_AMT jest #defined jako 4, przyda się to później przy
      "PLAY ",
      "CHANGE NICKNAME ",                   //przeklikiwaniu przez opcje w menu
      "HIGH SCORES ",
      "QUIT "
    };

    int currentChoice = 0;

    int choiceResults[] = {
      statePlaying,
      stateChangeName,
      stateHighScores,
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
  echo();
  curs_set(1); //widoczny kursor
  wgetstr(win, username);
  for (int i=0; i <15; i++){
    char c = getch();
    if((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)){ //poprawny nick to tylko litery i cyfry
      username[i] = c;
    }else if (getch() == '\n') { //enter zatwierdza nazwę
      username[i+1]='\0';
      noecho();
      curs_set(0);
      currentGameState = stateMenu;
      break;
    } else if(getch() == 27 || getch()==10) {     //escape || enter
      noecho();
      curs_set(0);
      currentGameState = stateMenu;
      break;
    }
  }
  noecho();
  curs_set(0);
  wrefresh(win);
  return 0;
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
            } else if(mapC[x + attemptedX][y + attemptedY] == 4){
                  x+= attemptedX;
                  y+= attemptedY;
                  map[x][y] = ' ';
                  mapC[x][y] = 0;
                  score += 50;
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


          /* Portal ඞ*/
          if (y==14){
            if (x<=0){
            x = 55-x;
            } else if ((x>=54) && attemptedX==1){
              x = 0;
            }
          }

          /*Duszki logika ඞ*/ 

          if(red.wait == 0){
            red.dir = red.dirInit;
          }
          red.wait--;

        switch(red.dir) {          //każdy z kierunków ma swoją wartość
            case dirLeft:
              red.attemptedX = -1;
              break;
            case dirRight:
              red.attemptedX = 1;
              break;
            case dirTop:
              red.attemptedY = -1;
              break;
            case dirBottom:
              red.attemptedY = 1;
              break;
        }


      if(mapG[i][j] == 'O'){
        red.x += red.attemptedX;
        red.y += red.attemptedY; 
      } else if(mapG[i][j] == 'Q'){
        if(red.dir == dirLeft){
          red.y++;
          red.dir = dirBottom;
        } else {
          red.x++;
          red.dir = dirRight;
        }
      }else if(mapG[i][j] == 'W'){
        if(red.dir == dirLeft){
          red.y--;
          red.dir = dirTop;
        } else {
          red.x++;
          red.dir = dirRight;
        }

      }else if(mapG[i][j] == 'E'){
        if(red.dir == dirRight){
          red.y++;
          red.dir = dirBottom;
        } else {
          red.x--;
          red.dir = dirLeft;
        }
        
      }else if(mapG[i][j] == 'R'){
        if(red.dir == dirLeft){
          red.y--;
          red.dir = dirTop;
        } else {
          red.x--;
          red.dir = dirLeft;
        }

      }else if(mapG[i][j] == 'T'){
        if(red.dir == dirRight){
          red.y++;
          red.dir = dirBottom;
        } else if{
          red.x--;
          red.dir = dirLeft;
        }
        
      }else if(mapG[i][j] == 'Y'){

      }else if(mapG[i][j] == 'U'){
        
      }else if(mapG[i][j] == 'I'){

      }else if(mapG[i][j] == 'P'){
        
      }else if(mapG[i][j] == 'L'){

      }





          init_pair(4, red.kolor, COLOR_BLACK); 
          wrefresh(win);
          wattron(win, COLOR_PAIR(4));
          mvwprintw(win, red.y, red.x, "ඞ"); //ඞ?
          wattroff(win, COLOR_PAIR(4));
          wmove(win,red.y,red.x);

        


/* mapa literki for future reference
Q - right or down
W - right or top
E - left or down
R - left or top

T - not right
Y - not left
U - not bottom
I - not top

P - all
L - teleport
*/
}

int main()
{

    //menu();

    FILE* mapFile;

    if( ( mapFile = fopen("mapa.txt", "r")) == NULL) {      //otwieramy plik w którym jest napisana plansza w cyfrach
      fprintf(stderr, "Musisz jeszcze pobrać plik z planszą i umieścić go w folderze gry!\n");
      return 1;
    }
    for(size_t i = 0; i < 55; i++) {
      for(size_t j = 0; j < 31; j++) {
        map[i][j] = ' ';    //rysujemy na razie pustą mape kolizji
      }
    }

    FILE* ghostFile;

    if((ghostFile = fopen("ghostmap.txt", "r")) == NULL){
      fprintf(stderr, "Musisz jeszcze pobrać plik dla duszków i umieścić go w folderze gry!\n");
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
        } else if(c == 'Y' || c == 'Z'){
          mapC[readX][readY] = 3;
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
//ඞ
    endwin(); //zamknij ncurses  
    return 0;
}

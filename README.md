# Pac-Man
Pac-Man to popularna gra zręcznościowa z roku 1980, wydana przez firmę Namco na automaty do gier. Obecnie jest to jedna z najpopularniejszych gier. Polega na poruszaniu się po labiryncie oraz uciekaniu orzed Duszkami, jednocześnie zdobywając punkty.
Nasza wersja gry Pac-Man powstała w języku C z wykorzystaniem zasobów biblioteki ncurses, umożliwiającej tworzenie aplikacji uruchamianych w oknie konsoli. Niektóre funkcje oryginalnej gry zostały zmienione - zachęcamy do przeczytania informacji poniżej.
## Pierwsze uruchomienie
Instalacja ncurses

By gra poprawnie działała, musimy upewnić się, że biblioteka ncurses jest zainstalowana na naszym komputerze - informacje jak to zrobić dostępne <a href="https://invisible-island.net/ncurses/#downloads">tutaj</a>.

Pobieramy paczkę z grą (Code -> Download ZIP), w folderze uruchamiamy terminal i wpisujemy komendę:

`$ gcc main.c -lncursesw && ./a.out`


## Funkcje gry

### Menu

![image](https://user-images.githubusercontent.com/43135879/175037411-a8562e27-8380-44ad-b03c-4bb1d7f714e1.png)

Strzałkami poruszamy się po menu, nasz wybór potwierdzamy przyciskiem ENTER.
U góry ekranu widzimy najlepszy zdobyty wynik oraz obecnego gracza.

### Change Nickname

![image](https://user-images.githubusercontent.com/43135879/175037577-8b3ba345-2077-4c26-bcac-49c882dfa3d4.png)

W opcji Change Nickname zmieniamy nazwę naszego gracza.


### Play
#### Rozgrywka:

Sterowanie: strzałki

ESC - wyjście do menu

![image](https://user-images.githubusercontent.com/43135879/175037708-36d21e2f-c522-4815-af86-ef087de86fdf.png)

#### Punktacja:

małe owoce: +10pkt

duże owoce: +50pkt

PacMan zatrzymuje się po napotkaniu ściany - sami również możemy go w ten sposób zatrzymać, co umożliwia nowe strategie (w oryginalnej grze PacMan zatrzymuje się sam).

Ale, ale! Złe Duszki mogą chcieć cię zjeść - uciekaj przed nimi!

![image](https://user-images.githubusercontent.com/43135879/175038004-af0a2a4e-731d-4def-b0b5-cf10041df613.png)


Jeśli podczas gry żaden Duszek cię nie dopadnie wygrywasz!

![image](https://user-images.githubusercontent.com/43135879/175039944-eda811ce-1f1d-4586-a655-6728c6faa4ff.png)



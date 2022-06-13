# Pac-Man
Pac-Man to popularna gra zręcznościowa z roku 1980, wydana przez firmę Namco na automaty do gier. Obecnie jest to jedna z najpopularniejszych gier. Polega na poruszaniu się po labiryncie oraz uciekaniu orzed Duszkami, jednocześnie zdobywając punkty.
Nasza wersja gry Pac-Man powstała w języku C z wykorzystaniem zasobów biblioteki ncurses, umożliwiającej tworzenie aplikacji uruchamianych w oknie konsoli. Niektóre funkcje oryginalnej gry zostały zmienione - zachęcamy do przeczytania informacji poniżej.
# Pierwsze uruchomienie
Instalacja ncurses

By gra poprawnie działała, musimy upewnić się, że biblioteka ncurses jest zainstalowana na naszym komputerze - informacje jak to zrobić dostępne <a href="https://invisible-island.net/ncurses/#downloads">tutaj</a>.

Pobieramy paczkę z grą (Code -> Download ZIP), w folderze uruchamiamy terminal i wpisujemy komendę:

$ gcc main.c -lncursesw

$ ./a.out

Menu

![image](https://user-images.githubusercontent.com/43135879/173416721-be97bd32-fa7b-4ff0-9c2d-e5e82385caac.png)

Strzałkami poruszamy się po menu, nasz wybór potwierdzamy przyciskiem ENTER.
U góry ekranu widzimy najlepszy zdobyty wynik oraz obecnego gracza.

Change Nickname

![image](https://user-images.githubusercontent.com/43135879/173417117-3b45916d-0031-41b3-ac14-8099d432798d.png)

W opcji Change Nickname zmieniamy nazwę naszego gracza.

High Scores

Tu mamy dostęp do 3 najlepszych graczy - wraz z ich rekordami.


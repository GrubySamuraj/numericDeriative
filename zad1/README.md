Wykres Błędu Numerycznego Pochodnej

Ten projekt ilustruje błąd numeryczny pojawiający się podczas obliczania pochodnej funkcji za pomocą różnic skończonych przy użyciu zarówno precyzji float, jak i double. Używana funkcja to:

f(x) = sin(x^3)

Projekt porównuje wartości pochodnych obliczonych numerycznie dla typów float i double z rzeczywistą, analityczną pochodną:

f'(x) = 3 _ x^2 _ cos(x^3)

Następnie błąd numeryczny jest przedstawiany na wykresie w funkcji h (kroku różnicowym użytym do obliczeń) w skali logarytmicznej.
Wymagania

    Kompilator G++
    GNUplot (do wizualizacji wyników)

Pliki

    zad1.cpp: Główny plik źródłowy, który oblicza pochodne numeryczne i rysuje błąd przy użyciu GNUplot.
    Makefile: Zawiera polecenia do kompilacji i uruchamiania kodu.

Kompilacja i Uruchomienie

1. Kompilacja

Aby skompilować program, użyj:

    make

2. Uruchomienie

Po kompilacji, aby uruchomić program, wpisz:

    make run_cw1

gdzie run_cw1 to jest podpunkt a z zadania - (f(x+h)-f(x-h))/h.

    make run_cw1_2

Aby wyczyścić wszystko z plikóœ skompilowanych użyj
make clean

a run_cw1_2 to jest podpunkt b, gdzie jest to (f(x+h) - f(x-h))/2\*h
Wynikiem będzie wygenerowanie wykresu błędów numerycznych dla typów float i double.

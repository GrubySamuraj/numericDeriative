**Opis Projektu**

Ten projekt jest programem napisanym w C++, który wykorzystuje bibliotekę Eigen do rozwiązywania równań macierzowych oraz analizowania wpływu małych zaburzeń na rozwiązania. Program oblicza wyniki dla dwóch macierzy A1A1​ i A2A2​ oraz analizuje, jak drobne zmiany wektora bb wpływają na wynik.
Wymagania

Aby skompilować i uruchomić ten program, potrzebujesz:

    Kompilatora g++ z obsługą C++17
    Biblioteki Eigen (ścieżka do nagłówków: /usr/include/eigen3)

Instrukcja instalacji i kompilacji

    Skopiuj kod źródłowy do folderu projektu.

    Upewnij się, że masz zainstalowaną bibliotekę Eigen (możesz zainstalować ją za pomocą menedżera pakietów, np. sudo apt install libeigen3-dev).

    W terminalu przejdź do katalogu projektu.

    Użyj polecenia make, aby skompilować projekt.

    make

_Instrukcja uruchomienia_

Po skompilowaniu możesz uruchomić program za pomocą polecenia:

make run

Program wykona obliczenia i wyświetli wyniki na ekranie, w tym rozwiązania układów równań dla macierzy A1​ i A2​ oraz różnice wynikające z zaburzeń wektora b.
Czyszczenie plików

Aby usunąć pliki obiektowe i plik wykonywalny, użyj polecenia:

make clean

Tworzenie archiwum

Aby utworzyć archiwum .tar.gz z kodem źródłowym i plikiem Makefile, użyj polecenia:

make tar

Spowoduje to utworzenie pliku main.tar.gz, który zawiera wszystkie pliki źródłowe projektu oraz plik Makefile.

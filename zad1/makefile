# Kompilator i flagi
CC = g++
RM = rm -f
CFLAGS  = -g -Wall

# Pliki docelowe
TARGET = zad1
TARGET2 = zad1_2

# Domyślny cel
all: $(TARGET).out $(TARGET2).out

# podpunkt a
$(TARGET).out: $(TARGET).cpp
	$(CC) $(CFLAGS) $< -o $@

run_cw1: $(TARGET).out
	./$(TARGET).out

# podpunkt b
$(TARGET2).out: $(TARGET2).cpp
	$(CC) $(CFLAGS) $< -o $@

run_cw1_2: $(TARGET2).out
	./$(TARGET2).out

# Czyszczenie
clean:
	$(RM) *.out

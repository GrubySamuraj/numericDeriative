CC = g++
RM = rm

CFLAGS  = -g -Wall

TARGET = zad1
TARGET2 = zad1_2


all: $(TARGET)

#podpunkt a
run_cw1:
	$(CC) $(CFLAGS) $(TARGET).cpp -o $(TARGET).out
	./$(TARGET).out

# podpunkt b
run_cw1_2:
	$(CC) $(CFLAGS) $(TARGET2).cpp -o $(TARGET2).out
	./$(TARGET2).out

clean:
	$(RM) *.out 

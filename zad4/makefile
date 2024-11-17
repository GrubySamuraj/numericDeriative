TARGET = main
SRC = main.cpp
CXX = g++
CXXFLAGS = -I /usr/include/eigen3 -O2 -std=c++17
ARCHIVE = archive.tar.gz

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

archive: $(TARGET) $(SRC)
	tar -czvf $(ARCHIVE) $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

clean-archive:
	rm -f $(ARCHIVE)

clean-all: clean clean-archive

.PHONY: run archive clean clean-archive clean-all

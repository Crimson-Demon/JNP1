CC     = g++
CFLAGS = -MP -MD -O2 -g -Wall -std=c++11 -lboost_regex

OUT = gsb_example
SRC = gsb_example.cpp gkb.cpp bankbuilder.cpp bank.cpp exchangetable.cpp exchangerate.cpp account.cpp interstellarclock.cpp planets.cpp citizens.cpp
OBJ = $(SRC:.cpp=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f ./*.o
	rm -f ./*.d

-include $(SRC:.cpp=.d)

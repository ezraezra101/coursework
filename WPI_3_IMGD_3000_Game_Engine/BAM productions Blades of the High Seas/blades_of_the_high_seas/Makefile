CC= g++
COMPILER_OPTS= #-Wall

# Library locations
SFML= ../sfml/2.3/
ENGINE= ../dragonfly/
ENGINELIB=$(ENGINE)dragonfly.a

LINK= -L$(SFML)lib/ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio $(ENGINELIB)
# Don't need sfml-network or Winmmm maybe sfml-main?

INCLUDE= -Iinclude/ -I$(SFML)include -I$(ENGINE)include/ 
SRC= $(wildcard src/*.cpp)
HEADERS= $(wildcard include/*.h)
OBJECTS= $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))

all: Makefile game

run: game
	./game

# Builds dragonfly by running make in its directory...
$(ENGINELIB):
	cd $(ENGINE); make

game: game.cpp $(SRC) $(OBJECTS) $(HEADERS) $(ENGINELIB)
	$(CC) $(COMPILER_OPTS) $(OBJECTS) $(INCLUDE) $(LINK) $< -o $@
	# Use relative version of sfml.
	install_name_tool -change /usr/local/lib/libsfml-graphics.2.3.dylib @executable_path/../sfml/2.3/lib/libsfml-graphics.2.3.dylib game
	install_name_tool -change /usr/local/lib/libsfml-window.2.3.dylib @executable_path/../sfml/2.3/lib/libsfml-window.2.3.dylib game
	install_name_tool -change /usr/local/lib/libsfml-system.2.3.dylib @executable_path/../sfml/2.3/lib/libsfml-system.2.3.dylib game
	install_name_tool -change /usr/local/lib/libsfml-audio.2.3.dylib @executable_path/../sfml/2.3/lib/libsfml-audio.2.3.dylib game


obj/%.o: src/%.cpp $(HEADERS)
	$(CC) $(COMPILER_OPTS) -c $(INCLUDE) $< -o $@

obj/Corporal.o: src/Corporal.cpp $(HEADERS) obj/Foe.o
	$(CC) $(COMPILER_OPTS) -c $(INCLUDE) $< -o $@

build_options:
	@echo "ENGINE: $(ENGINE)"
	@echo "SRC: $(SRC)"
	@echo "OBJECTS: $(OBJECTS)"

clean:
	rm game
	rm -f $(OBJECTS)
	rm -f dragonfly.log

CC= g++
COMPILER_OPTS= #-Wall
LINK= -L/usr/local/Cellar/sfml/2.3/lib/ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# Don't need sfml-network or Winmmm maybe sfml-main?

ENGINE= dragonfly.a

INCLUDE= -Iinclude/
SRC= $(wildcard src/*.cpp)
OBJECTS= $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))

# Tests
UNIT_TESTS= $(wildcard unit_tests/*.cpp)
UNIT_TEST_EXECUTABLES=$(addprefix unit_tests_exec/,$(notdir $(UNIT_TESTS:.cpp=.out)))

INTEGRATION_TESTS= $(wildcard integration_tests/*.cpp)
INTEGRATION_TEST_EXECUTABLES=$(addprefix integration_tests_exec/,$(notdir $(INTEGRATION_TESTS:.cpp=.out)))


all: $(ENGINE) Makefile

$(ENGINE): $(SRC) $(OBJECTS)
	ar rcs $(ENGINE) $(OBJECTS)

obj/%.o: src/%.cpp include/*.h
	$(CC) $(COMPILER_OPTS) -c $(INCLUDE) $(LINK) $< -o $@

unit_tests_exec/%: unit_tests/%.cpp $(ENGINE)
	$(CC) $(COMPILER_OPTS) $(INCLUDE) $(ENGINE) $(LINK) $< -o $@

integration_tests_exec/%: integration_tests/%.cpp $(ENGINE)
	$(CC) $(COMPILER_OPTS) $(INCLUDE) $(ENGINE) $(LINK) $< -o $@

tests: $(ENGINE) $(UNIT_TEST_EXECUTABLES) $(INTEGRATION_TEST_EXECUTABLES)

run_tests: tests
	@rm -f dragonfly.log
	@echo "\n    Removed old logfile"
	@make run_unit_tests
	@make run_integration_tests
	
	@echo "\n\n"

run_unit_tests: tests
	@echo "    Running unit tests..."
	@# Don't ask why this works...
	@$(UNIT_TEST_EXECUTABLES:=;)

run_integration_tests: tests
	@echo "\n    Running integration tests..."
	@$(INTEGRATION_TEST_EXECUTABLES:=;)

clean:
	rm -f $(OBJECTS)
	rm -f $(ENGINE)
	rm -f $(UNIT_TEST_EXECUTABLES)
	rm -f $(INTEGRATION_TEST_EXECUTABLES)
	rm -f dragonfly.log
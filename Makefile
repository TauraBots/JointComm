CC = gcc
CXX = g++
CFLAGS = -I./include -I/usr/include -pthread -Wall -O2
CXXFLAGS = -I./include -I/usr/include -Wall -O2
LDFLAGS = -L./lib -lm -lpthread
AR = ar
OBJECTS = obj/dynamixel.o obj/dxl_hal.o obj/jay_test_ids.o
RM = echo "hello"
INSTALL_PATH = /usr/local

.SECONDARY: $(OBJECTS)

.PHONY: all
all: bin/jay_test_ids
	@echo $(COLOR)All done! $(NOCOLOR)

COLOR = '\033[0;32m'
NOCOLOR='\033[0m'

bin/%: obj/%.o $(OBJECTS)
	@echo -e $(COLOR)Linking $@ $(NOCOLOR)
	@$(CXX) $^ -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	@echo -e $(COLOR)Compiling $@ $(NOCOLOR)
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

obj/%.o: src/%.c
	@echo -e $(COLOR)Compiling $@ $(NOCOLOR)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	@echo -e $(COLOR)Cleaning all binaries $(NOCOLOR)
	@rm -f obj/*.o lib/*.a bin/*

.PHONY: install
install:
	@echo -e $(COLOR)Installing library, binaries and headers at $(INSTALL_PATH) $(NOCOLOR)
	@cp lib/* $(INSTALL_PATH)/lib
	@mkdir -p $(INSTALL_PATH)/include/dimitri
	@cp include/*.h $(INSTALL_PATH)/include/dimitri
	@cp bin/* $(INSTALL_PATH)/bin

.PHONY: uninstall
uninstall:
	@echo -e $(COLOR)Removing library, binaries and headers $(NOCOLOR)
	@rm -rf $(INSTALL_PATH)/include/dimitri
	@rm -f $(INSTALL_PATH)/lib/libdimitri.a
	@rm -f $(INSTALL_PATH)/bin/dimitri*

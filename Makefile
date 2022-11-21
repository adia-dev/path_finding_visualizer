#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
CXX = clang++

EXE_DIR = bin
EXE = $(EXE_DIR)/app
SOURCES_DIR = src
IMGUI_DIR = include/imgui
SOURCES = $(wildcard src/*.cpp src/**/*.cpp)
HEADERS = $(wildcard src/*.h src/**/*.h)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
# set the object files directory to the source directory
OBJS_DIR = $(SOURCES_DIR)
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -D__DIR__=\"$(SOURCES_DIR)/\" -D__LOG__=\"./logs/$(shell date +%Y-%m-%d_%H-%M-%S).log\" -D__LOG_TO_FILE__
CXXFLAGS += -g -Wall -Wformat 
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:$(SOURCES_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "Compiling $<"
	@echo "Linking $@"

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo "\033[1;32mBuild complete!\033[0m for the \033[1;33m$(ECHO_MESSAGE)\033[0m platform"

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

recompile: $(EXE)
	@echo "\033[32m$(EXE) has been recompiled\033[0m"
	@echo "\033[32mRun it with ./$(EXE)\033[0m"

# run the program with the command "make run", recommended to use "make recompile" first to make sure the program is up to date
run: $(EXE)
	@echo "\033[32mRunning $(EXE)\033[0m"
	@echo "\033[32mPress Ctrl+C to exit\033[0m"
	@./$(EXE)

playground:
	@echo "\033[32mRunning $(EXE) playground\033[0m"
	@echo "\033[32mPress Ctrl+C to exit\033[0m"
	make recompile
	@./$(EXE) -playground

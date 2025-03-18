COMPILER =g++
BINARY =bin/Sjene

FLAGS_COMPILER =-std=c++11 -g -Wall -Wextra -pedantic -I./Vendor/
FLAGS_LINKER =-lGL -lGLU -lGLEW -lglfw

FILES_SOURCE =Application.cpp src/Window.cpp src/Camera.cpp src/VertexParser.cpp src/Model.cpp src/Shader.cpp
#FILES_HEADER =???

FILES_OBJECT =$(FILES_SOURCE:%.cpp=obj/%.o)

$(BINARY): $(FILES_OBJECT)
	mkdir -p bin/
	$(COMPILER) $(FILES_OBJECT) $(FLAGS_LINKER) -o $(BINARY)

obj/Application.o: Application.cpp inc/Window.hpp inc/Camera.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Window.o: src/Window.cpp inc/Window.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Camera.o: src/Camera.cpp inc/Camera.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/VertexParser.o: src/VertexParser.cpp inc/VertexParser.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Model.o: src/Model.cpp inc/Model.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Shader.o: src/Shader.cpp inc/Shader.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

clean:
	rm -rf obj/ bin/

run:
	./$(BINARY)

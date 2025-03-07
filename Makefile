COMPILER =g++
BINARY = bin/Sjene

FLAGS_COMPILER =-std=c++11 -g -Wall -Wextra -pedantic -I./Vendor/
FLAGS_LINKER =-lglfw -lGLU -lGL -lGLEW

#SRC = Application.cpp src/camera.cpp src/initEverything.cpp src/loadShader.cpp src/meshVertices.cpp src/model.cpp src/ShadowMap_dir.cpp src/ShadowMap_omni.cpp src/ShadowMap_variance.cpp src/ShadowVolume.cpp
FILES_SOURCE =Application.cpp
#FILES_HEADER =???

FILES_OBJECT =$(FILES_SOURCE:%.cpp=obj/%.o)

$(BINARY): $(FILES_OBJECT)
	mkdir -p bin/
	$(COMPILER) $(FLAGS_LINKER) $(FILES_OBJECT) -o $(BINARY)

obj/Application.o: Application.cpp
	mkdir -p obj/src/
	$(COMPILER) $(FLAGS_COMPILER) -c $< -o $@

clean:
	rm -rf obj/ bin/

run:
	./$(BINARY)

COMPILER =g++
BINARY =bin/Sjene

FLAGS_COMPILER =-std=c++11 -g -Wall -Wextra -pedantic -I./Vendor/
FLAGS_LINKER =-lGL -lGLU -lGLEW -lglfw

FILES_SOURCE =Application.cpp src/Window.cpp src/Camera.cpp \
	src/VertexAttributeParser.cpp src/Model.cpp src/Shader.cpp \
	src/Scene.cpp src/DefaultScene.cpp src/DirectionalShadowMapScene.cpp \
	src/DirectionalShadowMap.cpp src/OmnidirectionalShadowMapScene.cpp \
	src/OmnidirectionalShadowMap.cpp

#FILES_HEADER =???

FILES_OBJECT =$(FILES_SOURCE:%.cpp=obj/%.o)

$(BINARY): $(FILES_OBJECT)
	mkdir -p bin/
	$(COMPILER) $(FILES_OBJECT) $(FLAGS_LINKER) -o $(BINARY)

obj/Application.o: Application.cpp inc/Scene.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Window.o: src/Window.cpp inc/Window.hpp inc/Camera.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Camera.o: src/Camera.cpp inc/Camera.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/VertexAttributeParser.o: src/VertexAttributeParser.cpp inc/VertexAttributeParser.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Model.o: src/Model.cpp inc/Model.hpp inc/VertexAttributeParser.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Shader.o: src/Shader.cpp inc/Shader.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/Scene.o: src/Scene.cpp inc/Scene.hpp inc/Window.hpp inc/Model.hpp \
	inc/VertexAttributeParser.hpp inc/Shader.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/DefaultScene.o: src/DefaultScene.cpp inc/DefaultScene.hpp inc/Scene.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/DirectionalShadowMapScene.o: src/DirectionalShadowMapScene.cpp inc/DirectionalShadowMapScene.hpp inc/Scene.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/DirectionalShadowMap.o: src/DirectionalShadowMap.cpp inc/DirectionalShadowMap.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

## This lower line might not be true. Looks like dependencies of 'OmnidirectionalShadowMap' is missing.
obj/src/OmnidirectionalShadowMapScene.o: src/OmnidirectionalShadowMapScene.cpp inc/OmnidirectionalShadowMapScene.hpp inc/Scene.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

obj/src/OmnidirectionalShadowMap.o: src/OmnidirectionalShadowMap.cpp inc/OmnidirectionalShadowMap.hpp
	mkdir -p obj/src/
	$(COMPILER) -c $< $(FLAGS_COMPILER) -o $@

clean:
	rm -rf obj/ bin/

run:
	./$(BINARY)

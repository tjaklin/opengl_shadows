### README ###

### Usage ###

This program is comprised of 4 states which can be shuffled through by using "1"-"4" keys on your keyboard:
1) A simple scene with a directional light source, using the Shadow Mapping technique
2) A simple scene with a directional light source, using the Variance Shadow Mapping technique
3) A simple scene with a positional light source, using the Shadow Mapping technique
4) A simple scene with a positional light source, using the Shadow Volume technique

A light source present in each state is automatically moved in each frame but you can pause it by pressing "L".
By pressing "Q" in the 2nd and 3rd state you can turn on / off the PCF filtering technique (It is turned off by default).

### Tools used ###
g++.
glm, glfw, glew libraries.

Compiled and linked with: "g++ -std=c++11  main.cpp enkapsulacije_H/*cpp -lglfw -lGLU -lGL -lGLEW"

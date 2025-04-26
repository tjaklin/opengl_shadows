## About
The program implements some of the most widely used techniques for calculating and drawing shadows
in computer graphics. The user starts out with the 'Directional Shadow maps' technique, but the choice
**can be changed** in the Application.cpp's **main function**.
>Make sure to clone the repo with **-\-recursive** to download all the necessary submodules.
>If building on a Linux platform, additional dependencies might be needer for GLFW to compile correctly. Please refer to [this](https://www.glfw.org/docs/latest/compile_guide.html#compile_deps) for more information.

## Implemented techniques:
- Directional shadow maps
- Omnidirectional shadow maps
-  Variance shadow maps
- Shadow volume

## Tools used
- OpenGL version 3.3 (GLSL 330)
- GLEW version 2.1.0
- GLFW version 3.4
- stb_image version 2.30
- GLM version 1.0.1
- Cmake 3.20
- C++11

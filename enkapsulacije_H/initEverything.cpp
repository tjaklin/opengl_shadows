#include "initEverything.h"

GLFWwindow* initEverything(int windowWidth, int windowHeight){
	GLFWwindow* window = NULL;

	if( !glfwInit() ){						
		std::cout<<"Error in glfwInit()! Closing... "<<std::endl;
		return NULL;
	}
	// Setting glfwWindow hints
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Creating a Window
	window = glfwCreateWindow( windowWidth, windowHeight, "Program", NULL, NULL);
	if( !window ){
		std::cout<<"Error in glfwCreateWindow()! Closing... "<<std::endl;
		glfwTerminate();
		return NULL;	
	}
	// Creating an OpenGL context that is tied to the window pointer
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true;
	if( glewInit() != GLEW_OK ){
		std::cout<<"Error in glewInit()! Closing... "<<std::endl;
		glfwTerminate();
		return NULL;
	}
return window;
}
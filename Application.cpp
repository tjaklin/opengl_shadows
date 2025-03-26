// TODO: Ubacit sve ove '3rd party dependencije' u Dependency folder, koji je dio
//			projekta samoga !
// TODO: Gornji tudu malo izmjenit, na nacin da zapravo ne ubacim sve dependensije u
//			zaseban Dependency folder, nego da umjesto toga, u repozitorij, ubacim
//			koristenje CMake-a, umjesto trenutnog Make-a. Zatim sa tim CMake-om mogu
//			napisat pravila za ocekivane Dependencyje koje projekt koristi i, kaj
//			napravit u slucaju da nisu pristuni (tj. napisat pravilo da se Dependencyji
//			automacki skinu s githaba) !

#include "inc/Window.hpp"
#include "inc/Scene.hpp"
#include "inc/DefaultScene.hpp"
#include "inc/ShadowMapScene.hpp"
#include "inc/DirectionalShadowMap.hpp"

/*
    #include "inc/ShadowMap_omni.h"
    #include "inc/ShadowVolume.h"
    #include "inc/ShadowMap_variance.h"
*/

/*
void checKeyState( uint* state, bool* isMoving, bool* isPCF, GLFWwindow* mWindow ){
	if (glfwGetKey( mWindow, GLFW_KEY_1 ) == GLFW_PRESS)
		*state = 1;
	if (glfwGetKey( mWindow, GLFW_KEY_2 ) == GLFW_PRESS)
		*state = 2;
	if (glfwGetKey( mWindow, GLFW_KEY_3 ) == GLFW_PRESS)
		*state = 3;
	if (glfwGetKey( mWindow, GLFW_KEY_4 ) == GLFW_PRESS)
		*state = 4;
	if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_PRESS)
		if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_RELEASE)
			*isMoving = !*isMoving;
	if (glfwGetKey( mWindow, GLFW_KEY_Q ) == GLFW_PRESS)
		if (glfwGetKey( mWindow, GLFW_KEY_Q ) == GLFW_RELEASE)
			*isPCF = !*isPCF;
}
*/

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    Window window(1024, 1024, "OpenGL Sjene");

	if (false)
	{
		DefaultScene default_scene(&window);
		default_scene.Run();
	}
	else
	{
		ShadowMapScene shadowmap_scene(&window);
		shadowmap_scene.Run();
	}

    return 0;
}

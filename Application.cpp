#include "inc/Window.hpp"

#include "inc/DefaultScene.hpp"
#include "inc/ShadowMapScene.hpp"

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    Window window(1024, 1024, "OpenGL Sjene");

	//DefaultScene default_scene(&window);
	//default_scene.Run();

	ShadowMapScene shadowmap_scene(&window);
	shadowmap_scene.Run();

    return 0;
}

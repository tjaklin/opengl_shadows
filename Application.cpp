#include "inc/Window.hpp"

#include "inc/DefaultScene.hpp"
#include "inc/DirectionalShadowMapScene.hpp"
#include "inc/OmnidirectionalShadowMapScene.hpp"

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    Window window(1024, 1024, "OpenGL Sjene");

	//DefaultScene default_scene(&window);
	//default_scene.Run();

	//DirectionalShadowMapScene dir_shadowmap_scene(&window);
	//dir_shadowmap_scene.Run();

    OmnidirectionalShadowMapScene omni_shadowmap_scene(&window);
    omni_shadowmap_scene.Run();

    return 0;
}

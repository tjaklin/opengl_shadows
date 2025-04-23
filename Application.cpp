#include "Window.hpp"

#include "DefaultScene.hpp"
#include "DirectionalShadowMapScene.hpp"
#include "OmnidirectionalShadowMapScene.hpp"
#include "VarianceShadowMapScene.hpp"
#include "ShadowVolumeScene.hpp"

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    Window window("OpenGL shadowing techniques");
    Scene* scene = nullptr;

    enum class ActiveScene
    {
        DEFAULT = 0,    // A simple scene with no shadows drawn.
        SHADOWMAP_DIRECTIONAL,
        SHADOWMAP_OMNIDIRECTIONAL,
        SHADOWMAP_VARIANCE,
        SHADOWVOLUME,
        BAREBONES   // A 'skeleton' implementation. No OpenGL techniques here.
    };

    switch (ActiveScene::SHADOWMAP_DIRECTIONAL)
    {
        case ActiveScene::DEFAULT:
            scene = new DefaultScene(&window);
            break;
        case ActiveScene::SHADOWMAP_DIRECTIONAL:
            scene = new DirectionalShadowMapScene(&window);
            break;
        case ActiveScene::SHADOWMAP_OMNIDIRECTIONAL:
            scene = new OmnidirectionalShadowMapScene(&window);
            break;
        case ActiveScene::SHADOWMAP_VARIANCE:
            scene = new VarianceShadowMapScene(&window);
            break;
        case ActiveScene::SHADOWVOLUME:
            scene = new ShadowVolumeScene(&window);
            break;
        case ActiveScene::BAREBONES:
            scene = new Scene(&window);
            break;
        default:
            printf("[ActiveScene] Incorrect choice.\n");
    }

    if (scene)
    {
        scene->Run();
        delete scene;
        scene = nullptr;
    }

    return 0;
}

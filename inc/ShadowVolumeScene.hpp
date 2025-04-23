#include "Scene.hpp"
#include "ShadowVolume.hpp"

class ShadowVolumeScene : public Scene
{
public:
    ShadowVolumeScene(Window* window);
    ~ShadowVolumeScene() override = default;

    void Run() const override;
};
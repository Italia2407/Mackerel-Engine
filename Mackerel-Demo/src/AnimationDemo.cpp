#include "AnimationDemo.h"

namespace MCK
{
    Demo::AnimationDemo::AnimationDemo()
    {
       
    }

    Demo::AnimationDemo::~AnimationDemo()
    {
        // Delete resources: TODO
    }

    void Demo::AnimationDemo::Init()
    {
        #pragma region UI Init
                uiComponent = new EntitySystem::UIComponent();
                //MCK::AssetType::Texture* image = uiComponent->LoadUIImage("../Mackerel-Core/res/Textures/TestImage.png");
                uiComponent->CreateStandardHUD(true);
        #pragma endregion
    }

    void  Demo::AnimationDemo::AddEntities(EntitySystem::Scene& scene)
    {
        EntitySystem::Entity* uiEntity = scene.CreateEntity();
        uiEntity->AddComponent(uiComponent);
    }
}



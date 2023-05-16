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
                MCK::AssetType::Texture* hudIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/PlayAnimation.png");
                uiComponent->CreateShape(true, ImVec2(10, 10), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, ImVec2(276, 100), ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, hudIMG);
        #pragma endregion

        animMesh = new MCK::AssetType::Mesh("three_bone");
        animMesh->LoadFromFile("../Mackerel-Core/res/Meshes/three_bone.glb");
    }

    void  Demo::AnimationDemo::AddEntities(EntitySystem::Scene& scene)
    {
        EntitySystem::Entity* uiEntity = scene.CreateEntity();
        uiEntity->AddComponent(uiComponent);
    }
}



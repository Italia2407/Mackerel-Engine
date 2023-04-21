#include "DemoApp.h"

DemoApp::DemoApp()
{}

DemoApp::~DemoApp()
{}

void DemoApp::MovingCubeCallback(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    Eigen::Vector2f inp{};
    if (key == MCK::Key::W)
    {
        inp.x() = 0.f;
        inp.y() = 1.f;
    }
    else if (key == MCK::Key::S)
    {
        inp.x() = 0.f;
        inp.y() = -1.f;
    }
    else if (key == MCK::Key::A)
    {
        inp.x() = -1.f;
        inp.y() = 0.f;
    }
    else if (key == MCK::Key::D)
    {
        inp.x() = 1.f;
        inp.y() = 0.f;
    }
    cubeInput = inp;
    //std::cout << "k" << key << " b" << (int)ButtonEvents << std::endl;
}

#include <filesystem>

void DemoApp::Start()
{
    /* Entity Creation */
    scene.InitialiseScene();
    entity = scene.CreateEntity();
    cameraEntity = scene.CreateEntity();
    floorEntity = scene.CreateEntity();

    transformComp.Position() = Eigen::Vector3f({ 0, 15, 20 });
    floorTransform.Position() = Eigen::Vector3f({ 0, -3, 20 });
    floorTransform.Scale() = Eigen::Vector3f({ 10, 1, 10 });

    /* Input setup */
    entity->AddComponent(&inputComp);

    /* Graphics setup */
    testMesh = new MCK::AssetType::Mesh("Test Mesh");
    testMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");
    testMaterial = new MCK::AssetType::Material();
    testMaterial->addUInt16Uniform("lightShaderID", 0);
    testMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, testShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, lightShader);

    meshRenderer = new MCK::EntitySystem::MeshRendererComponent(testMesh, testShader, testMaterial);
    floorRenderer = new MCK::EntitySystem::MeshRendererComponent(testMesh, testShader, testMaterial);

    MCK::Rendering::Renderer::AddUnlitShader(lightShader);

    cameraComponent = new MCK::EntitySystem::PerspectiveCamera(1280.f / 720.f);
    cameraComponent->Position() = Eigen::Vector3f(0.0f, -1.0f, -3.0f);

    /* Audio Engine Testing */
    //MCK::Audio::AudioEngine audioEngine; 

    // Initialize the audio engine - Uncomment to see the issue with fmodL.dll
    //audioEngine.Initialise();

    // Load a sound with ID 1
    //MCK::Audio::Sound sound1 = audioEngine.LoadSound("../Mackerel-Core/res/Sounds/test.mp3", 1, false, false, false);

    // Unload the sounds
    //audioEngine.UnloadSound(&sound1);

    // Teardown the audio engine
    //audioEngine.Teardown();

    /* Scene setup */
    entity->AddComponent(&transformComp);
    //entity->AddComponent(&testComp);
    entity->AddComponent(meshRenderer);
    entity->AddComponent(&rigidComp);

    floorEntity->AddComponent(&floorTransform);
    floorEntity->AddComponent(&collisionComp);
    floorEntity->AddComponent(floorRenderer);

    cameraEntity->AddComponent(cameraComponent);

    scene.InitialiseScene();
}

void DemoApp::Update()
{
    cubeInput = { 0,0 };
    cameraComponent->AspectRatio() = static_cast<float>(context->Width()) / context->Height();

    scene.UpdateScene();
}

void DemoApp::End()
{
    MCK::Input::Unsubscribe(&receipt);

    delete entity;
    delete cameraEntity;
    delete floorEntity;

    delete testMesh;
    delete testMaterial;

    delete testShader;
    delete lightShader;

    delete meshRenderer;
    delete floorRenderer;

    delete cameraComponent;
}

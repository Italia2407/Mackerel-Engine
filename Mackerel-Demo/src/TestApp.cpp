#include "TestApp.h"

#include "ShaderLibrary.h"
#include "PerspectiveCamera.h"

#include "Renderer.h"
#include "Input.h"

TestApp::TestApp() :
    m_MeshEntity(nullptr), m_Mesh(nullptr), m_Material(nullptr), m_UnlitShader(nullptr), m_MonoColourShader(nullptr), m_Camera(nullptr) {}
TestApp::~TestApp()
{}

void TestApp::MoveCameraCallback(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    Eigen::Vector3f input{};
    if (key == MCK::Key::W && ButtonEvents == MCK::ButtonEvents::Pressed)
        input.z() += 1.0f;
}

void TestApp::Start()
{
    m_MeshEntity = new MCK::EntitySystem::Entity();
    m_MeshEntity->AddComponent(&m_MeshTransform);

    m_Mesh = new MCK::AssetType::Mesh("Mesh");
    m_Mesh->LoadFromFile("../Mackerel-Core/res/Meshes/Suzanne.obj");

    m_Material = new MCK::AssetType::Material();
    m_Material->addUInt16Uniform("lightShaderID", 0);
    m_Material->addVec3Uniform("albedoColour", Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    //MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, m_UnlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT_SHADOWS, m_UnlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);

    //MCK::Rendering::Renderer::AddUnlitShader(m_UnlitShader);
    MCK::Rendering::Renderer::AddDirectionLightShader(m_UnlitShader);

    m_Camera = new MCK::EntitySystem::PerspectiveCamera(1280.0f / 720.0f);
    m_Camera->Position() = Eigen::Vector3f(0.0f, 0.0f, 3.0f);
    m_Camera->FrontDirection() = Eigen::Vector3f(0.0f, 0.0f, -1.0f);

    m_MeshTransform.Position() = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    //m_MeshTransform.Scale() = Eigen::Vector3f(1.0f, 1.0f, 0.1f);

    light = new MCK::Rendering::DirectionLight(Eigen::Vector3f(0.0, -1.0f, 0.0f), Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Eigen::Vector4f::Zero(), Eigen::Vector4f(0.1f, 0.1f, 0.1f, 1.0f));
}

void TestApp::Update()
{
    m_MeshTransform.Rotation() = m_MeshTransform.Rotation() * Eigen::AngleAxisf(0.002f, Eigen::Vector3f(0.0f, 1.0f, 0.0f));

    MCK::Rendering::Renderer::QueueMeshInstance(m_MeshTransform, m_Mesh, m_MonoColourShader, m_Material, false);
    MCK::Rendering::Renderer::UseCamera(*m_Camera);

    MCK::Rendering::Renderer::QueueDirectionLight(light);
}

void TestApp::End()
{
    delete m_Mesh;
    delete m_Material;
    delete m_Camera;
}

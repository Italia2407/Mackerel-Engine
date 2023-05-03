#include "TestJsonApp.h"

#include "ShaderLibrary.h"
#include "PerspectiveCamera.h"

#include "Renderer.h"
#include "Input.h"

#include "ComponentRegistrar.h"
#include "TestDemoComponent.h"

TestJsonApp::TestJsonApp()
{}
TestJsonApp::~TestJsonApp()
{}

void TestJsonApp::Start()
{
	RegisterComponents();
	MCK::EntitySystem::Demo::TestDemoComponent c;
	scene.InitialiseScene();
	std::string path = "../scenes/TestScene.scn";
	scene.LoadScene(path.c_str());
}

void TestJsonApp::Update()
{
	scene.UpdateScene();
}

void TestJsonApp::End()
{
	scene.UnloadScene();
}

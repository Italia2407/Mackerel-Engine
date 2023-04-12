#include "App.h"

MCK::App::App()
{}

MCK::App::~App()
{}

void MCK::App::Init()
{}

void MCK::App::Load()
{}

void MCK::App::Start()
{}

void MCK::App::Update()
{}

void MCK::App::Draw()
{}

void MCK::App::End()
{}

void MCK::App::Unload()
{}

void MCK::App::Release()
{}

void MCK::App::BeforeLoop()
{
	Init();
	Load();
	Start();
}

void MCK::App::DuringLoop()
{
	Update();
	Draw();
}

void MCK::App::AfterLoop()
{
	End();
	Unload();
	Release();
}
// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Instance.h>

#include "DemoApp.h"
#include "TestApp.h"
#include "PlatformerDemo.h"
#include "FinalDemoApp.h" 
#include "TestJsonApp.h"
#include "CacheReader.h"

int main()
{
	//MCK::Instance::Start(new DemoApp);
	//MCK::Instance::Start(new TestApp());
	MCK::Instance::Start(new PlatformerApp());
	//MCK::Instance::Start(new TestJsonApp());

	//MCK::Instance::Start(new FinalDemoApp());
}

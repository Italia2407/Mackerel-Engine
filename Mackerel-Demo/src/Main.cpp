// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Instance.h>

#include "DemoApp.h"
#include "TestApp.h"

int main()
{
	//MCK::Instance::Start(new DemoApp);
	MCK::Instance::Start(new TestApp());
}

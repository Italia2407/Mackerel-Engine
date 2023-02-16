#include "MaterialLibrary.h"

using namespace MCK;

MaterialLibrary* MaterialLibrary::instance = nullptr;

MaterialLibrary::MaterialLibrary()
{}

MaterialLibrary::~MaterialLibrary()
{}

bool MCK::MaterialLibrary::privGet(MaterialEnum asset, AssetType::Material * out)
{
	// TODO: implement this
	return false;
}

bool MCK::MaterialLibrary::privLoad(MaterialEnum asset)
{
	// TODO: implement this
	return false;
}

bool MCK::MaterialLibrary::privFree(MaterialEnum asset)
{
	// TODO: implement this
	return false;
}

void MCK::MaterialLibrary::Release()
{
	// TODO: implement this
}

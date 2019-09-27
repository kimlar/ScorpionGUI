#include "ResourceManager.h"

#include <iostream>

ResourceManager::ResourceManager(SharedContext &sharedContext) : sharedContext(sharedContext)
{
	sharedContext.resourceManager = this;

	SetCurrentDirectory();
	Load();
}

void ResourceManager::Load()
{
	//LoadFont("SourceCodePro/SourceCodePro-Regular.ttf");
	LoadFont("C:/Windows/Fonts/Arial.ttf", true);
}

//
// Font
//
void ResourceManager::LoadFont(std::string fileName, bool absolutePath)
{
	std::string filePath;
	if (absolutePath)
		filePath = fileName;
	else
		filePath = GetCurrentDirectory() + "Assets/Fonts/" + fileName;

	if (!font.loadFromFile(filePath))
	{
		std::cout << "ERROR: could not load font file." << std::endl;
	}
}

//
// CurrentDirectory
//
std::string ResourceManager::GetCurrentDirectory()
{
	return currentDirectory;
}
void ResourceManager::SetCurrentDirectory()
{
	currentDirectory = FileUtils::GetWorkingDirectory();
}

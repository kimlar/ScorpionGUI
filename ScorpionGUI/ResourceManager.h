#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "SharedContext.h"
#include "FileUtils.h"

class ResourceManager
{
public:
	ResourceManager(SharedContext &sharedContext);
	virtual ~ResourceManager() {}
private:
	void Load();

// Font
public:
	void LoadFont(std::string fileName, bool absolutePath = false);
	sf::Font &GetFont() { return font; }
private:
	sf::Font font;

// CurrentDirectory
public:
	std::string GetCurrentDirectory();
private:
	void SetCurrentDirectory();
	std::string currentDirectory;

// SharedContext
private:
	SharedContext& sharedContext;
};

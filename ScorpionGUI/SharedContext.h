#pragma once

#include <SFML/Graphics.hpp>

class Window;
class ResourceManager;
class GuiManager;

#include "GuiControl.h"

struct SharedContext
{
	SharedContext() : window(nullptr),
		renderWindow(nullptr),
		resourceManager(nullptr),
		guiManager(nullptr) {}

	// Pointer bag
	Window* window;
	sf::RenderWindow* renderWindow;
	ResourceManager* resourceManager;
	GuiManager* guiManager;

	// Getters
	Window& GetWindow() { return *window; }
	sf::RenderWindow& GetRenderWindow() { return *renderWindow; }
	ResourceManager& GetResourceManager() { return *resourceManager; }
	GuiManager& GetGuiManager() { return *guiManager; }
};

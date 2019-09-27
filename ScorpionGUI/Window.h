#pragma once

//
// TODO:
//	* Handle resizing window event: Normal, Maximize, Minimize
//	* Change: Window size
//	* Change: VSync On/Off method
//	* Change: Caption
//	* Change: Icon
//	* Change: Form background
//

#include <SFML/Graphics.hpp>

#include "SharedContext.h"

#include "Mouse.h"

class Window
{
public:
	Window(SharedContext& sharedContext, int width, int height);
	virtual ~Window();

	void CheckEvent(sf::Event &event);

	void RenderBegin();
	void RenderEnd();

	Mouse mouse;

private:
	SharedContext& sharedContext;
	sf::RenderWindow renderWindow;

	void Create(int width, int height);
};

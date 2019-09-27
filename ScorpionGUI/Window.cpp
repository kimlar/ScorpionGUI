#include "Window.h"

Window::Window(SharedContext& sharedContext, int width, int height) : sharedContext(sharedContext), mouse(sharedContext)
{
	Create(width, height);
}

Window::~Window()
{
}

void Window::Create(int width, int height)
{
	renderWindow.create(sf::VideoMode(width, height), "ScorpionGUI");
	//renderWindow.setVerticalSyncEnabled(true);

	sharedContext.renderWindow = &renderWindow;
	sharedContext.window = this;

	mouse.Load();
}

void Window::CheckEvent(sf::Event &event)
{
	if (event.type == sf::Event::Closed)
		sharedContext.GetRenderWindow().close();

	mouse.CheckEvent(event);
}

void Window::RenderBegin()
{
	renderWindow.clear(sf::Color(140, 140, 140));
}

void Window::RenderEnd()
{
	mouse.Render();
	renderWindow.display();
}

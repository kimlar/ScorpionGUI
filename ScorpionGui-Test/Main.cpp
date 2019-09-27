
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "SharedContext.h"
#include "Window.h"
#include "ResourceManager.h"

#include "Form1.h"

int main()
{
	SharedContext sharedContext;
	ResourceManager resourceManager(sharedContext);
	Window window(sharedContext, 800, 600);
	
	Form1 form1(sharedContext);
	form1.Load();
	
	while (sharedContext.GetRenderWindow().isOpen())
	{
		sf::Event event;
		while (sharedContext.GetRenderWindow().pollEvent(event))
		{
			sharedContext.GetWindow().CheckEvent(event);
			sharedContext.GetGuiManager().CheckEvent(event);
			sharedContext.GetGuiManager().Update();
		}

		sharedContext.GetWindow().RenderBegin();
		sharedContext.GetGuiManager().Render();
		sharedContext.GetWindow().RenderEnd();

		sharedContext.GetGuiManager().ClearEvents();
	}

	return 0;
}

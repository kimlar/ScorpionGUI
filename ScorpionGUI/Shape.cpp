#include "Shape.h"

#include "ResourceManager.h"
#include "GuiManager.h"

Shape::Shape(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position) : sharedContext(sharedContext), guiID(id), guiName(name), position(position)
{
	this->renderWindow = &sharedContext.GetRenderWindow();

	SetGuiControlType(GuiControlType::Shape);

	hasTabFocus = false;

	Create();
}

void Shape::CheckEvent(sf::Event & event)
{
}

void Shape::Render()
{
	//renderWindow->draw(text);
	renderWindow->draw(rectShape);
}

void Shape::Create()
{
	size = sf::Vector2f(100, 100);

	//font = &sharedContext.GetResourceManager().GetFont();
	//text.setFont(*font);
	//text.setStyle(sf::Text::Regular);
	//text.setCharacterSize(16);
	//text.setFillColor(sf::Color(0, 0, 0));
	//text.setString("This is my label:");
	//text.setPosition(position);

	SetPosition(position); //rectShape.setPosition(position);
	SetSize(size); //rectShape.setSize(sf::Vector2f(6, 6));
	
	rectShape.setFillColor(sf::Color(255,255,255));
	rectShape.setOutlineColor(sf::Color(0, 0, 0));
	rectShape.setOutlineThickness(1.0f);
}

void Shape::SetPosition(sf::Vector2f position)
{
	this->position = position;
	//text.setPosition(position);
	rectShape.setPosition(position);
}

void Shape::SetSize(sf::Vector2f size)
{
	this->size = size;
	rectShape.setSize(size);
}

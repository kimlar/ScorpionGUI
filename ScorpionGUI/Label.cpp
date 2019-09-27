#include "Label.h"

#include "ResourceManager.h"
#include "GuiManager.h"

//Label::Label(SharedContext& sharedContext, sf::Vector2f position) : sharedContext(sharedContext), position(position)
Label::Label(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position) : sharedContext(sharedContext), guiID(id), guiName(name), position(position)
{
	this->renderWindow = &sharedContext.GetRenderWindow();
	//this->id = sharedContext.GetGuiManager().AddControl();

	SetGuiControlType(GuiControlType::Label);

	hasTabFocus = false;

	Create();
}

void Label::CheckEvent(sf::Event & event)
{
}

void Label::Render()
{
	renderWindow->draw(text);
}

void Label::Create()
{
	//size = sf::Vector2f(120, 20);

	font = &sharedContext.GetResourceManager().GetFont();
	text.setFont(*font);
	text.setStyle(sf::Text::Regular);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color(0, 0, 0));
	text.setString("This is my label:");
	SetPosition(position); //text.setPosition(position);
}

void Label::SetPosition(sf::Vector2f position)
{
	this->position = position;
	text.setPosition(position);
}

void Label::SetSize(sf::Vector2f size)
{
	this->size = GetSize();
	// Can not really set size to label rectangle-wise
}

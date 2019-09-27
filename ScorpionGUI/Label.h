#pragma once

//
// Label control
//
//
// BUGS:
//	* ...
//
//
// TODO:
//	* ...
//

#include <string>

#include <SFML/Graphics.hpp>

struct SharedContext;

//#include "GuiBase.h"
#include "GuiControl.h"

//class Label : public GuiBase
class Label : public GuiControl
{
public:
	//Label(SharedContext& sharedContext, sf::Vector2f position);
	Label(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position);
	virtual ~Label() {};
	
	void CheckEvent(sf::Event& event);

	void Render();

	void GiveFocus() {}
	void RemoveFocus() {}
	
private:
	SharedContext& sharedContext;
	sf::RenderWindow* renderWindow;

	//sf::RectangleShape frameBox;

	sf::Font* font;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Create();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//public:
//	int id;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
//	void SetGuiID(int id) { guiID = id; }
	int GetGuiID() { return guiID; }
private:
	int guiID;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void SetGuiName(std::string name) { guiName = name; }
	std::string GetGuiName() { return guiName; }
private:
	std::string guiName;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	// Text
	std::string GetText() { return text.getString().toAnsiString(); }
	void SetText(std::string text) { sf::String tempStr = sf::String(text); this->text.setString(tempStr); }
	// Position
	sf::Vector2f GetPosition() { return position; }
	void SetPosition(sf::Vector2f position);
	// Size
	void SetSize(sf::Vector2f size);
	sf::Vector2f GetSize() { return sf::Vector2f(text.getLocalBounds().width, text.getLocalBounds().height); }
	// FillColor
	sf::Color GetFillColor() { return text.getFillColor(); }
	void SetFillColor(sf::Color color) { text.setFillColor(color); }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

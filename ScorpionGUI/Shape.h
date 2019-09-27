#pragma once

//
// Shape control
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

#include "GuiControl.h"

class Shape : public GuiControl
{
public:
	Shape(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position);
	virtual ~Shape() {};

	void CheckEvent(sf::Event& event);

	void Render();

	void GiveFocus() {}
	void RemoveFocus() {}

private:
	SharedContext& sharedContext;
	sf::RenderWindow* renderWindow;

	//sf::RectangleShape frameBox;

	sf::RectangleShape rectShape;
	//sf::CircleShape circShape;
	//sf::ConvexShape
	//sf::Lines
	//sf::LinesStrip
	//sf::LineStrip
	//sf::Shape
	//sf::VertexArray


	//sf::Font* font;
	//sf::Text text;

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
	//// Text
	//std::string GetText() { return text.getString().toAnsiString(); }
	//void SetText(std::string text) { sf::String tempStr = sf::String(text); this->text.setString(tempStr); }
	// Position
	sf::Vector2f GetPosition() { return position; }
	void SetPosition(sf::Vector2f position);
	// Size
	sf::Vector2f GetSize() { return size; }
	void SetSize(sf::Vector2f size);
	// FillColor
	sf::Color GetFillColor() { return rectShape.getFillColor(); }
	void SetFillColor(sf::Color color) { rectShape.setFillColor(color); }
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

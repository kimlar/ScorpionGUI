#pragma once

//
// Button control
//
//
// BUGS:
//	* ...
//
//
// TODO:
//	* Remake Button control to use sf::RenderTexture. This has the benefit to auto clip the text.
//	* Auto center text (horizontal and vertical)
//

#include <SFML/Graphics.hpp>

struct SharedContext;

#include "GuiControl.h"

class Button : public GuiControl
{
public:
	Button(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position);
	virtual ~Button() {};

	void CheckEvent(sf::Event& event);

	void Render();

	void GiveFocus();
	void RemoveFocus();

	
private:
	SharedContext& sharedContext;
	sf::RenderWindow* renderWindow;

	sf::RectangleShape frameBox;

	sf::Font* font;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;
	
	sf::Color fillColor = sf::Color(200, 200, 200);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Create();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool hasFocus;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void MouseCheckForGotFocus(int mouseX, int mouseY);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Events:
	void OnLeftClick(int mouseX, int mouseY);
	void OnLeftPressed(int mouseX, int mouseY);
	void OnLeftReleased(int mouseX, int mouseY);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	enum class ButtonState
	{
		ButtonDown = 0,
		ButtonUp,
	};
	//enum class ButtonAction
	//{
	//	None = 0,
	//	LeftClick,
	//	MiddleClick,
	//	RightClick,
	//};

	ButtonState prevButtonState = ButtonState::ButtonDown;
	ButtonState buttonState = ButtonState::ButtonDown;
	//ButtonAction buttonAction = ButtonAction::None;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//public:
//	int tabID;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//void SetGuiID(int id) { guiID = id; }
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
	sf::Vector2f GetSize() { return size; }
	void SetSize(sf::Vector2f size);
	// FillColor
	sf::Color GetFillColor() { return frameBox.getFillColor(); }
	void SetFillColor(sf::Color color) { fillColor = color; frameBox.setFillColor(color); }
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

};

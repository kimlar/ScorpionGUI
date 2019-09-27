#pragma once

#include <string>
//#include "GuiBase.h"



#include <SFML/Graphics.hpp>

#include "GuiControlEnum.h"

//class GuiControl : public GuiBase
class GuiControl
{
public:
	//virtual void CheckEvent(sf::Event& event) = 0;

	virtual void CheckEvent(sf::Event& event) = 0;

	virtual void Render() = 0;

	//virtual void SetGuiID(int id) = 0;
	virtual int GetGuiID() = 0;

	virtual void SetGuiName(std::string name) = 0;
	virtual std::string GetGuiName() = 0;

	void SetGuiControlType(GuiControlType controlType) { this->controlType = controlType; }
	GuiControlType GetGuiControlType() { return controlType; }
	GuiControlType controlType;

	virtual void SetPosition(sf::Vector2f position) = 0;
	virtual sf::Vector2f GetPosition() = 0;
	virtual void SetSize(sf::Vector2f size) = 0;
	virtual sf::Vector2f GetSize() = 0;

	virtual void GiveFocus() = 0;
	virtual void RemoveFocus() = 0;
	bool hasTabFocus = true;

private:
};

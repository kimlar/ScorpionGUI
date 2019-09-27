#include "Button.h"

#include "ResourceManager.h"
#include "GuiManager.h"

Button::Button(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position) : sharedContext(sharedContext), guiID(id), guiName(name), position(position)
{
	this->renderWindow = &sharedContext.GetRenderWindow();
	//this->tabID = sharedContext.GetGuiManager().GetTabID();

	SetGuiControlType(GuiControlType::Button);

	Create();
}

void Button::CheckEvent(sf::Event & event)
{
	if (event.type == sf::Event::Resized)
	{
		// TODO: Resized window needs to get recalculated/repositioned
		return;
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			int mx = event.mouseButton.x;
			int my = event.mouseButton.y;

			MouseCheckForGotFocus(mx, my);

			if (hasFocus)
			{
				OnLeftPressed(mx, my);
				buttonState = ButtonState::ButtonDown;
				prevButtonState = buttonState;
			}
		}
		return;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			int mx = event.mouseButton.x;
			int my = event.mouseButton.y;

			MouseCheckForGotFocus(mx, my);

			if (hasFocus)
			{
				OnLeftReleased(mx, my);
				buttonState = ButtonState::ButtonUp;
				if (buttonState != prevButtonState)
				{
					OnLeftClick(mx, my);
				}
				prevButtonState = buttonState;
			}

		}
		return;
	}

	// TODO: Press ENTER to click button
}

void Button::Render()
{
	renderWindow->draw(frameBox);
	renderWindow->draw(text);
}

void Button::GiveFocus()
{
	hasFocus = true;
}

void Button::RemoveFocus()
{
	hasFocus = false;
}

void Button::Create()
{
	size = sf::Vector2f(60, 20);
	
	font = &sharedContext.GetResourceManager().GetFont();
	text.setFont(*font);
	text.setStyle(sf::Text::Regular);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color(0, 0, 0));
	text.setString("OK");
	//text.setPosition(position + sf::Vector2f(size.x/2 - text.getLocalBounds().width/2, 0.0f));
		
	//frameBox.setPosition(position);
	//frameBox.setSize(size);
	//frameBox.setFillColor(sf::Color(200,200,200));
	frameBox.setFillColor(fillColor);
	frameBox.setOutlineColor(sf::Color(0, 0, 0));
	frameBox.setOutlineThickness(1.0f);

	SetSize(size);
	SetPosition(position);
}

void Button::MouseCheckForGotFocus(int mouseX, int mouseY)
{
	hasFocus = false;
	if (mouseX > position.x && mouseX < (position.x + frameBox.getSize().x))
	{
		if (mouseY > position.y && mouseY < (position.y + frameBox.getSize().y))
		{
			hasFocus = true;
			//sharedContext.GetGuiManager().SetFocus(this->tabID);
			sharedContext.GetGuiManager().SetFocus(guiID);

			//OnLeftPressed(mouseX, mouseY);
			
			//frameBox.setFillColor(sf::Color::Green); // TESTING PURPOSE!!!! REMOVE LATER
			return;
		}
	}
	//frameBox.setFillColor(sf::Color::Red);
}

void Button::OnLeftClick(int mouseX, int mouseY)
{
	GuiEvent guiEvent;

	guiEvent.guiEventType = GuiEvent::GuiEventType::OnMouseLeftClick;
	//guiEvent.controlID = this->tabID;
	guiEvent.controlID = guiID;

	sharedContext.GetGuiManager().guiEvents.push_back(guiEvent);
}

void Button::OnLeftPressed(int mouseX, int mouseY)
{
	GuiEvent guiEvent;

	guiEvent.guiEventType = GuiEvent::GuiEventType::OnMouseLeftPressed;
	//guiEvent.controlID = this->tabID;
	guiEvent.controlID = guiID;

	sharedContext.GetGuiManager().guiEvents.push_back(guiEvent);

	//frameBox.setFillColor(sf::Color::Red);
	//frameBox.setFillColor(sf::Color(180, 180, 180));

	sf::Color darkenedColor;
	if (fillColor.r >= 20) darkenedColor.r = fillColor.r - 20;
	if (fillColor.g >= 20) darkenedColor.g = fillColor.g - 20;
	if (fillColor.b >= 20) darkenedColor.b = fillColor.b - 20;

	frameBox.setFillColor(darkenedColor);
}

void Button::OnLeftReleased(int mouseX, int mouseY)
{
	GuiEvent guiEvent;

	guiEvent.guiEventType = GuiEvent::GuiEventType::OnMouseLeftReleased;
	//guiEvent.controlID = this->tabID;
	guiEvent.controlID = guiID;

	sharedContext.GetGuiManager().guiEvents.push_back(guiEvent);

	//frameBox.setFillColor(sf::Color::Green);
	//frameBox.setFillColor(sf::Color(200, 200, 200));
	frameBox.setFillColor(fillColor);
}

void Button::SetPosition(sf::Vector2f position)
{
	this->position = position;
	frameBox.setPosition(position);
	text.setPosition(position + sf::Vector2f(size.x / 2 - text.getLocalBounds().width / 2, 0.0f));
}

void Button::SetSize(sf::Vector2f size)
{
	this->size = size;
	frameBox.setSize(size);
	text.setPosition(position + sf::Vector2f(size.x / 2 - text.getLocalBounds().width / 2, 0.0f));
}

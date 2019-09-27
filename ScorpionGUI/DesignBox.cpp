#include "DesignBox.h"

#include "ResourceManager.h"
#include "GuiManager.h"
#include "Window.h"

DesignBox::DesignBox(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position) : sharedContext(sharedContext), guiID(id), guiName(name), position(position)
{
	this->renderWindow = &sharedContext.GetRenderWindow();

	hasTabFocus = false;

	SetGuiControlType(GuiControlType::DesignBox);

	Create();
}

void DesignBox::CheckEvent(sf::Event& event)
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

			//isGrabbing = true; // TODO: FIX LINE
			MouseCheckForGrabbing(mx, my);
		}
		return;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			int mx = event.mouseButton.x;
			int my = event.mouseButton.y;

			isDragging = false;

			isGrabbing = false; // TODO: FIX LINE
		}
		return;
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		if (isDragging)
		{
			int mx = event.mouseMove.x;// -(int)(GetSize().x) / 2;
			int my = event.mouseMove.y;// -(int)(GetSize().y) / 2;
			
			//SetPosition(sf::Vector2f((float)mx, (float)my));
			SetPosition(sf::Vector2f((float)mx - (attachSize.x / 2), (float)my - (attachSize.y / 2)));

			//printf("%s", attachControl->GetGuiName().c_str());
			switch (attachControlType)
			{
			case GuiControlType::Label:
				attachPosition = sf::Vector2f((float)mx - (attachSize.x / 2), (float)my - (attachSize.y / 2));
				GetControlLabel()->SetPosition(attachPosition);
				break;
			case GuiControlType::Button:
				attachPosition = sf::Vector2f((float)mx - (attachSize.x / 2), (float)my - (attachSize.y / 2));
				GetControlButton()->SetPosition(attachPosition);
				break;
			case GuiControlType::EditBox:
				attachPosition = sf::Vector2f((float)mx - (attachSize.x / 2), (float)my - (attachSize.y / 2));
				GetControlEditBox()->SetPosition(attachPosition);
				break;
			case GuiControlType::Shape:
				attachPosition = sf::Vector2f((float)mx - (attachSize.x / 2), (float)my - (attachSize.y / 2));
				GetControlShape()->SetPosition(attachPosition);
				break;
			}
		}

		if (isGrabbing)
		{
			int mx = event.mouseMove.x;
			int my = event.mouseMove.y;

			grabIndex = sharedContext.GetWindow().mouse.grabIndex;

			if (attachControlType == GuiControlType::EditBox)
				GetControlEditBox()->isDesignDragged = true;

			if (grabIndex == ResizeDirection::NW)
			{
				attachControl->SetSize(sf::Vector2f(position.x + size.x - mx, (position.y + size.y) - my));
				attachSize = sf::Vector2f(position.x + size.x - mx, (position.y + size.y) - my);
				size = attachSize;

				attachControl->SetPosition(sf::Vector2f((float)mx, (float)my));
				attachPosition = sf::Vector2f((float)mx, (float)my);
				position = attachPosition;

				rectShape[0].setPosition(position.x - grabSize, position.y - grabSize);
				rectShape[2].setPosition(rectShape[2].getPosition().x, rectShape[0].getPosition().y);
				rectShape[1].setPosition(rectShape[0].getPosition().x + (rectShape[2].getPosition().x - rectShape[0].getPosition().x) / 2, rectShape[0].getPosition().y);
				rectShape[5].setPosition(rectShape[0].getPosition().x, rectShape[5].getPosition().y);
				rectShape[3].setPosition(rectShape[0].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
				rectShape[4].setPosition(rectShape[4].getPosition().x, rectShape[3].getPosition().y);
				rectShape[6].setPosition(rectShape[1].getPosition().x, rectShape[5].getPosition().y);
			}
			else if (grabIndex == ResizeDirection::N)
			{
				attachControl->SetSize(sf::Vector2f(attachSize.x, (position.y + size.y) - my));
				attachSize = sf::Vector2f(attachSize.x, (position.y + size.y) - my);
				size = attachSize;

				attachControl->SetPosition(sf::Vector2f(attachPosition.x, (float)my));
				attachPosition = sf::Vector2f(attachPosition.x, (float)my);
				position = attachPosition;

				rectShape[0].setPosition(rectShape[0].getPosition().x, position.y - grabSize);
				rectShape[1].setPosition(rectShape[1].getPosition().x, position.y - grabSize);
				rectShape[2].setPosition(rectShape[2].getPosition().x, position.y - grabSize);
				rectShape[3].setPosition(rectShape[3].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
				rectShape[4].setPosition(rectShape[4].getPosition().x, rectShape[3].getPosition().y);
			}
			else if (grabIndex == ResizeDirection::NE)
			{
				attachControl->SetSize(sf::Vector2f(mx - position.x, position.y + size.y - my));
				attachSize = sf::Vector2f(mx - position.x, position.y + size.y - my);
				size = attachSize;

				attachControl->SetPosition(sf::Vector2f(attachPosition.x, (float)my));
				attachPosition = sf::Vector2f(attachPosition.x, (float)my);
				position = attachPosition;

				rectShape[0].setPosition(rectShape[0].getPosition().x, position.y - grabSize);
				rectShape[2].setPosition(mx + 1.0f, position.y - grabSize);
				rectShape[1].setPosition(rectShape[0].getPosition().x + (rectShape[2].getPosition().x - rectShape[0].getPosition().x) / 2, position.y - grabSize);
				rectShape[3].setPosition(rectShape[3].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
				rectShape[4].setPosition(mx + 1.0f, rectShape[3].getPosition().y);
				rectShape[6].setPosition(rectShape[1].getPosition().x, rectShape[6].getPosition().y);
				rectShape[7].setPosition(mx + 1.0f, rectShape[7].getPosition().y);
			}
			else if (grabIndex == ResizeDirection::W)
			{
				attachControl->SetSize(sf::Vector2f(position.x + size.x - mx, attachSize.y));
				attachSize = sf::Vector2f(position.x + size.x - mx, attachSize.y);
				size = attachSize;

				attachControl->SetPosition(sf::Vector2f((float)mx, attachPosition.y));
				attachPosition = sf::Vector2f((float)mx, attachPosition.y);
				position = attachPosition;

				rectShape[0].setPosition(position.x - grabSize, rectShape[0].getPosition().y);
				rectShape[1].setPosition(rectShape[0].getPosition().x + (rectShape[2].getPosition().x - rectShape[0].getPosition().x) / 2, rectShape[1].getPosition().y);
				rectShape[3].setPosition(rectShape[0].getPosition().x, rectShape[3].getPosition().y);
				rectShape[5].setPosition(rectShape[0].getPosition().x, rectShape[5].getPosition().y);
				rectShape[6].setPosition(rectShape[0].getPosition().x + (rectShape[7].getPosition().x - rectShape[5].getPosition().x) / 2, rectShape[6].getPosition().y);
			}
			else if (grabIndex == ResizeDirection::E)
			{
				//GetControlButton()->SetSize(sf::Vector2f(mx - position.x, attachSize.y));
				attachControl->SetSize(sf::Vector2f(mx - position.x, attachSize.y));
				
				attachSize = sf::Vector2f(mx - position.x, attachSize.y);
				size = attachSize;

				rectShape[2].setPosition(mx + 1.0f, rectShape[2].getPosition().y);
				rectShape[4].setPosition(mx + 1.0f, rectShape[4].getPosition().y);
				rectShape[7].setPosition(mx + 1.0f, rectShape[7].getPosition().y);

				rectShape[1].setPosition(rectShape[0].getPosition().x + (rectShape[2].getPosition().x - rectShape[0].getPosition().x) / 2, rectShape[1].getPosition().y);
				rectShape[6].setPosition(rectShape[5].getPosition().x + (rectShape[7].getPosition().x - rectShape[5].getPosition().x) / 2, rectShape[6].getPosition().y);
			}
			else if (grabIndex == ResizeDirection::SW)
			{
				attachControl->SetSize(sf::Vector2f(position.x + size.x - mx, my - position.y));
				attachSize = sf::Vector2f(position.x + size.x - mx, my - position.y);
				size = attachSize;

				attachControl->SetPosition(sf::Vector2f((float)mx, (float)my - attachSize.y));
				attachPosition = sf::Vector2f((float)mx, (float)my - attachSize.y);
				position = attachPosition;

				rectShape[0].setPosition(position.x - grabSize, rectShape[0].getPosition().y);
				rectShape[1].setPosition(rectShape[0].getPosition().x + (rectShape[2].getPosition().x - rectShape[0].getPosition().x) / 2, rectShape[1].getPosition().y);
				rectShape[5].setPosition(position.x - grabSize, position.y + attachSize.y);
				rectShape[3].setPosition(rectShape[0].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
				rectShape[4].setPosition(rectShape[4].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
				rectShape[7].setPosition(rectShape[7].getPosition().x, rectShape[5].getPosition().y);
				rectShape[6].setPosition(rectShape[5].getPosition().x + (rectShape[7].getPosition().x - rectShape[5].getPosition().x) / 2, rectShape[5].getPosition().y + (rectShape[7].getPosition().y - rectShape[5].getPosition().y) / 2);
			}
			else if (grabIndex == ResizeDirection::S)
			{
				//GetControlButton()->SetSize(sf::Vector2f(attachSize.x, my - position.y));
				attachControl->SetSize(sf::Vector2f(attachSize.x, my - position.y));

				attachSize = sf::Vector2f(attachSize.x, my - position.y);
				size = attachSize;

				rectShape[5].setPosition(rectShape[5].getPosition().x, my + 1.0f);
				rectShape[6].setPosition(rectShape[6].getPosition().x, my + 1.0f);
				rectShape[7].setPosition(rectShape[7].getPosition().x, my + 1.0f);

				rectShape[3].setPosition(rectShape[3].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
				rectShape[4].setPosition(rectShape[4].getPosition().x, rectShape[2].getPosition().y + (rectShape[7].getPosition().y - rectShape[2].getPosition().y) / 2);
			}
			else if (grabIndex == ResizeDirection::SE)
			{
				//GetControlButton()->SetSize(sf::Vector2f(mx - position.x, my - position.y));
				attachControl->SetSize(sf::Vector2f(mx - position.x, my - position.y));

				attachSize = sf::Vector2f(mx - position.x, my - position.y);
				size = attachSize;

				rectShape[2].setPosition(mx + 1.0f, rectShape[2].getPosition().y);
				rectShape[1].setPosition(rectShape[0].getPosition().x + (rectShape[2].getPosition().x - rectShape[0].getPosition().x) / 2, rectShape[1].getPosition().y);
				rectShape[7].setPosition(mx + 1.0f, my + 1.0f);
				rectShape[6].setPosition(rectShape[5].getPosition().x + (rectShape[7].getPosition().x - rectShape[5].getPosition().x) / 2, my + 1.0f);
				rectShape[4].setPosition(mx + 1.0f, rectShape[2].getPosition().y + (rectShape[7].getPosition().y - rectShape[2].getPosition().y) / 2);
				rectShape[5].setPosition(rectShape[5].getPosition().x, my + 1.0f);
				rectShape[3].setPosition(rectShape[3].getPosition().x, rectShape[0].getPosition().y + (rectShape[5].getPosition().y - rectShape[0].getPosition().y) / 2);
			}

		}
		else
		{
			// reset lock
			if (attachControlType == GuiControlType::EditBox && GetControlEditBox()->isDesignDragged)
			{
				GetControlEditBox()->isDesignDragged = false;
				GetControlEditBox()->SetSize(size);
			}
		}

	}
}

void DesignBox::Render()
{
	for(int i = 0; i < 8; i++)
		renderWindow->draw(rectShape[i]);
}

void DesignBox::Create()
{
	size = sf::Vector2f(100, 100);

	SetPosition(position); //rectShape.setPosition(position);
	SetSize(size); //rectShape.setSize(sf::Vector2f(6, 6));

	for (int i = 0; i < 8; i++)
	{
		rectShape[i].setFillColor(sf::Color(255, 255, 255));
		rectShape[i].setOutlineColor(sf::Color(0, 0, 0));
		rectShape[i].setOutlineThickness(1.0f);
	}
}

void DesignBox::SetPosition(sf::Vector2f position)
{
	this->position = position;
	
	rectShape[0].setPosition(position + sf::Vector2f(-7, -7)); // NW
	rectShape[1].setPosition(position + sf::Vector2f(attachSize.x / 2 - 3, -7)); // N
	rectShape[2].setPosition(position + sf::Vector2f(attachSize.x + 1, -7)); // NE
	rectShape[3].setPosition(position + sf::Vector2f(-7, attachSize.y / 2 - 3)); // W
	rectShape[4].setPosition(position + sf::Vector2f(attachSize.x + 1, attachSize.y / 2 - 3)); // E
	rectShape[5].setPosition(position + sf::Vector2f(-7, attachSize.y + 1)); // SW
	rectShape[6].setPosition(position + sf::Vector2f(attachSize.x / 2 - 3, attachSize.y + 1)); // S
	rectShape[7].setPosition(position + sf::Vector2f(attachSize.x + 1, attachSize.y + 1)); // SE
}

void DesignBox::SetSize(sf::Vector2f size)
{
	this->size = size;
	for (int i = 0; i < 8; i++)
		rectShape[i].setSize(size);
}

void DesignBox::MouseCheckForGotFocus(int mouseX, int mouseY)
{	
	if (mouseX > attachPosition.x && mouseX < (attachPosition.x + attachSize.x))
	{
		if (mouseY > attachPosition.y && mouseY < (attachPosition.y + attachSize.y))
		{
			isDragging = true;
			return;
		}
	}

	isDragging = false;
}

void DesignBox::MouseCheckForGrabbing(int mouseX, int mouseY)
{
	//isGrabbing = true;

	for (int i = 0; i < 8; i++)
	{
		if (mouseX > rectShape[i].getPosition().x && mouseX < (rectShape[i].getPosition().x + grabSize))
		{
			if (mouseY > rectShape[i].getPosition().y && mouseY < (rectShape[i].getPosition().y + grabSize))
			{
				isGrabbing = true;
				return;
			}
		}
	}

	isGrabbing = false;

}

void DesignBox::AttachToControl(std::string name)
{
	attachControl = sharedContext.GetGuiManager().guiControls[sharedContext.GetGuiManager().GetControlIdByName(name)].get();

	if (attachControl->GetGuiControlType() == GuiControlType::Label)
	{
		attachControlType = GuiControlType::Label;

		Label* label = sharedContext.GetGuiManager().CastControlToLabel(name);
		attachPosition = sf::Vector2f(label->GetPosition());
		attachSize = sf::Vector2f(label->GetSize());
	}
	if (attachControl->GetGuiControlType() == GuiControlType::Button)
	{
		attachControlType = GuiControlType::Button;

		Button* button = sharedContext.GetGuiManager().CastControlToButton(name);
		attachPosition = sf::Vector2f(button->GetPosition());
		attachSize = sf::Vector2f(button->GetSize());
	}
	if (attachControl->GetGuiControlType() == GuiControlType::EditBox)
	{
		attachControlType = GuiControlType::EditBox;

		EditBox* editBox = sharedContext.GetGuiManager().CastControlToEditBox(name);
		attachPosition = sf::Vector2f(editBox->GetPosition());
		attachSize = sf::Vector2f(editBox->GetSize());
	}
	if (attachControl->GetGuiControlType() == GuiControlType::Shape)
	{
		attachControlType = GuiControlType::Shape;

		Shape* shape = sharedContext.GetGuiManager().CastControlToShape(name);
		attachPosition = sf::Vector2f(shape->GetPosition());
		attachSize = sf::Vector2f(shape->GetSize());
	}

	SetPosition(attachPosition);
	size = attachSize;
	//SetSize(attachSize);
}

Label* DesignBox::GetControlLabel()
{
	if (attachControl->GetGuiControlType() == GuiControlType::Label)
	{
		Label* label = sharedContext.GetGuiManager().CastControlToLabel(attachControl->GetGuiName());
		return label;
	}

	return nullptr;
}

Button* DesignBox::GetControlButton()
{
	if (attachControl->GetGuiControlType() == GuiControlType::Button)
	{
		Button* button = sharedContext.GetGuiManager().CastControlToButton(attachControl->GetGuiName());
		return button;
	}

	return nullptr;
}

EditBox* DesignBox::GetControlEditBox()
{
	if (attachControl->GetGuiControlType() == GuiControlType::EditBox)
	{
		EditBox* editBox = sharedContext.GetGuiManager().CastControlToEditBox(attachControl->GetGuiName());
		return editBox;
	}

	return nullptr;
}

Shape* DesignBox::GetControlShape()
{
	if (attachControl->GetGuiControlType() == GuiControlType::Shape)
	{
		Shape* shape = sharedContext.GetGuiManager().CastControlToShape(attachControl->GetGuiName());
		return shape;
	}

	return nullptr;
}

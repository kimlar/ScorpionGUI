#include "Mouse.h"

#include <iostream>

#include "Window.h"
#include "GuiManager.h"
#include "ResourceManager.h"

Mouse::Mouse(SharedContext& sharedContext) : sharedContext(sharedContext)
{
	//SetCursor(Cursor::Arrow);
}

Mouse::~Mouse()
{
	UseSystemCursor(true);
}

void Mouse::CheckEvent(sf::Event &event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		isPressedLeft = true;
		return;
	}

	if (event.type == sf::Event::MouseButtonReleased)
	{
		isPressedLeft = false;
		return;
	}

	if (event.type == sf::Event::MouseMoved)
	{
		Cursor cursor = Cursor::Arrow;
		sprite.setOrigin(0.0f, 0.0f);
		for (auto& i : sharedContext.GetGuiManager().guiControls)
		{
			sf::Vector2f position = i->GetPosition();
			sf::Vector2f size = i->GetSize();

			if (this->position.x > position.x && this->position.x < (position.x + size.x))
			{
				if (this->position.y > position.y && this->position.y < (position.y + size.y))
				{
					if (i->controlType == GuiControlType::EditBox)
						cursor = Cursor::Edit;
					if (i->controlType == GuiControlType::DesignBox)
					{
						cursor = Cursor::Move;
						sprite.setOrigin(9.0f, 9.0f);
					}
				}
			}

			// Check resize boxes
			if (i->controlType == GuiControlType::DesignBox)
			{
				float grabSize = 7.0f;
				float grabBox[8][2] = {
					{ position.x - grabSize, position.y - grabSize },
					{ position.x + size.x / 2 - 3, position.y - grabSize },
					{ position.x + size.x + 1, position.y - grabSize },
					{ position.x - grabSize, position.y + size.y / 2 - 3 },
					{ position.x + size.x + 1, position.y + size.y /2 - 3 },
					{ position.x - grabSize, position.y + size.y + 1 },
					{ position.x + size.x / 2 - 3, position.y + size.y + 1 },
					{ position.x + size.x + 1, position.y + size.y + 1 }
				};

				float mx = this->position.x;
				float my = this->position.y;

				if(!isPressedLeft)
					grabIndex = DesignBox::ResizeDirection::NA;

				for (int i = 0; i < 8; i++)
				{
					if (mx >= grabBox[i][0] && mx <= (grabBox[i][0] + grabSize))
					{
						if (my >= grabBox[i][1] && my <= (grabBox[i][1] + grabSize))
						{
							grabIndex = (DesignBox::ResizeDirection)i;
							break;
						}
					}
				}

				if (grabIndex == DesignBox::ResizeDirection::NE || grabIndex == DesignBox::ResizeDirection::SW)
				{
					cursor = Cursor::ResizeNE;
					sprite.setOrigin(8.0f, 8.0f);
				}
				if (grabIndex == DesignBox::ResizeDirection::N || grabIndex == DesignBox::ResizeDirection::S)
				{
					cursor = Cursor::ResizeNS;
					sprite.setOrigin(4.0f, 8.0f);
				}
				if (grabIndex == DesignBox::ResizeDirection::NW || grabIndex == DesignBox::ResizeDirection::SE)
				{
					cursor = Cursor::ResizeNW;
					sprite.setOrigin(4.0f, 8.0f);
				}
				if (grabIndex == DesignBox::ResizeDirection::W || grabIndex == DesignBox::ResizeDirection::E)
				{
					cursor = Cursor::ResizeWE;
					sprite.setOrigin(8.0f, 4.0f);
				}
			}
		}

		SetMouseCursor(cursor);

		Update(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
	}

	if (event.type == sf::Event::MouseLeft)
		show = false;

	if (event.type == sf::Event::MouseEntered)
		show = true;
}

void Mouse::UseSystemCursor(bool use)
{
	sharedContext.GetRenderWindow().setMouseCursorVisible(use);
}

void Mouse::Update(sf::Vector2i position)
{
	this->position = sf::Vector2f((float)position.x, (float)position.y);
	sprite.setPosition(this->position);
}

void Mouse::Render()
{
	if(show)
		sharedContext.GetRenderWindow().draw(sprite);
}

void Mouse::Load()
{
	UseSystemCursor(false);
	LoadMouseCursor("Resources/Cursors.png");
}

void Mouse::LoadMouseCursor(std::string file)
{
	texCoords.push_back(sf::IntRect(1, 1, 11, 18)); // Arrow
	texCoords.push_back(sf::IntRect(13, 1, 7, 18)); // Edit
	texCoords.push_back(sf::IntRect(21, 1, 19, 19)); // Move
	texCoords.push_back(sf::IntRect(41, 1, 7, 19)); // ResizeNS
	texCoords.push_back(sf::IntRect(49, 1, 15, 15)); // ResizeNE
	texCoords.push_back(sf::IntRect(65, 1, 19, 7)); // ResizeWE
	texCoords.push_back(sf::IntRect(85, 1, 15, 15)); // ResizeNW
		
	//if(!texture.loadFromFile(FileUtils::GetWorkingDirectory() + file))
	if (!texture.loadFromFile(sharedContext.GetResourceManager().GetCurrentDirectory() + file))
		std::cout << "Error loading mouse texture" << std::endl;
		
	sprite.setTexture(texture);
	sprite.setTextureRect(texCoords[(int)cursor]);

	SetMouseCursor(Cursor::Arrow);
}

void Mouse::SetMouseCursor(Cursor cursor)
{
	this->cursor = cursor;
	if(texCoords.size() > 0)
		sprite.setTextureRect(texCoords[(int)cursor]);
}

#include "Image.h"

#include "ResourceManager.h"
#include "GuiManager.h"

Image::Image(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position) : sharedContext(sharedContext), guiID(id), guiName(name), position(position)
{
		this->renderWindow = &sharedContext.GetRenderWindow();

		SetGuiControlType(GuiControlType::Image);

		hasTabFocus = false;

		this->position = position;
		this->size = sf::Vector2f(0.0f, 0.0f);

		SetPosition(position);
		SetSize(size);
}

void Image::CheckEvent(sf::Event& event)
{
	if (event.type == sf::Event::EventType::Resized)
	{
		// TODO: Implement window resizing event. Do we need to resize image?
	}
}

void Image::Render()
{
	renderWindow->draw(sprite);
}

void Image::LoadFile(std::string name)
{
	texture.loadFromFile( sharedContext.GetResourceManager().GetCurrentDirectory() + name);
	sprite.setTexture(texture);

	//size = sf::Vector2f(100, 100);
	size = sf::Vector2f((float)texture.getSize().x, (float)texture.getSize().y);
	
	//SetPosition(position); //rectShape.setPosition(position);
	//SetSize(size); //rectShape.setSize(sf::Vector2f(6, 6));
}

void Image::SetPosition(sf::Vector2f position)
{
	this->position = position;
	sprite.setPosition(position);
}

void Image::SetSize(sf::Vector2f size)
{
	this->size = size;
	sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
}

#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "SharedContext.h"

#include "DesignBox.h"

class Mouse
{
public:
	enum class Cursor
	{
		Arrow = 0,
		Edit,
		Move,
		ResizeNS,
		ResizeNE,
		ResizeWE,
		ResizeNW,
		//Cross,
		//Pen,
		//Wait,
		//HandOpen,
		//HandClose,
	};

	Mouse(SharedContext& sharedContext);
	virtual ~Mouse();

	void CheckEvent(sf::Event &event);

	void Update(sf::Vector2i position);
	void Render();
		
	void Load();
	void SetMouseCursor(Cursor cursor);

	DesignBox::ResizeDirection grabIndex = DesignBox::ResizeDirection::NA;

private:
	void LoadMouseCursor(std::string file);
	void UseSystemCursor(bool use);

	SharedContext& sharedContext;
	
	std::vector<sf::IntRect> texCoords;
	sf::Texture texture;
	sf::Sprite sprite;
	
	Cursor cursor;
	sf::Vector2f position;
	bool show;

	bool isPressedLeft;
};

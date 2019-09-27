#pragma once

//
// EditBox control
//
//
// BUGS:
//	* Maximize Window bug
//	* Sometimes the text cursor gets misplaced some pixel to the right.
//
//
// TODO:
//	* Refine member functions (long term goal)
//
//
// MAYBE:
//	* Redo: Ctrl+Y (at least single step)
//	* Mouse: Right-click menu: Copy/Paste/Cut
//	* Keyboard: Insert to change between insert text or override text mode
//

#include <SFML/Graphics.hpp>

#include <vector>

struct SharedContext;

#include "GuiControl.h"

class EditBox : public GuiControl
{
public:
	EditBox(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position);
	virtual ~EditBox() {};

	void CheckEvent(sf::Event& event);

	void Render();

	void GiveFocus();
	void RemoveFocus();

private:
	SharedContext& sharedContext;
	sf::RenderWindow* renderWindow;
	
	sf::RenderTexture renderTexture;
	sf::Sprite sprite;

	sf::RectangleShape frameBox;
	sf::RectangleShape outerBox;
	sf::RectangleShape innerBox;
	sf::RectangleShape cursorBox;
	sf::RectangleShape selectBox;

	sf::Font* font;
	sf::Text text;

	sf::Vector2f position;
	sf::Vector2f size;

	sf::Vector2f outerPosition;
	sf::Vector2f innerPosition;
	sf::Vector2f textPosition;
	sf::Vector2f cursorPosition;
	sf::Vector2f selectStartPosition;
	sf::Vector2f selectEndPosition;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Create();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	int innerCharLoc;
	int cursorCharLoc;
	int selectStartCharLoc;
	int selectEndCharLoc;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void InnerMoveLeft();
	void InnerMoveRight();

	void CursorMoveLeft();
	void CursorMoveRight();

	int GetTextCount();
	float GetCursorX();
	float GetInnerX();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Glyph
	void AddGlyph(char glyph);
	void RemoveGlyph();
	void RemoveGlyphPost();

	// Update
	void ResizeOuterBox();

	void UpdateVirtualPosition();
	sf::Vector2f virtualPosition;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void JumpToStartOfLine(); // Home-key
	void JumpToEndOfLine(); // End-key

	void JumpToPrevWord(); // Ctrl+Left
	void JumpToNextWord(); // Ctrl+Right

	bool IsSplitCharacter(char ch);
	int FindSplitCharacter(std::string str);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool isSelected;

	void StartSelecting();
	void UpdateSelection();
	void DeleteSelection();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool isControlKey;

	void ClipboardCopy();
	void ClipboardPaste();
	void ClipboardCut();

	void InsertText(sf::String str);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	struct UndoStruct
	{
		std::string undoString;
		int cursorCharLoc;
		sf::Vector2f cursorPosition;
		int innerCharLoc;
		sf::Vector2f innerPosition;
	};
		
	std::vector<UndoStruct> undoData;

	void Undo();

	void AddUndo();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool hasFocus;
	bool hadFocus;
	bool allowTabChar = false; // Property: Default=false

	void SetFocus();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	bool isDragging;

	void MouseCheckForGotFocus(int mouseX, int mouseY);
	void MouseCheckForCursorLocation(int mouseX);
	void MouseCheckForDragScroll(int mouseX);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//public:
//	int tabID;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
	sf::Color GetFillColor() { return innerBox.getFillColor(); }
	void SetFillColor(sf::Color color) { innerBox.setFillColor(color); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	bool isDesignDragged = false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

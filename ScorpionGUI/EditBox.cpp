#include "EditBox.h"

#include <windows.h>

#include "ResourceManager.h"
#include "GuiManager.h"
#include "Window.h"
//#include "Mouse.h"

EditBox::EditBox(SharedContext& sharedContext, int id, std::string name, sf::Vector2f position) : sharedContext(sharedContext), guiID(id), guiName(name), position(position)
{
	this->renderWindow = &sharedContext.GetRenderWindow();
	//this->tabID = sharedContext.GetGuiManager().GetTabID();

	SetGuiControlType(GuiControlType::EditBox);

	Create();
}

void EditBox::Create()
{
	size = sf::Vector2f(200, 20);

	sf::Vector2f innerSize = sf::Vector2f(200, 20);
	sf::Vector2f cursorSize = sf::Vector2f(1, 20);
	sf::Vector2f selectSize = sf::Vector2f(0, 20);

	//renderTexture.create((int)innerSize.x, (int)innerSize.y);
	//sprite.setPosition(position);
	
	font = &sharedContext.GetResourceManager().GetFont();
	text.setFont(*font);
	text.setStyle(sf::Text::Regular);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color(0, 0, 0));
	text.setString("");
	AddUndo();

	//frameBox.setPosition(position + sf::Vector2f(-1.0f, -1.0f));
	//frameBox.setSize(innerSize + sf::Vector2f(2.0f, 2.0f));
	frameBox.setFillColor(sf::Color::Black);

	cursorBox.setSize(cursorSize);
	cursorBox.setFillColor(sf::Color::Black);

	outerBox.setSize(sf::Vector2f(text.getLocalBounds().width, 20));

	//innerBox.setSize(innerSize);
	innerBox.setFillColor(sf::Color::White);

	selectBox.setSize(selectSize);
	selectBox.setFillColor(sf::Color(50,150,255));

	SetSize(size);
	SetPosition(position);
}

void EditBox::CheckEvent(sf::Event& event)
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
			MouseCheckForCursorLocation(mx);
			if (hasFocus)
			{
				isDragging = true;
				isSelected = true;
				StartSelecting();
			}
			else
			{
				isSelected = false;
			}

			UpdateVirtualPosition();
		}
		return;
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
	{
		if (allowTabChar)
		{
			if (!hasFocus)
				SetFocus();
		}
		else
		{
			if (!hasFocus)
				SetFocus();
			else
				RemoveFocus();
		}
		return;
	}

	if (!hasFocus)
		return;
	
	if (event.type == sf::Event::MouseMoved)
	{
		/*
		int mx = event.mouseMove.x;
		int my = event.mouseMove.y;
		bool cursorEdit = false;
		if (mx > position.x && mx < (position.x + frameBox.getSize().x))
		{
			if (my > position.y && my < (position.y + frameBox.getSize().y))
			{
				cursorEdit = true;
			}
		}
		if (cursorEdit)
			sharedContext.GetWindow().mouse.SetCursor(Mouse::Cursor::Edit);
		else
			sharedContext.GetWindow().mouse.SetCursor(Mouse::Cursor::Arrow);
		*/
		
		if (isDragging)
		{
			int mouseDragX = event.mouseMove.x;
			MouseCheckForCursorLocation(mouseDragX);
			UpdateSelection();
			MouseCheckForDragScroll(mouseDragX);
		}
		return;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		isDragging = false;
		return;
	}
	
	if (event.type == sf::Event::KeyPressed && event.text.unicode < 128) // letters (if selected remove selection)
	{
		if (!event.key.control && !event.key.shift)
		{
			if (isSelected)
			{
				DeleteSelection();
				isSelected = false;
				return;
			}
		}
	}
	
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Delete)
		{
			if (isSelected)
				DeleteSelection();
			else
				RemoveGlyphPost();
			AddUndo();
			UpdateVirtualPosition();
		}
		else if (event.key.code == sf::Keyboard::BackSpace)
		{
			if (isSelected)
				DeleteSelection();
			else
				RemoveGlyph();
			AddUndo();
			UpdateVirtualPosition();
		}
		else if (event.key.code == sf::Keyboard::C)
		{
			if (event.key.control)
				ClipboardCopy();
		}
		else if (event.key.code == sf::Keyboard::V)
		{
			if (event.key.control)
			{
				ClipboardPaste();
				AddUndo();
				UpdateVirtualPosition();
			}
		}
		else if (event.key.code == sf::Keyboard::X)
		{
			if (event.key.control)
			{
				ClipboardCut();
				AddUndo();
				UpdateVirtualPosition();
			}
		}
		else if (event.key.code == sf::Keyboard::Insert)
		{
			if (event.key.control)
				ClipboardCopy();
			else if (event.key.shift)
			{
				ClipboardPaste();
				AddUndo();
				UpdateVirtualPosition();
			}
		}
		else if (event.key.code == sf::Keyboard::Z)
		{
			if (event.key.control)
			{
				Undo();
				UpdateVirtualPosition();
			}
		}

		if (event.key.shift)
		{
			if (!isSelected)
			{
				isSelected = true;
				StartSelecting();
			}
		}
		else
		{
			if (!event.key.control)
				isSelected = false;
		}

		if (event.key.control)
			isControlKey = true;
		else
			isControlKey = false;

		if (event.key.code == sf::Keyboard::Left)
		{
			if (event.key.control)
			{
				JumpToPrevWord();
				UpdateVirtualPosition();
			}
			else
			{
				CursorMoveLeft();
				if (cursorPosition.x < innerPosition.x)
					InnerMoveLeft();
				UpdateVirtualPosition();
			}

			if (isSelected)
				UpdateSelection();
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			if (event.key.control)
			{
				JumpToNextWord();
				UpdateVirtualPosition();
			}
			else
			{
				CursorMoveRight();
				if (cursorPosition.x > (innerPosition.x + innerBox.getLocalBounds().width))
					InnerMoveRight();
				UpdateVirtualPosition();
			}

			if (isSelected)
				UpdateSelection();
		}
		else if (event.key.code == sf::Keyboard::Home)
		{
			JumpToStartOfLine();
			UpdateVirtualPosition();

			if (isSelected)
				UpdateSelection();
		}
		else if (event.key.code == sf::Keyboard::End)
		{
			JumpToEndOfLine();
			UpdateVirtualPosition();

			if (isSelected)
				UpdateSelection();
		}
	}
	else if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == '\b') // backspace
		{
			return; // don't print character
		}
		else if (event.text.unicode == 9) // tab
		{
			return; // don't print character
		}
		else if (event.text.unicode == 13) // return
		{
			return; // Ohh the irony! :D
		}
		else if (event.text.unicode == 27) // escape
		{
			return; // don't print character
		}
		else if (event.text.unicode < 128) // letters
		{
			if (!isControlKey)
			{
				if (hadFocus) // tab
				{
					hadFocus = false;
					return;
				}

				AddGlyph((char)event.text.unicode);
				AddUndo();
				UpdateVirtualPosition();
			}
		}
	}
}

void EditBox::Render()
{
	renderWindow->draw(frameBox);

	renderTexture.clear(sf::Color(120,120,120));
	renderTexture.draw(innerBox);
	if(isSelected)
		renderTexture.draw(selectBox);
	renderTexture.draw(text);
	if (hasFocus)
		renderTexture.draw(cursorBox);
	renderTexture.display();

	sprite.setTexture(renderTexture.getTexture());
	renderWindow->draw(sprite);
}

void EditBox::InnerMoveLeft()
{
	if (innerPosition.x > outerPosition.x)
	{
		sf::Vector2f tempPosition = innerPosition;

		for (int i = innerCharLoc; i >= 0; i--)
		{
			innerCharLoc--;
			if (innerCharLoc < 0)
				innerCharLoc = 0;

			innerPosition = sf::Vector2f(GetInnerX(), 0.0f);

			if (innerPosition.x < tempPosition.x)
			{
				virtualPosition = sf::Vector2f(virtualPosition.x - (innerPosition.x - tempPosition.x), virtualPosition.y);
				break;
			}
		}
	}
}

void EditBox::InnerMoveRight()
{
	if ((innerPosition.x + innerBox.getLocalBounds().width) < (outerPosition.x + outerBox.getLocalBounds().width))
	{
		sf::Vector2f tempPosition = innerPosition;

		int textCount = GetTextCount();
		for (int i = innerCharLoc; i < (textCount + 1); i++)
		{
			innerCharLoc++;
			if (innerCharLoc > textCount)
				innerCharLoc = textCount;

			innerPosition = sf::Vector2f(GetInnerX(), 0.0f);
			float tempPos = innerPosition.x;
			
			if (innerPosition.x > (tempPosition.x + innerBox.getLocalBounds().width))
			{
				innerPosition = sf::Vector2f(tempPos - innerBox.getLocalBounds().width, innerPosition.y);

				virtualPosition = sf::Vector2f(virtualPosition.x - ((tempPos - innerBox.getLocalBounds().width) - tempPosition.x), virtualPosition.y);
				break;
			}
		}
	}
}

void EditBox::CursorMoveLeft()
{
	cursorCharLoc--;
	if (cursorCharLoc < 0)
		cursorCharLoc = 0;

	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);
}

void EditBox::CursorMoveRight()
{
	int textCount = GetTextCount();
	
	cursorCharLoc++;
	if (cursorCharLoc > textCount)
		cursorCharLoc = textCount;

	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);
}

int EditBox::GetTextCount()
{
	return (int)text.getString().toAnsiString().size();
}

float EditBox::GetCursorX()
{	
	sf::String tempStr = text.getString();
	text.setString(text.getString().substring(0, cursorCharLoc));
	float tempX = text.getLocalBounds().width;
	text.setString(tempStr);
	return tempX;
}

float EditBox::GetInnerX()
{
	sf::String tempStr = text.getString();
	text.setString(text.getString().substring(0, innerCharLoc));
	float tempX = text.getLocalBounds().width;
	text.setString(tempStr);
	return tempX;
}

void EditBox::AddGlyph(char glyph)
{
	sf::String tempStr = text.getString();
	tempStr.insert(cursorCharLoc, glyph);
	text.setString(tempStr);

	ResizeOuterBox();

	CursorMoveRight();
	if (cursorPosition.x > (innerPosition.x + innerBox.getLocalBounds().width))
		InnerMoveRight();
}

void EditBox::RemoveGlyph()
{
	sf::String tempStr = text.getString();

	if (cursorCharLoc > 0)
	{
		tempStr.erase(cursorCharLoc - 1);
		text.setString(tempStr);

		ResizeOuterBox();

		CursorMoveLeft();
		if (cursorPosition.x < innerPosition.x)
			InnerMoveLeft();
	}
}

void EditBox::RemoveGlyphPost()
{
	if (text.getString().getSize() > 0)
	{
		sf::String tempStr = text.getString();
		tempStr.erase(cursorCharLoc);
		text.setString(tempStr);

		ResizeOuterBox();
	}
}

void EditBox::ResizeOuterBox()
{
	outerBox.setSize(sf::Vector2f(text.getLocalBounds().width, 20));
}

void EditBox::UpdateVirtualPosition()
{
	outerBox.setPosition(outerPosition + virtualPosition);
	innerBox.setPosition(innerPosition + virtualPosition);
	text.setPosition(textPosition + virtualPosition);
	cursorBox.setPosition(cursorPosition + virtualPosition);
	selectBox.setPosition(selectStartPosition + virtualPosition);
}

void EditBox::JumpToStartOfLine()
{
	cursorCharLoc = 0;
	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

	innerCharLoc = 0;
	innerPosition = sf::Vector2f(0.0f, innerPosition.y);

	virtualPosition = sf::Vector2f(0.0f, virtualPosition.y);
}

void EditBox::JumpToEndOfLine()
{
	int textCount = GetTextCount();

	cursorCharLoc = textCount;
	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

	if (outerBox.getLocalBounds().width > innerBox.getLocalBounds().width)
	{

		innerCharLoc = textCount;
		innerPosition = sf::Vector2f(GetInnerX(), innerPosition.y);

		virtualPosition = sf::Vector2f(-innerPosition.x, virtualPosition.y);

		sf::Vector2f tempPosition = innerPosition;

		for (int i = innerCharLoc; i >= 0; i--)
		{
			innerCharLoc--;
			if (innerCharLoc < 0)
				innerCharLoc = 0;

			innerPosition = sf::Vector2f(GetInnerX(), 0.0f);

			if (innerPosition.x < (tempPosition.x - innerBox.getLocalBounds().width))
			{
				virtualPosition = sf::Vector2f(virtualPosition.x - (innerPosition.x - tempPosition.x), virtualPosition.y);

				InnerMoveRight();
				InnerMoveRight();
				break;
			}
		}
	}
}

void EditBox::JumpToPrevWord()
{
	int textCount = GetTextCount();
	int tempCharLoc = cursorCharLoc;

	for (int i = cursorCharLoc-1; i > 0; i--)
	{

		if (IsSplitCharacter(text.getString()[i-1]))
		{
			cursorCharLoc = i;
			if (cursorCharLoc < 0)
				cursorCharLoc = 0;
			break;
		}
	}

	if (tempCharLoc == cursorCharLoc)
		cursorCharLoc = 0;

	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

	if (cursorPosition.x < innerPosition.x)
	{
		if (FindSplitCharacter(text.getString().toAnsiString()) > cursorCharLoc)
		{
			JumpToStartOfLine();
			return;
		}

		for (int i = cursorCharLoc; i >= 0; i--)
		{
			if (cursorPosition.x > innerPosition.x)
				break;

			InnerMoveLeft();
		}
	}
}

void EditBox::JumpToNextWord()
{
	int textCount = GetTextCount();
	int tempCharLoc = cursorCharLoc;

	for (int i = cursorCharLoc; i < textCount; i++)
	{
		if (IsSplitCharacter(text.getString()[i]))
		{
			cursorCharLoc = i + 1;
			if (cursorCharLoc > textCount)
				cursorCharLoc = textCount;
			break;
		}
	}

	if (tempCharLoc == cursorCharLoc)
		cursorCharLoc = textCount;
	
	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

	for (int i = cursorCharLoc; i < (textCount+1); i++)
	{
		if (cursorPosition.x < (innerPosition.x + innerBox.getLocalBounds().width))
			break;
		
		std::string tempStr = text.getString().toAnsiString().substr(i);
		if (FindSplitCharacter(tempStr) == -1) // did not find split char
		{
			JumpToEndOfLine();
			break;
		}
	
		InnerMoveRight();
	}
}

bool EditBox::IsSplitCharacter(char ch)
{
	if ((ch >= 65 && ch <= 90) ||	// A-Z
		(ch >= 97 && ch <= 122) ||	// a-z
		(ch >= 48 && ch <= 57) ||	// 0-9
		(ch == 95))					// _
	{
		return false; // found valid ASCII char (non-split separator)
	}

	return true; // found split separator char
}

int EditBox::FindSplitCharacter(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		char tempChar = str[i];

		if (IsSplitCharacter(tempChar))
			return i; // found split separator char at location i
	}
	return -1; // did not find any split char
}

void EditBox::StartSelecting()
{
	selectStartCharLoc = cursorCharLoc;
	selectEndCharLoc = cursorCharLoc;

	selectStartPosition = cursorPosition;

	selectBox.setPosition(selectStartPosition);
	selectBox.setSize(sf::Vector2f(0.0f, selectBox.getSize().y));
}

void EditBox::UpdateSelection()
{
	selectEndCharLoc = cursorCharLoc;

	selectBox.setSize(sf::Vector2f(cursorPosition.x - selectStartPosition.x, selectBox.getSize().y));
}

void EditBox::DeleteSelection()
{
	if (isSelected)
	{
		if (selectStartCharLoc > selectEndCharLoc)
		{
			int tempCharLoc = selectStartCharLoc;
			selectStartCharLoc = selectEndCharLoc;
			selectEndCharLoc = tempCharLoc;

			cursorCharLoc = selectEndCharLoc;
			cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

			UpdateVirtualPosition();
		}

		sf::String firstStr = text.getString().substring(0, selectStartCharLoc);
		sf::String lastStr = text.getString().substring(selectEndCharLoc);

		int selectLen = (int)(text.getString().getSize() - (firstStr.getSize() + lastStr.getSize()));
		for (int i = 0; i < selectLen; i++)
			RemoveGlyph();
	}
}

void EditBox::ClipboardCopy()
{
	if (isSelected)
	{
		if (selectStartCharLoc > selectEndCharLoc)
		{
			int tempCharLoc = selectStartCharLoc;
			selectStartCharLoc = selectEndCharLoc;
			selectEndCharLoc = tempCharLoc;

			cursorCharLoc = selectEndCharLoc;
			cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

			UpdateVirtualPosition();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Win32 code blob:
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//

		std::string tempStr = text.getString().substring(selectStartCharLoc, selectEndCharLoc - selectStartCharLoc);
		const char* output = tempStr.c_str();
		const size_t len = strlen(output) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), output, len);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
		GlobalFree(hMem);

		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// End of code blob
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}

void EditBox::ClipboardPaste()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Win32 code blob:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//

	std::string textStr;

	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		//std::cout << "ERROR: Could not open clipboard" << std::endl;
		return;
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{
		//std::cout << "ERROR: Could not get clipboard data" << std::endl;
		return;
	}

	// Lock the handle to get the actual text pointer
	char* sText = static_cast<char*>(GlobalLock(hData));
	if (sText == nullptr)
	{
		//std::cout << "ERROR: Could not get global lock for clipboard" << std::endl;
		return;
	}
	textStr = sText; // Save text in a string

	GlobalUnlock(hData); // Release the lock
	CloseClipboard(); // Release the clipboard

	//
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// End of code blob
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	InsertText(textStr);
}

void EditBox::ClipboardCut()
{
	ClipboardCopy();
	DeleteSelection();
	isSelected = false;
}

void EditBox::InsertText(sf::String str)
{
	sf::String tempStr = text.getString();
	tempStr.insert(cursorCharLoc, str);
	text.setString(tempStr);

	cursorCharLoc += (int)str.getSize();
	cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

	ResizeOuterBox();

	for (int i = 0; tempStr.getSize(); i++)
	{
		if (cursorPosition.x > (innerPosition.x + innerBox.getLocalBounds().width))
			InnerMoveRight();
		else
			break;
	}
}

void EditBox::Undo()
{
	if (undoData.size() > 1)
		undoData.pop_back();

	text.setString(sf::String(undoData.back().undoString));

	ResizeOuterBox();

	cursorCharLoc = undoData.back().cursorCharLoc;
	cursorPosition = undoData.back().cursorPosition;

	innerCharLoc = undoData.back().innerCharLoc;
	innerPosition = undoData.back().innerPosition;

	virtualPosition = sf::Vector2f(-innerPosition.x, virtualPosition.y);
}

void EditBox::AddUndo()
{
	UndoStruct undo;
	undo.undoString = text.getString().toAnsiString();
	undo.cursorCharLoc = cursorCharLoc;
	undo.cursorPosition = cursorPosition;
	undo.innerCharLoc = innerCharLoc;
	undo.innerPosition = innerPosition;

	undoData.push_back(undo);
}

void EditBox::GiveFocus()
{
	hasFocus = true;
}

void EditBox::RemoveFocus()
{
	hasFocus = false;
	hadFocus = false;
}

void EditBox::SetFocus()
{
	hasFocus = true;
	hadFocus = true;
}

void EditBox::MouseCheckForGotFocus(int mouseX, int mouseY)
{
	hasFocus = false;
	if (mouseX > position.x && mouseX < (position.x + innerBox.getSize().x))
	{
		if (mouseY > position.y && mouseY < (position.y + innerBox.getSize().y))
		{
			hasFocus = true;
			//sharedContext.GetGuiManager().SetFocus(this->tabID);
			sharedContext.GetGuiManager().SetFocus(guiID);
		}
	}
}

void EditBox::MouseCheckForCursorLocation(int mouseX)
{
	if (hasFocus)
	{
		sf::Vector2f mousePosition = sf::Vector2f((float)mouseX - position.x + innerPosition.x, 0.0);

		cursorCharLoc = 0;
		cursorPosition = sf::Vector2f(GetCursorX(), 0.0f);

		int textCount = GetTextCount();
		for (int i = 0; i < textCount; i++)
		{
			CursorMoveRight();
			if (cursorPosition.x > mousePosition.x)
			{
				CursorMoveLeft();
				break;
			}
		}
		UpdateVirtualPosition();
	}
}

void EditBox::MouseCheckForDragScroll(int mouseX)
{
	sf::Vector2f mousePosition = sf::Vector2f((float)mouseX - position.x + innerPosition.x, 0.0);

	if (mousePosition.x > (innerPosition.x + innerBox.getLocalBounds().width))
		InnerMoveRight();
	else if (mousePosition.x < innerPosition.x)
		InnerMoveLeft();
	
	UpdateVirtualPosition();
}

void EditBox::SetPosition(sf::Vector2f position)
{
	this->position = position;
	sprite.setPosition(position);
	frameBox.setPosition(position + sf::Vector2f(-1.0f, -1.0f));
}

void EditBox::SetSize(sf::Vector2f size)
{
	this->size = size;
	//sprite.setSize(size);
	sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));

	innerBox.setSize(size);	
	frameBox.setSize(size + sf::Vector2f(2.0f, 2.0f));

	if (!isDesignDragged)
	{
		renderTexture.clear();
		renderTexture.create((int)size.x, (int)size.y);
	}
}

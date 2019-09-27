#pragma once

class GuiEvent
{
public:
	enum class GuiEventType
	{
		//Error = -1,
		Unknown = 0,
		OnMouseLeftClick,
		OnMouseLeftPressed,
		OnMouseLeftReleased,
		//OnMouseLeftDrag,
		OnMouseMove,
		//OnKeyboardPressEnter,
		//OnWindowLoseFocus,
		//OnWindowGainFocus,
	};

	GuiEventType guiEventType;
	int controlID;

private:
};


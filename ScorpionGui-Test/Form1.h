#pragma once

#include "GuiManager.h"
#include "GuiProperty.h"

class Form1 : public GuiManager
{
public:
	Form1(SharedContext &sharedContext) : GuiManager(sharedContext) {}

	void Load();
	void UpdateControls(GuiEvent guiEvent);
};

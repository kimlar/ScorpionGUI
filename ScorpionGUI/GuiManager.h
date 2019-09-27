#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "SharedContext.h"
#include "GuiControl.h"
#include "GuiControlEnum.h"
#include "GuiEvent.h"
#include "GuiProperty.h"

#include "Label.h"
#include "Button.h"
#include "EditBox.h"
#include "Shape.h"
#include "Image.h"
#include "DesignBox.h"

class GuiManager
{
public:
	GuiManager(SharedContext &sharedContext);
	virtual ~GuiManager() {}
	
	//void Load();

	void CheckEvent(sf::Event& event);
	
	void Render();
	void Update();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// Visual components (without event handling)
	//std::vector<std::unique_ptr<GuiBase>> guiVisuals;

	// Controls components
	std::vector<std::unique_ptr<GuiControl>> guiControls;
	
	// Focus
	int curFocus = 0;
	void NextFocus()
	{
		curFocus++;
		if (curFocus >= (int)guiControls.size())
			curFocus = 0; // go back to first control
	}
	void PrevFocus()
	{
		curFocus--;
		if (curFocus < 0)
			curFocus = (int)guiControls.size() -1; // go to last control
	}
	int GetTabID()
	{
		if ((int)guiControls.size() > 0)
			guiControls[0]->GiveFocus();
		return (int)guiControls.size();
	}
	void SetFocus(int controlID)
	{
		curFocus = controlID;

		for (auto& i : guiControls)
			i->RemoveFocus();
		guiControls[curFocus]->GiveFocus();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Events:
	std::vector<GuiEvent> guiEvents;
	void ClearEvents() { guiEvents.clear(); }
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	SharedContext& sharedContext;

private:
	std::unordered_map<std::string, int> controlName;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void AddControl(GuiControlType control, std::string name, sf::Vector2f position);
	int GetControlIdByName(std::string name);
	bool CheckForControlEvent(GuiEvent& event, std::string name, GuiEvent::GuiEventType type);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	Label* CastControlToLabel(std::string name);
	Button* CastControlToButton(std::string name);
	EditBox* CastControlToEditBox(std::string name);
	Shape* CastControlToShape(std::string name);
	Image* CastControlToImage(std::string name);
	DesignBox* CastControlToDesignBox(std::string name);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	virtual void Load() = 0;
	virtual void UpdateControls(GuiEvent guiEvent) = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void SetProperty(std::string control, GuiProperty property, int data);
	void SetProperty(std::string control, GuiProperty property, std::string data);
	void SetProperty(std::string control, GuiProperty property, sf::Vector2f data);
	void SetProperty(std::string control, GuiProperty property, sf::Color data);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void SetFormTitle(std::string title) { sharedContext.renderWindow->setTitle(title); }
	//void SetFormPosition ...
	void SetFormSize(int width, int height) { sharedContext.renderWindow->setSize(sf::Vector2u(width, height)); }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

#include "GuiManager.h"

#include <stdexcept>      // std::out_of_range

GuiManager::GuiManager(SharedContext &sharedContext) : sharedContext(sharedContext)
{
	sharedContext.guiManager = this;
}

void GuiManager::CheckEvent(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
	{
		if (event.key.shift)
			PrevFocus();
		else
			NextFocus();
		SetFocus(curFocus);
		return;
	}

	for (auto& i : guiControls)
		i->CheckEvent(event);
}

void GuiManager::Render()
{
	for (auto& i : guiControls)
		i->Render();
}

void GuiManager::Update()
{
	if (sharedContext.GetGuiManager().guiEvents.size() > 0)
	{
		for (auto& i : sharedContext.GetGuiManager().guiEvents)
		{
			UpdateControls(i);
		}
	}
}

void GuiManager::AddControl(GuiControlType control, std::string name, sf::Vector2f position)
{
	int nextID = (int)guiControls.size();

	// Create control
	switch (control)
	{
	case GuiControlType::Label:
		guiControls.emplace_back(new Label(sharedContext, nextID, name, position));
		break;
	case GuiControlType::Button:
		guiControls.emplace_back(new Button(sharedContext, nextID, name, position));
		break;
	case GuiControlType::EditBox:
		guiControls.emplace_back(new EditBox(sharedContext, nextID, name, position));
		break;
	case GuiControlType::Shape:
		guiControls.emplace_back(new Shape(sharedContext, nextID, name, position));
		break;
	case GuiControlType::Image:
		guiControls.emplace_back(new Image(sharedContext, nextID, name, position));
		break;
	case GuiControlType::DesignBox:
		guiControls.emplace_back(new DesignBox(sharedContext, nextID, name, position));
		break;
	}

	// Giving aliases for the control
	controlName[name] = nextID;
}

int GuiManager::GetControlIdByName(std::string name)
{
	try
	{
		return controlName.at(name);
	}
	catch (const std::out_of_range& ex)
	{
		printf("Exception: %s", ex.what());
		return -1;
	}

	//return controlName[name];
}

bool GuiManager::CheckForControlEvent(GuiEvent& event, std::string name, GuiEvent::GuiEventType type)
{
	if (event.controlID == GetControlIdByName(name) && event.guiEventType == type)
		return true;

	return false;
}

Label* GuiManager::CastControlToLabel(std::string name)
{
	Label* label = dynamic_cast<Label*>(sharedContext.GetGuiManager().guiControls[GetControlIdByName(name)].get());
	if (label)
		return label;

	return nullptr;
}

Button* GuiManager::CastControlToButton(std::string name)
{
	Button* button = dynamic_cast<Button*>(sharedContext.GetGuiManager().guiControls[GetControlIdByName(name)].get());
	if (button)
		return button;

	return nullptr;
}

EditBox* GuiManager::CastControlToEditBox(std::string name)
{
	EditBox* editBox = dynamic_cast<EditBox*>(sharedContext.GetGuiManager().guiControls[GetControlIdByName(name)].get());
	if (editBox)
		return editBox;
	
	return nullptr;
}

Shape* GuiManager::CastControlToShape(std::string name)
{
	Shape* shape = dynamic_cast<Shape*>(sharedContext.GetGuiManager().guiControls[GetControlIdByName(name)].get());
	if (shape)
		return shape;

	return nullptr;
}

Image* GuiManager::CastControlToImage(std::string name)
{
	Image* image = dynamic_cast<Image*>(sharedContext.GetGuiManager().guiControls[GetControlIdByName(name)].get());
	if (image)
		return image;

	return nullptr;
}

DesignBox* GuiManager::CastControlToDesignBox(std::string name)
{
	DesignBox* designBox = dynamic_cast<DesignBox*>(sharedContext.GetGuiManager().guiControls[GetControlIdByName(name)].get());
	if (designBox)
		return designBox;

	return nullptr;
}

void GuiManager::SetProperty(std::string control, GuiProperty property, int data)
{
}

void GuiManager::SetProperty(std::string control, GuiProperty property, std::string data)
{
	// Get ID of the control
	int id = GetControlIdByName(control);
	if (id == -1)
		return; // did not find the control by name
	
	// Apply property
	switch (property)
	{
	case GuiProperty::Text:		
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Label)
			CastControlToLabel(control)->SetText(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Button)
			CastControlToButton(control)->SetText(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::EditBox)
			CastControlToEditBox(control)->SetText(data);
		break;
	case GuiProperty::File:
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Image)
			CastControlToImage(control)->LoadFile(data);
		break;
	}
}

void GuiManager::SetProperty(std::string control, GuiProperty property, sf::Vector2f data)
{
	// Get ID of the control
	int id = GetControlIdByName(control);
	if (id == -1)
		return; // did not find the control by name

	// Apply property
	switch (property)
	{
	case GuiProperty::Position:
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Label)
			CastControlToLabel(control)->SetPosition(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Button)
			CastControlToButton(control)->SetPosition(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::EditBox)
			CastControlToEditBox(control)->SetPosition(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Shape)
			CastControlToShape(control)->SetPosition(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Image)
			CastControlToImage(control)->SetPosition(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::DesignBox)
			CastControlToDesignBox(control)->SetPosition(data);
		break;
	case GuiProperty::Size:
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Button)
			CastControlToButton(control)->SetSize(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::EditBox)
			CastControlToEditBox(control)->SetSize(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Shape)
			CastControlToShape(control)->SetSize(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Image)
			CastControlToImage(control)->SetSize(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::DesignBox)
			CastControlToDesignBox(control)->SetSize(data);
		break;
	}
}

void GuiManager::SetProperty(std::string control, GuiProperty property, sf::Color data)
{
	// Get ID of the control
	int id = GetControlIdByName(control);
	if (id == -1)
		return; // did not find the control by name

	// Apply property
	switch (property)
	{
	case GuiProperty::FillColor:
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Label)
			CastControlToLabel(control)->SetFillColor(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Button)
			CastControlToButton(control)->SetFillColor(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::EditBox)
			CastControlToEditBox(control)->SetFillColor(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::Shape)
			CastControlToShape(control)->SetFillColor(data);
		if (guiControls[id]->GetGuiControlType() == GuiControlType::DesignBox)
			CastControlToDesignBox(control)->SetFillColor(data);
		break;
	}
}

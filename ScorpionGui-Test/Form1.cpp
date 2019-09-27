#include "Form1.h"

void Form1::Load()
{
	SetFormTitle("Hello");

	// Add controls to the application form
	AddControl(GuiControlType::Label, "Label1", sf::Vector2f(20, 30));
	AddControl(GuiControlType::Label, "Label2", sf::Vector2f(20, 60));
	AddControl(GuiControlType::EditBox, "Text1", sf::Vector2f(90, 60));
	AddControl(GuiControlType::Label, "Label3", sf::Vector2f(20, 90));
	AddControl(GuiControlType::EditBox, "Text2", sf::Vector2f(90, 90));
	AddControl(GuiControlType::Label, "Label4", sf::Vector2f(20, 120));
	AddControl(GuiControlType::EditBox, "Text3", sf::Vector2f(90, 120));
	AddControl(GuiControlType::Button, "Button1", sf::Vector2f(300, 120));
	AddControl(GuiControlType::Shape, "Shape1", sf::Vector2f(150, 300));
	AddControl(GuiControlType::Button, "Button2", sf::Vector2f(70, 320));
	//AddControl(GuiControlType::DesignBox, "DesignBox1", sf::Vector2f(350, 300));
	//AddControl(GuiControlType::DesignBox, "DesignBox2", sf::Vector2f(50, 300));
	AddControl(GuiControlType::Image, "Image1", sf::Vector2f(300, 200));

	// TEMP STUFF!!!!!
	NextFocus();
	SetFocus(curFocus);

	// Set properties
	SetProperty("Label1", GuiProperty::Text, "Power calculator: P=U*I");
	SetProperty("Label2", GuiProperty::Text, "Voltage:");
	SetProperty("Label3", GuiProperty::Text, "Ampere:");
	SetProperty("Label4", GuiProperty::Text, "Power:");
	SetProperty("Button1", GuiProperty::Text, "OK");
	SetProperty("Shape1", GuiProperty::Position, sf::Vector2f(50, 300));
	SetProperty("Shape1", GuiProperty::Size, sf::Vector2f(100, 200));
	SetProperty("Shape1", GuiProperty::FillColor, sf::Color(30, 100, 40));
	//SetProperty("DesignBox1", GuiProperty::Position, sf::Vector2f(100, 300));
	SetProperty("Button2", GuiProperty::Text, "OK");
	SetProperty("Image1", GuiProperty::File, "Resources/Sprite.png");

	//SetProperty("DesignBox1", GuiProperty::Size, sf::Vector2f(6, 6));
	//SetProperty("DesignBox1", GuiProperty::FillColor, sf::Color(30, 220, 40));

	//SetProperty("DesignBox2", GuiProperty::Size, sf::Vector2f(6, 6));
	//SetProperty("DesignBox2", GuiProperty::FillColor, sf::Color(30, 220, 40));

	// Post initialization of controls
	//CastControlToDesignBox("DesignBox1")->AttachToControl("Button2");
	//CastControlToDesignBox("DesignBox1")->AttachToControl("Text2");
	//CastControlToDesignBox("DesignBox2")->AttachToControl("Shape1");
}

void Form1::UpdateControls(GuiEvent guiEvent)
{
	if (CheckForControlEvent(guiEvent, "Button1", GuiEvent::GuiEventType::OnMouseLeftClick))
	{
		float num1 = (float)std::atof(CastControlToEditBox("Text1")->GetText().c_str());
		float num2 = (float)std::atof(CastControlToEditBox("Text2")->GetText().c_str());
		float ans = num1 * num2;

		CastControlToEditBox("Text3")->SetText(std::to_string(ans));
		return;
	}

	if (CheckForControlEvent(guiEvent, "Button2", GuiEvent::GuiEventType::OnMouseLeftClick))
	{
		CastControlToEditBox("Text1")->SetText("");
		CastControlToEditBox("Text2")->SetText("");
		CastControlToEditBox("Text3")->SetText("");
	}

	/*
	if (CheckForControlEvent(guiEvent, "DesignBox1", GuiEvent::GuiEventType::OnMouseLeftPressed))
	{
		CastControlToDesignBox("DesignBox1")->isDragging = true;
		return;
	}

	if (CheckForControlEvent(guiEvent, "DesignBox1", GuiEvent::GuiEventType::OnMouseLeftReleased))
	{
		CastControlToDesignBox("DesignBox1")->isDragging = false;
		return;
	}

	if (CheckForControlEvent(guiEvent, "DesignBox1", GuiEvent::GuiEventType::OnMouseMove))
	{
		if (CastControlToDesignBox("DesignBox1")->isDragging)
		{

		}
		return;
	}
	*/
}

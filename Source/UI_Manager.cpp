#include"UI_Manager.h"

UI_Manager::UI_Manager()
{

}

UI_Manager::~UI_Manager()
{
	Clear();
}

void UI_Manager::Update(float elapsedTime)
{
	for (auto& ui : UiArray)
	{
		ui->Update(elapsedTime);
	}
}

void UI_Manager::Render()
{
	for (auto& ui : UiArray)
	{
		ui->Render();
	}
}

void UI_Manager::Register(UI*ui)
{
	UiArray.push_back(ui);
}

void UI_Manager::Clear()
{
	for (auto& uis : UiArray)
	{
		delete uis;
	}
	remove.clear();
}

void UI_Manager::Remove(UI* ui)
{
	remove.insert(ui);
}
#include"UI.h"
#include"Sperm_Manager.h"
#include"Graphics/Graphics.h"

Sperm_Count_UI::Sperm_Count_UI()
{
	sSperm = std::make_unique<Sprite>("Data/Sprite/otama.png");

}

Sperm_Count_UI::~Sperm_Count_UI()
{
	
}

void Sperm_Count_UI::Update(float elapsedTime)
{
	sperm_count = Sperm_Manager::Instance().GetSpermCount();
}

void Sperm_Count_UI::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	sSperm->Render(dc,
		0, 0, 100, 100,
		0, 0, 2048, 2048,
		0,
		1, 1, 1, 1);
}
#pragma once
#include"Graphics/Sprite.h"
#include"Graphics/Font.h"

class UI
{
public:
	virtual ~UI() {}

	virtual void Update(float elapsedTime) = 0;
	virtual void Render() = 0;
};

class Sperm_Count_UI:public UI
{
public:
	Sperm_Count_UI();
	~Sperm_Count_UI()override;
	void Update(float elapsedTime)override;
	void Render()override;
private:
	std::unique_ptr<Sprite>sSperm;
	int sperm_count = 0;
};
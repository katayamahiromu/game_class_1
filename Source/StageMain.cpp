#include"StageMain.h"

StageMain::StageMain() {
	model = new Model("Data/Model/BaseStage/BaseStage.mdl");
}

StageMain::~StageMain() {
	delete model;
}

void StageMain::Update(float elapsedTime) {

}

void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader) {
	shader->Draw(dc, model);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::InterserctRayVsModel(start, end, model,hit);
}
#include "RailCamera.h"
#include <WorldTransform_MT.h>

#include <ImGuiManager.h>
void RailCamera::Initialize(Vector3 cameraPos, Vector3 cameraRotate) {


	worldTransform_.Initialize();

	worldTransform_.translation_ = cameraPos;
	worldTransform_.rotation_ = cameraRotate;

	viewProjection_.farZ = 1145;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	

	worldTransform_.translation_.z += 0.01f;
	//worldTransform_.rotation_.z += 0.01f;

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -600.0f, 600.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -10.0f, 10.0f);
	ImGui::End();


	worldTransform_.UpdateMatrix();

}

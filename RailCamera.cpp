#include "RailCamera.h"
#include <WorldTransform_MT.h>

#include <ImGuiManager.h>
void RailCamera::Initialize(Vector3 cameraPos, Vector3 cameraRotate) {

	worldTransform_.translation_ = cameraPos;
	worldTransform_.rotation_ = cameraRotate;

	viewProjection_.Initialize();
}

void RailCamera::Update() {

	worldTransform_.translation_.z += 0.01f;
	worldTransform_.rotation_.z += 0.01f;

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	//worldTransform_.UpdateMatrix();
}

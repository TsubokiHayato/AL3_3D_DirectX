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
	
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	
	worldTransform_.UpdateMatrix();

}

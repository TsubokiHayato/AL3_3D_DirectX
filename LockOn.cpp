#include "LockOn.h"
#include"TextureManager.h"

void LockOn::Initialize() { 
	viewProjection_->Initialize();
	worldTransform_.Initialize();
	lockOnMark_->Initialize();

	Vector2 anchorPoint = {0.5f, 0.5f};
	uint32_t textureReticle = TextureManager::Load("2D_Reticle.png");
	lockOnMark_->Create(textureReticle, {}, {1.0f, 1.0f, 1.0f, 1.0f}, anchorPoint);


}

void LockOn::Update() {


}

void LockOn::Draw() {

	lockOnMark_->Draw(&viewProjection_); 
}

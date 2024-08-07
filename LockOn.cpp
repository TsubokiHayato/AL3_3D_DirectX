#include "LockOn.h"
#include"TextureManager.h"

void LockOn::Initialize(ViewProjection* viewProjection) { 
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	

	Vector2 anchorPoint = {0.5f, 0.5f};
	uint32_t textureReticle = TextureManager::Load("2D_Reticle.png");
	lockOnMark_->Create(textureReticle, {}, {1.0f, 1.0f, 1.0f, 1.0f}, anchorPoint);


}

void LockOn::Update(std::list<std::unique_ptr<Enemy>>& enemies,const ViewProjection& viewProjection ) {


	worldTransform_.UpdateMatrix();
	viewProjection_->UpdateMatrix();
	viewProjection_->TransferMatrix();
}

void LockOn::Draw() {

	lockOnMark_->Draw(); 
}

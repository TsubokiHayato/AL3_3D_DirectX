#include "PlayerBullet.h"
#include <cassert>
#include<TextureManager.h>

void PlayerBullet::Initialize(Model* model, const Vector3& pos) {

	assert(model);

	// 3Dモデルの作成
	model_ = model;

	textureHandle_ = TextureManager::Load("uvChecker.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;

	

}

void PlayerBullet::Update() { worldTransform_.UpdateMatrix(); }

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	//
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

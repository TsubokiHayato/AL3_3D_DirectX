#include "PlayerBullet.h"
#include <cassert>
#include<TextureManager.h>

void PlayerBullet::Initialize(Model* model, const Vector3& pos,const Vector3& velocity) {

	assert(model);

	// 3Dモデルの作成
	model_ = model;

	textureHandle_ = TextureManager::Load("uvChecker.png");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;
	
	velocity_ = velocity;

	

}

void PlayerBullet::Update() { 

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	
	
	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	//
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


Vector3 PlayerBullet::GetWorldPos() {

	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void PlayerBullet::OnCollision() { isDead_ = true; }

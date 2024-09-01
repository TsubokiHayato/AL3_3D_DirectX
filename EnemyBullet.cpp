#include "EnemyBullet.h"
#include <TextureManager.h>
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {

	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = pos;

	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.png");
}

void EnemyBullet::Update() {
	

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPos() { 

	
	Vector3 worldPos = {};

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;


}

#include "Enemy.h"
#include <TextureManager.h>
#include <Vector3.h>
#include <cassert>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 3.0f;
	worldTransform_.translation_.z = 100.0f;
	textureHandle_ = TextureManager::Load("slime.jpg");

	viewProjection_ = viewProjection;
}

void Enemy::Update() {

	// Enemyの移動ベクトル
	Vector3 moveApproach = {0.0f, 0.0f, 0.0f};
	Vector3 moveLeave = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	 const float kCharaSpeed = 0.2f;

	switch (phase_) {
	case Phase::Approach:
	    moveApproach.z -= kCharaSpeed;
	    worldTransform_.translation_ += moveApproach;
	    if (worldTransform_.translation_.z < 0.0f) {
	        phase_ = Phase::Leave;
	    }
	    break;
	case Phase::Leave:

	    moveLeave.z += kCharaSpeed;
	    worldTransform_.translation_ += moveLeave;

	    break;

	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {

	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

#include "Enemy.h"
#include<TextureManager.h>
#include <cassert>
#include<Vector3.h>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 3.0f;
	worldTransform_.translation_.z = 10.0f;
	textureHandle_ = TextureManager::Load("slime.jpg");

	

	viewProjection_ = viewProjection;

	


}

void Enemy::Update() {

// Enemyの移動ベクトル
    Vector3 move = {0.0f, 0.0f, 0.0f};

// キャラクターの移動速さ
const float kCharaSpeed = 0.2f;
	move.z += kCharaSpeed;

worldTransform_.translation_.z -= move.z;

// 行列計算
worldTransform_.UpdateMatrix();

}

void Enemy::Draw() {

	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}

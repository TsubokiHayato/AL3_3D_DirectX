#include <Player.h>
#include <cassert>
//初期化
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	assert(model);

	// 3Dモデルの作成
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}
//更新
void Player::Update() { 
	worldTransform_.UpdateMatrix(&worldTransform_);
	
}
//描画
void Player::Draw() {
	// 3Dポーションを作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
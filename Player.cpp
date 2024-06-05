#include <Player.h>
#include <cassert>
// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;
	textureHandle_ = textureHandle;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;

	input_ = Input::GetInstance();
}
// 更新
void Player::Update() {

	// playerの移動ベクトル
	Vector3 move = {};

	// キャラクターの移動速さ
	const float kCharaSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_D)) {
		{ move.x += kCharaSpeed; }
	}
	worldTransform_.translation_ += move;

	worldTransform_.TransferMatrix();
}
// 描画
void Player::Draw() {
	// 3Dポーションを作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
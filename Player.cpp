#include <Player.h>
#include <cassert>
#include<ImGuiManager.h>
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
	Vector3 move = {0.0f,0.0f,0.0f};

	// キャラクターの移動速さ
	const float kCharaSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_D)) {
		{ move.x += kCharaSpeed; }
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kCharaSpeed;
	} else if (input_->PushKey(DIK_W)) {
		{ move.y += kCharaSpeed; }
	}

	ImGui::SliderFloat3("Player Position", &worldTransform_.translation_.x, -600.0f, 600.0f);
	
	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);



	

	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix4x4();
}
// 描画
void Player::Draw() {
	// 3Dポーションを作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
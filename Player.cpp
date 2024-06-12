
#include <Player.h>

#include <algorithm>
#include <cassert>

#include <ImGuiManager.h>
#include<MT.h>


Player::~Player() {
	model_ = nullptr;
	viewProjection_ = nullptr;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity,worldTransform_.matWorld_ );

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPos() { 
	
	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	
	
	
	return worldPos;
}

// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	textureHandle_ = textureHandle;

	//// プレイヤーの初期角度
	//worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;

	input_ = Input::GetInstance();
}
// 更新
void Player::Update() {


	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
 		return false;
	});

	// 回転速さ
	const float kRotSpeed = 0.02f;
	// 回転処理
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	// playerの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// キャラクターの移動速さ
	const float kCharaSpeed = 0.2f;

	// 移動処理
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharaSpeed;
	}

	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharaSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharaSpeed;
	}



	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	ImGui::Begin("Memo");
	ImGui::SliderFloat3("Player Position", &worldTransform_.translation_.x, -600.0f, 600.0f);
	ImGui::Text("playerMove : LEFT RIGHT UP DOWN");
	ImGui::Text("playerRotate : A D");
	ImGui::Text("playerAttack :Space");
	ImGui::Text("DebugCameraMode : Q");
	ImGui::End();
	// 移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_ += move;

	// 行列計算
	worldTransform_.UpdateMatrix();
}
// 描画
void Player::Draw() {
	// 3D作成
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {

		bullet->Draw(*viewProjection_);
	}
}

void Player::OnCollision() {


}

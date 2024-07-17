
#include <Player.h>

#include <algorithm>
#include <cassert>

#include <ImGuiManager.h>
#include <MT.h>
#include <TextureManager.h>
#include"WinApp.h"
#include"WorldTransform_MT.h"
#include"ViewProjection.h"

Player::~Player() {
	model_ = nullptr;
	modelTarget_ = nullptr;
	viewProjection_ = nullptr;

	delete sprite2DReticle_;

	input_ = nullptr;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Attack() {

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER){
	
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = worldTransform3DReticle_.translation_ - GetWorldPos();
		velocity = Normalize(velocity) * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		newBullet->SetParent(worldTransform_.parent_);
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

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

// 初期化
void Player::Initialize(Model* model, Model* targetModel, uint32_t textureHandle, ViewProjection* viewProjection, Vector3 Position) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;
	modelTarget_ = targetModel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = Position;

	textureHandle_ = textureHandle;

	//// プレイヤーの初期角度
	// worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
	
	worldTransform3DReticle_.Initialize();

	Vector2 anchorPoint = {0.5f, 0.5f};
	uint32_t textureReticle = TextureManager::Load("2D_Reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {}, {1.0f,1.0f,1.0f,1.0f},anchorPoint);

	input_ = Input::GetInstance();
}
// 更新
void Player::Update(const Matrix4x4 viewMatrix, const Matrix4x4 viewProjectionMatrix) {

	


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
	if (input_->PushKey(DIK_W)) {
		worldTransform_.rotation_.x -= kRotSpeed;
	} else if (input_->PushKey(DIK_S)) {
		worldTransform_.rotation_.x += kRotSpeed;
	}


	Vector2 spritePos = sprite2DReticle_->GetPosition();

	

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePos.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

		sprite2DReticle_->SetPosition(spritePos);

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

	

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	
		move.x += (float)joyState.Gamepad.sThumbLX/SHRT_MAX*kCharaSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharaSpeed;
	}

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	const float kDistancePlayerTo3DReticle = 215.0f;
	//
	Vector3 offset = {0, 0, 1.0f};
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ = offset;

    worldTransform3DReticle_.UpdateMatrix();


	

	Vector3 positionReticle = worldTransform3DReticle_.translation_;

	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = viewMatrix * viewProjectionMatrix * matViewport;






	////POINT mousePos;
	//GetCursorPos(&spritePos);

	//HWND hwnd = WinApp::GetInstance()->GetHwnd();

	//ScreenToClient(hwnd, &spritePos);

	
	positionReticle = Transform(positionReticle, matVPV);

	sprite2DReticle_->SetPosition(Vector2((float)spritePos.x,(float)spritePos.y));


	

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 posNear = Vector3(static_cast<float>(spritePos.x), static_cast<float>(spritePos.y), 0.0f);
	Vector3 posFar = Vector3(static_cast<float>(spritePos.x), static_cast<float>(spritePos.y), 1.0f);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = posFar - posNear; // posNearからposFarへのベクトルを計算する
	mouseDirection = Normalize(mouseDirection);

	const float kDistanceTestObject = 50.0f; // 設定距離（適切な距離を設定する）

	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject; // posNearからmouseDirectionの方向にkDistanceTestObject進んだ座標
	
	worldTransform3DReticle_.UpdateMatrix();


	ImGui::Begin("Memo");
	//ImGui::SliderFloat2("Position", &positionReticle.x, -600.0f, 600.0f);
	
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
	//modelTarget_->Draw(worldTransform3DReticle_, *viewProjection_);
	for (PlayerBullet* bullet : bullets_) {

		bullet->Draw(*viewProjection_);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::OnCollision() {}

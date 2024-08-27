#include "winApp.h"
#include <Player.h>
#include <cassert>

#include "MT_Matrix.h"

#include "ImGuiManager.h"
#include "Input.h"
#include "ViewProjection.h"
#include <algorithm>

#include <XInput.h>
#include <iostream>

#include "GlobalVariables.h"

#pragma comment(lib, "XInput.lib")

#include "LockOn.h"

#define PI 3.14159265359f

inline Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
	return Vector3(
	    vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0], vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
	    vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]);
}
// 初期化
void Player::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {

	// 基底クラスの初期化
	BaseCharacter::Initialize(models, viewProjection);
	
	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "period", period);

	ApplyGlobalVariables();

	assert(models[kModelIndexHead]);
	assert(models[kModelIndexBody]);
	assert(models[kModelIndexL_arm]);
	assert(models[kModelIndexR_arm]);
	assert(models[kModelIndexHammer]);

	modelHead_ = models[kModelIndexHead];
	modelBody_ = models[kModelIndexBody];
	modelLeftArm_ = models[kModelIndexL_arm];
	modelRightArm_ = models[kModelIndexR_arm];
	modelHammer_ = models[kModelIndexHammer];

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldHeadTransform_.Initialize();
	worldLArmTransform_.Initialize();
	worldRArmTransform_.Initialize();
	worldBodyTransform_.Initialize();
	worldHammerTransform_.Initialize();

	worldBodyTransform_.parent_ = &worldTransform_;
	worldHeadTransform_.parent_ = &worldBodyTransform_;
	worldLArmTransform_.parent_ = &worldBodyTransform_;
	worldRArmTransform_.parent_ = &worldBodyTransform_;

	worldHammerTransform_.parent_ = &worldRArmTransform_;

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldHeadTransform_.translation_ = {0.0f, 1.4f, 0.0f};
	worldBodyTransform_.translation_ = {0.0f, 0.3f, 0.0f};
	worldLArmTransform_.translation_ = {-0.5f, 1.2f, 0.0f};
	worldRArmTransform_.translation_ = {0.5f, 1.2f, 0.0f};

	viewProjection_ = viewProjection;

	velocity_ = {};

	worldHammerTransform_.rotation_.x = -3.5f;

	InitializeFloatingGimmick();
}
// 更新
void Player::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		globalVariables->SaveFile("Player");
	}

	if (behaviorRequest_) {

		behavior_ = behaviorRequest_.value();

		switch (behavior_) {

		case Behavior::kRoot:

		default:
			BehaviorRootInitialize();

			break;
		case Behavior::kAttack:

			BehaviorAttackInitialize();

			break;

		case Behavior::kJump:

			BehaviorJumpInitialize();
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {

	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kJump:
		BehaviorJumpUpdate();
	}

	// ImGui::DragFloat3("Body_Scale", &worldBodyTransform_.scale_.x, 0.1f);
	// ImGui::DragFloat3("Body_Rotation", &worldBodyTransform_.rotation_.x, 0.1f);
	// ImGui::DragFloat3("Body_Transform", &worldBodyTransform_.translation_.x, 0.1f);

	UpdateFloatingGimmick();

	worldBodyTransform_.UpdateMatrix();
	worldHeadTransform_.UpdateMatrix();
	worldLArmTransform_.UpdateMatrix();
	worldRArmTransform_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	worldHammerTransform_.UpdateMatrix();
}
// 描画
void Player::Draw() {

	modelHead_->Draw(worldHeadTransform_, *viewProjection_);
	modelBody_->Draw(worldBodyTransform_, *viewProjection_);
	modelLeftArm_->Draw(worldLArmTransform_, *viewProjection_);
	modelRightArm_->Draw(worldRArmTransform_, *viewProjection_);

	if (isAttack) {
		modelHammer_->Draw(worldHammerTransform_, *viewProjection_);
	}
}

Vector3 Player::GetCenterPos() const { 

	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;

}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;
	period = 120;

	floatingSwing = 0.01f;
}

void Player::UpdateFloatingGimmick() {
	step = 2.0f * PI / period;
	floatingParameter_ += step;

	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * PI);

	worldBodyTransform_.translation_.y += std::sin(floatingParameter_) * floatingSwing;
}

void Player::BehaviorRootUpdate() {

	ImGui::Text("RootUpdate");

	isAttack = false;
	XINPUT_STATE joyState;

	ZeroMemory(&joyState, sizeof(XINPUT_STATE));

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// Get joystick input values and clamp them to valid range
		float thumbLX = static_cast<float>(joyState.Gamepad.sThumbLX);
		float thumbLY = static_cast<float>(joyState.Gamepad.sThumbLY);

		// Normalize the joystick values
		float maxThumbValue = static_cast<float>(SHRT_MAX);
		thumbLX = std::clamp(thumbLX / maxThumbValue, -1.0f, 1.0f);
		thumbLY = std::clamp(thumbLY / maxThumbValue, -1.0f, 1.0f);
		if (thumbLX != 0.0f || thumbLY != 0.0f) {

			/*-------
			移動処理
			-------*/
			const float speed = 0.3f;

			velocity_.x = thumbLX;
			velocity_.y = 0.0f;
			velocity_.z = thumbLY;

			// Normalize the movement vector
			velocity_ = Normalize(velocity_) * speed;

			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);

			velocity_ = TransformNormal(velocity_, rotateMatrix);
			Vector3 targetPos = worldTransform_.translation_ + velocity_;
			Vector3 sub = targetPos - GetWorldPos();

			//
			worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);

			// idou
			worldTransform_.translation_ = targetPos;

		} else if (lockOn_ && lockOn_->ExistTarget()) {

			// ロックオン座標
			Vector3 lockOnPosition = lockOn_->GetTargetPos();

			// プレイヤーからロックオン座標へのベクトル
			Vector3 sub = lockOnPosition - GetWorldPos();

			// Y軸周りの角度を計算してプレイヤーの回転を更新
			worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
		}

		// Attack to B
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			behaviorRequest_ = Behavior::kAttack;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kJump;
		}
	}
}

void Player::BehaviorAttackUpdate() {
	float speed = 0.4f;
	isAttack = true;
	// ロックオン中
	if (lockOn_ && lockOn_->ExistTarget()) {
		//ロックオン座標
		Vector3 lockOnPos = lockOn_->GetTargetPos();
		//追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPos - GetWorldPos();

		//距離
		float distance = Length(sub);

		//距離しきい値
		const float threshold = 0.2f;

		// しきい値より離れている時のみ
		if (distance > threshold) {
			// Y軸周り角度
			worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);

			// しきい値を超える速さなら補正する
			if (speed > distance - threshold) {
				// ロックオン対象へのめり込み防止
				speed = distance - threshold;
			}
		}
	}

	worldRArmTransform_.rotation_.x += 0.07f;
	worldLArmTransform_.rotation_.x += 0.07f;

	if (worldRArmTransform_.rotation_.x > -1.5f) {

		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpUpdate() {

	// 移動
	worldTransform_.translation_ += velocity_;
	// 重力加速度
	const float kGravityAcceleration = 0.05f;
	// 加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	// 加速する
	velocity_ += accelerationVector;

	// 着地
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0;
		// ジャンプ終わり
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables() {

	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	period = globalVariables->GetIntValue(groupName, "period");
}

void Player::OnCollision() { 
	behaviorRequest_ = Behavior::kJump;
	ImGui::Text("HIT!!");
}

/*---------------



----------------*/
void Player::BehaviorRootInitialize() {
	ImGui::Text("RootInit");

	worldHammerTransform_.rotation_.x = 0.0f;
	worldLArmTransform_.rotation_.x = 0.0f;
	worldRArmTransform_.rotation_.x = 0.0f;
}

void Player::BehaviorAttackInitialize() {
	ImGui::Text("attackInit");
	worldHammerTransform_.translation_ = {-0.5f, 1.3f, -1.0f};
	worldHammerTransform_.rotation_.x = -3.5f;
	worldLArmTransform_.rotation_.x = -3.0f;
	worldRArmTransform_.rotation_.x = -3.0f;
}

void Player::BehaviorJumpInitialize() {

	worldBodyTransform_.translation_.y = 0;
	worldLArmTransform_.rotation_.x = 0;
	worldRArmTransform_.rotation_.x = 0;

	const float kJumpFirstSpeed = 1.0f;

	velocity_.y = kJumpFirstSpeed;
}

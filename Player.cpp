#include "winApp.h"
#include <Player.h>
#include <cassert>

#include "MT_Matrix.h"

#include "ImGuiManager.h"
#include "Input.h"
#include "ViewProjection.h"
#include <algorithm>

#define PI 3.14159265359f

inline Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
	return Vector3(
	    vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0], vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
	    vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]);
}
// 初期化
void Player::Initialize(const std::vector<Model*>&models, ViewProjection* viewProjection) {

	assert(models[kModelIndexHead]);
	assert(models[kModelIndexBody]);
	assert(models[kModelIndexL_arm]);
	assert(models[kModelIndexR_arm]);
	assert(models[kModelIndexHammer]);

	modelHead_= models[kModelIndexHead];
	modelBody_=models[kModelIndexBody];
	modelLeftArm_= models[kModelIndexL_arm];
	 modelRightArm_=models[kModelIndexR_arm];
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

	worldTransform_.translation_ = {0.0f,0.0f, 0.0f};
	worldHeadTransform_.translation_ = {0.0f, 1.4f, 0.0f};
	worldBodyTransform_.translation_ = {0.0f, 0.3f, 0.0f};
	worldLArmTransform_.translation_ = {-0.5f, 1.2f, 0.0f};
	worldRArmTransform_.translation_ = {0.5f, 1.2f, 0.0f};

	viewProjection_ = viewProjection;

	move = {};

	worldHammerTransform_.rotation_.x = -3.5f;


	InitializeFloatingGimmick();
}
// 更新
void Player::Update() {



	//switch (phase_) {
	//case Phase::Approach:
	//	BehaviorAttackUpdate();

	//	if (worldRArmTransform_.rotation_.x > -1.5f) {
	//		phase_ = Phase::Leave;
	//	}
	//	break;

	//case Phase::Leave:

	//	worldHammerTransform_.translation_ = {-0.5f, 1.3f, -1.0f};
	//	worldHammerTransform_.rotation_.x = -3.5f;
	//	worldLArmTransform_.rotation_.x = -3.0f;
	//	worldRArmTransform_.rotation_.x = -3.0f;

	//	phase_ = Phase::Approach;

	//	break;
	//}


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
	}
	


	ImGui::DragFloat3("Body_Scale", &worldBodyTransform_.scale_.x, 0.1f);
	ImGui::DragFloat3("Body_Rotation", &worldBodyTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("Body_Transform", &worldBodyTransform_.translation_.x, 0.1f);

	UpdateFloatingGimmick();

	worldTransform_.UpdateMatrix();
	worldBodyTransform_.UpdateMatrix();
	worldHeadTransform_.UpdateMatrix();
	worldLArmTransform_.UpdateMatrix();
	worldRArmTransform_.UpdateMatrix();
	worldHammerTransform_.UpdateMatrix();
}
// 描画
void Player::Draw() {

	modelHead_->Draw(worldHeadTransform_, *viewProjection_);
	modelBody_->Draw(worldBodyTransform_, *viewProjection_);
	modelLeftArm_->Draw(worldLArmTransform_, *viewProjection_);
	modelRightArm_->Draw(worldRArmTransform_, *viewProjection_);
	modelHammer_->Draw(worldHammerTransform_, *viewProjection_);



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

	
	XINPUT_STATE joyState;


	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float speed = 0.3f;

		// Get joystick input values and clamp them to valid range
		float thumbLX = static_cast<float>(joyState.Gamepad.sThumbLX);
		float thumbLY = static_cast<float>(joyState.Gamepad.sThumbLY);

		// Normalize the joystick values
		float maxThumbValue = static_cast<float>(SHRT_MAX);
		thumbLX = std::clamp(thumbLX / maxThumbValue, -1.0f, 1.0f);
		thumbLY = std::clamp(thumbLY / maxThumbValue, -1.0f, 1.0f);

		move.x = thumbLX;
		move.y = 0.0f;
		move.z = thumbLY;

		// Normalize the movement vector
		move = Normalize(move) * speed;

		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);

		move = TransformNormal(move, rotateMatrix);
		Vector3 targetPos = worldTransform_.translation_ + move;
		Vector3 sub = targetPos - GetWorldTransformTranslate();

		worldTransform_.rotation_.y = std::atan2(sub.x, sub.y);

		worldTransform_.rotation_.y = viewProjection_->rotation_.y;
		// Add the movement to the translation
		worldTransform_.translation_ = targetPos;
	} 


	if ()


}

void Player::BehaviorAttackUpdate() { 

	worldRArmTransform_.rotation_.x += 0.04f;
	worldLArmTransform_.rotation_.x += 0.04f;

	if (worldRArmTransform_.rotation_.x > -1.5f) {
		behavior_ = Behavior::kRoot;
	}

	ImGui::Begin("player");
	ImGui::DragFloat3("hammer_tra", &worldHammerTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("hammer_rotate", &worldHammerTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("LArm_rotate", &worldLArmTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("RArm_rotate", &worldRArmTransform_.rotation_.x, 0.1f);
	ImGui::DragInt("period", &period, 1, 1, 300);
	ImGui::DragFloat("floatingSwing", &floatingSwing, 0.1f, 0.1f, 30.0f);
	ImGui::End();

}


void Player::BehaviorRootInitialize() {
	worldHammerTransform_.translation_ = {0.0f, -100.0f, 0.0f};
	worldHammerTransform_.rotation_.x = 0.0f;
	worldLArmTransform_.rotation_.x = 0.0f;
	worldRArmTransform_.rotation_.x = 0.0f;
}

void Player::BehaviorAttackInitialize() {

	worldHammerTransform_.translation_ = {-0.5f, 1.3f, -1.0f};
	worldHammerTransform_.rotation_.x = -3.5f;
	worldLArmTransform_.rotation_.x = -3.0f;
	worldRArmTransform_.rotation_.x = -3.0f;

}
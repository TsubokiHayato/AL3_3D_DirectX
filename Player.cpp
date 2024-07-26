#include "winApp.h"
#include <Player.h>
#include <cassert>

#include "MT_Matrix.h"

#include "ImGuiManager.h"
#include "Input.h"
#include "ViewProjection.h"

#define PI 3.14159265359f

inline Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
	return Vector3(
	    vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0], vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
	    vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]);
}
// 初期化
void Player::Initialize(Model* modelHead, Model* modelBody, Model* modelLeftArm, Model* modelRightArm, ViewProjection* viewProjection) {

	assert(modelHead);
	assert(modelBody);
	assert(modelLeftArm);
	assert(modelRightArm);

	modelHead_ = modelHead;
	modelBody_ = modelBody;
	modelLeftArm_ = modelLeftArm;
	modelRightArm_ = modelRightArm;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	worldHeadTransform_.Initialize();
	worldLArmTransform_.Initialize();
	worldRArmTransform_.Initialize();
	worldBodyTransform_.Initialize();

	worldHeadTransform_.parent_ = &worldBodyTransform_;
	worldLArmTransform_.parent_ = &worldBodyTransform_;
	worldRArmTransform_.parent_ = &worldBodyTransform_;

	worldHeadTransform_.translation_ = {0.0f, 1.5f, 0.0f};
	worldBodyTransform_.translation_ = {0.0f, 2.0f, 0.0f};
	worldLArmTransform_.translation_ = {-0.5f, 1.2f, 0.0f};
	worldRArmTransform_.translation_ = {0.5f, 1.2f, 0.0f};

	

	InitializeFloatingGimmick();

	HeadViewProjection_ = viewProjection;
	BodyViewProjection_ = viewProjection;
	LArmViewProjection_ = viewProjection;
	RArmViewProjection_ = viewProjection;

	viewProjection_ = viewProjection;
}
// 更新
void Player::Update() {

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float speed = 0.3f;

		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

		move = Normalize(move) * speed;

		Matrix4x4 viewport = BodyViewProjection_->matView;
		Vector3 rotate = TransformNormal(move, Inverse(viewport));

		move.sanitize();
		rotate.sanitize();

		worldBodyTransform_.rotation_.y = atan2(rotate.x, rotate.z);
		worldBodyTransform_.translation_ += move;
	}

	UpdateFloatingGimmick();

	ImGui::DragFloat3("Body_Scale", &worldBodyTransform_.scale_.x, 0.1f);
	ImGui::DragFloat3("Body_Rotation", &worldBodyTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("Body_Transform", &worldBodyTransform_.translation_.x, 0.1f);

	worldBodyTransform_.UpdateMatrix();
	worldHeadTransform_.UpdateMatrix();
	worldLArmTransform_.UpdateMatrix();
	worldRArmTransform_.UpdateMatrix();

	worldTransform_.UpdateMatrix();
}
// 描画
void Player::Draw() {

	modelHead_->Draw(worldHeadTransform_, *HeadViewProjection_);
	modelBody_->Draw(worldBodyTransform_, *BodyViewProjection_);
	modelLeftArm_->Draw(worldLArmTransform_, *LArmViewProjection_);
	modelRightArm_->Draw(worldRArmTransform_, *RArmViewProjection_);
}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;
	period = 120;

	floatingSwing = 0.1f;
}

void Player::UpdateFloatingGimmick() {
	step = 2.0f * PI / period;
	floatingParameter_ += step;

	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * PI);

	worldBodyTransform_.translation_.y += std::sin(floatingParameter_) * floatingSwing;

	/*worldLArmTransform_.rotation_.x += std::sin(floatingParameter_) * floatingSwing;
	worldRArmTransform_.rotation_.x += std::sin(floatingParameter_) * floatingSwing;*/

	ImGui::Begin("player");
	ImGui::DragFloat3("Head_Transform", &worldHeadTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("LArm_Transform", &worldLArmTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("RArm_Transform", &worldRArmTransform_.rotation_.x, 0.1f);
	ImGui::DragInt("period", &period, 1,1,300);
	ImGui::DragFloat("floatingSwing", &floatingSwing, 0.1f,0.1f,30.0f);
	ImGui::End();
}

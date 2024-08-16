#include "LockOn.h"
#include"TextureManager.h"

#include <Input.h>
#include <iostream>
#include"MT_Matrix.h"
#include"ImGuiManager.h"
#include"WinApp.h"

void LockOn::Initialize(uint32_t textureReticle) { 


	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "LockOn";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "kDegreeToRadian", kDegreeToRadian);

	ApplyGlobalVariables();


	
	worldTransform_.Initialize();
	

	Vector2 anchorPoint = {0.5f, 0.5f};
	
	lockOnMark_.reset(Sprite::Create(textureReticle, {}, {1.0f,1.0f,1.0f,1.0f},anchorPoint));


}

void LockOn::Update(std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	angleRange_ = 20.0f * kDegreeToRadian;

	//ボタンを押したらロックオン
	XINPUT_STATE joyState;

	ZeroMemory(&joyState, sizeof(XINPUT_STATE));

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (target_) {
			// ロックオン解放戦線

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				target_ = nullptr;
			} else if (IsOutOfRangeJudgment(viewProjection)) {
				target_ = nullptr;
			}

		} else {

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				Search(enemies, viewProjection);
			}
		}
	}


	if (target_) {
	//
		
			Vector3 posWorld = target_->GetWorldTransform();
	

			Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
			Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;

			posWorld = Transform(posWorld, matViewProjectionViewport);

			lockOnMark_->SetPosition(Vector2(posWorld.x, posWorld.y));
		
		

	}
	worldTransform_.UpdateMatrix();

}

void LockOn::Draw() {
	if (target_) {


		lockOnMark_->Draw();
	}
}

void LockOn::Search(std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	std::list<std::pair<float, const Enemy*>> targets;

	for (const std::unique_ptr<Enemy>& enemy : enemies) {
	
		Vector3 posWorld = enemy->GetWorldTransform();

		Vector3 posView = Transform(posWorld, viewProjection.matView);



		
		if (minDistance_ <= posView.z && posView.z <= maxDistance_) {

			//
			float arcTangent = std::atan2(
				std::sqrt(posView.x * posView.x + posView.y * posView.y),
				posView.z);

			//角度条件チェック(コーンに収まっているか)
			if (std::abs(arcTangent) <= angleRange_) {
				targets.emplace_back(std::make_pair(posView.z, enemy.get()));
			}
			
		}

		target_ = nullptr;

		if (!targets.empty()) {
		
			//
			targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
			target_ = targets.front().second;
		}

	}

}

bool LockOn::IsOutOfRangeJudgment(const ViewProjection& viewProjection) {

	Vector2 enemyLockOnPos=lockOnMark_->GetPosition();

	Vector3 posView = Transform(enemyLockOnPos, viewProjection.matView);

	if (minDistance_ <= posView.z && posView.z < maxDistance_) {
	

		//
		float arcTangent = std::atan2(std::sqrt(posView.x * posView.x + posView.y * posView.y), posView.z);

		// 角度条件チェック(コーンに収まっているか)
		if (std::abs(arcTangent) <= angleRange_) {
			return true;
		}

	}
	return false;
	
}

Vector3 LockOn::GetTargetPos() const {
	

	if (target_) {
		return target_->GetWorldTransform();
	}
	return Vector3();
	}


void LockOn::ApplyGlobalVariables() {

	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "LockOn";

	kDegreeToRadian = globalVariables->GetFloatValue(groupName, "kDegreeToRadian");

}


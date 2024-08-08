#include "LockOn.h"
#include"TextureManager.h"

#include <Input.h>
#include <iostream>
#include"MT_Matrix.h"
#include"ImGuiManager.h"

void LockOn::Initialize(uint32_t textureReticle) { 
	
	worldTransform_.Initialize();
	

	Vector2 anchorPoint = {0.5f, 0.5f};
	
	lockOnMark_.reset(Sprite::Create(textureReticle, {}));


}

void LockOn::Update(std::list<std::unique_ptr<Enemy>>& enemies,const ViewProjection& viewProjection, ) {


	//ボタンを押したらロックオン
	XINPUT_STATE joyState;

	ZeroMemory(&joyState, sizeof(XINPUT_STATE));

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			Search(enemies, viewProjection);
			isSearch = true;
		}

	}

	if (target_) {
	//
		Vector3 posWorld = target_->;

		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewport = viewMatrix * viewProjectionMatrix * matViewport;

		posWorld= Transform(positionReticle, matViewProjectionViewport);

		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

		//
		Vector3 posScreen = ;

	}
	worldTransform_.UpdateMatrix();
	
}

void LockOn::Draw() {
	if (isSearch) {

		//ImGui::Text("Search and Destroy");
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

#include "LockOn.h"
#include"TextureManager.h"

#include <Input.h>
#include <iostream>
#include"MT_Matrix.h"

void LockOn::Initialize(uint32_t textureReticle) { 
	
	worldTransform_.Initialize();
	

	Vector2 anchorPoint = {0.5f, 0.5f};
	
	lockOnMark_->Create(textureReticle, {}, {1.0f, 1.0f, 1.0f, 1.0f}, anchorPoint);


}

void LockOn::Update(std::list<std::unique_ptr<Enemy>>& enemies,const ViewProjection& viewProjection ) {


	//ボタンを押したらロックオン
	XINPUT_STATE joyState;

	ZeroMemory(&joyState, sizeof(XINPUT_STATE));

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			Search(enemies, viewProjection);
		}

	}

	worldTransform_.UpdateMatrix();
	
}

void LockOn::Draw() {

	lockOnMark_->Draw(); 

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

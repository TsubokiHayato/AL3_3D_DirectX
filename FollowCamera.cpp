#include "FollowCamera.h"
#include "Input.h"

#include "LockOn.h"
#include "MT_Matrix.h"

inline Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
	return Vector3(
	    vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0], vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
	    vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]);
}
void FollowCamera::Initialize() { viewProjection_.Initialize(); }

void FollowCamera::Update() {

	
	
	if (lockOn_&&lockOn_->ExistTarget()) {

		Vector3 lockOnPos = lockOn_->GetTargetPos();
		Vector3 playerPos = target_->translation_;

		Vector3 sub = lockOnPos - playerPos;
		viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);

	    Vector3 offset{0.0f, 2.0f, -10.0f};
		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(viewProjection_.rotation_);

		offset = TransformNormal(offset, rotateMatrix);
		


		viewProjection_.translation_ = playerPos + offset;

	} else {
		// 追従対象がいれば
		if (target_) {
			
			Vector3 offset ={0.0f, 2.0f, -10.0f};
			Vector3 targetPos = target_->translation_;

			XINPUT_STATE joyState;
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				const float speed = 0.1f;

				Vector3 move{};
				move.x = 0;
				move.y += (float)(joyState.Gamepad.sThumbRX);
				move.z = 0;

				
					move = Normalize(move) * speed;
			

				viewProjection_.rotation_ += move;
			}

			Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(viewProjection_.rotation_);

			
			offset = TransformNormal(offset, rotateMatrix);

			
			viewProjection_.translation_ = targetPos + offset;
		}
	}

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}
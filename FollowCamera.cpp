#include "FollowCamera.h"
#include"Input.h"

#include "MT_Matrix.h"

inline Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
	return Vector3(
	    vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0], vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
	    vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]);
}
void FollowCamera::Initialize() {
	
	viewProjection_.Initialize(); 

}

void FollowCamera::Update() {

	
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float speed = 0.1f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * speed;
	}

	if (target_) {
		Vector3 offset{0.0f, 2.0f, -10.0f};

		Vector3 cameraRotationAngles = viewProjection_.rotation_;

		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(cameraRotationAngles.y);

		offset = TransformNormal(offset, rotateYMatrix);

		viewProjection_.translation_ = target_->translation_ + offset;
	}

	viewProjection_.UpdateMatrix();
	
}

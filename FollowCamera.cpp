#include "FollowCamera.h"

void FollowCamera::Initialize() { viewProjection->Initialize(); }

void FollowCamera::Update() {
	if (target_) {
		Vector3 offset{0.0f, 2.0f, -10.0f};

		viewProjection->translation_ = target_->translation_ + offset;
	}

	viewProjection->UpdateMatrix();
}

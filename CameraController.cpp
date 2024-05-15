#define NOMINMAX
#define NOMINMIN
#include "CameraController.h"

#include "Player.h"

#include <ImGuiManager.h>

#include <algorithm>


Vector3 lerp(Vector3 start, Vector3 end, float t) {
	Vector3 result = {};
	result.x = (1.0f - t) * start.x + t * end.x;
	result.y = (1.0f - t) * start.y + t * end.y;
	result.z = (1.0f - t) * start.z + t * end.z;
	return result;
}
void CameraController::Initialize(ViewProjection* viewProjection_) {

	//
	viewProjection = viewProjection_; 
	viewProjection->Initialize();


}

void CameraController::Update() {

	// target_のWorldTransformを取得
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();
	// ビュー行列の移動成分を更新
	goalPos.x = targetWorldTransform.translation_.x + targetOffset_.x+targetVelocity.x*kVelocityBias;
	goalPos.y = targetWorldTransform.translation_.y + targetOffset_.y+targetVelocity.y*kVelocityBias;
	goalPos.z = targetWorldTransform.translation_.z + targetOffset_.z+targetVelocity.z*kVelocityBias;
	
	viewProjection->translation_ = lerp(viewProjection->translation_, goalPos, kInterpolationRate);

	viewProjection->translation_.x = std::max(viewProjection->translation_.x,targetWorldTransform.translation_.x+margin. left);
	viewProjection->translation_.x = std::min(viewProjection->translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection->translation_.y = std::max(viewProjection->translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection->translation_.y = std::min(viewProjection->translation_.y, targetWorldTransform.translation_.y + margin.top);


	//移動範囲制限
	viewProjection->translation_.x = std::max(viewProjection->translation_.x, movableArea_.left);
	viewProjection->translation_.x = std::min(viewProjection->translation_.x, movableArea_.right);
	viewProjection->translation_.y = std::max(viewProjection->translation_.y, movableArea_.bottom);
	viewProjection->translation_.y = std::min(viewProjection->translation_.y, movableArea_.top);

	// ビュー行列と射影行列の更新
	viewProjection->UpdateMatrix();

//
//	ImGui::Text("cameraPos.x %f", viewProjection->translation_.x);
//	ImGui::Text("cameraPos.y %f", viewProjection->translation_.y);
//	ImGui::Text("cameraPos.z %f", viewProjection->translation_.z);
//	
}

void CameraController::Reset() {

	// 追従対象とワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追求対象とオフセットからカメラの座標を計算
	viewProjection->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

}


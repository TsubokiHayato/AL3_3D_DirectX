#include "Plane.h"

#include <cassert>

void Plane::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	viewProjection_ = viewProjection;
}

void Plane::Update() {
	worldTransform_.translation_ = {};
	worldTransform_.TransferMatrix(); }

void Plane::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

#include "Skydome.h"

#include <cassert>

void SkyDome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	// 3Dモデルの作成
	Sky_Model_ = model;

	
	// ワールドトランスフォームの初期化
	Sky_worldTransform_.Initialize();

	Sky_viewProjection_ = viewProjection;
}

void SkyDome::Update() { Sky_worldTransform_.TransferMatrix(); }
void SkyDome::Draw() { Sky_Model_->Draw(Sky_worldTransform_, *Sky_viewProjection_); }
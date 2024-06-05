#pragma once

#include "Model.h"

#include "WorldTransform.h"

#include <numbers>

#include "Input.h"

enum class LRDirection {
	kRight,
	kLeft
};

// 自キャラ
class Player {
public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection);
	// 更新
	void Update();
	// 描画
	void Draw();

	

	~Player();

	const Vector3& GetPosition() const { return worldTransform_.translation_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; };
	
	

private:

	Input* input_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_ = nullptr;

	// ビューポート
	ViewProjection* viewProjection_ = nullptr;


};

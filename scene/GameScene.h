#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <DebugCamera.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	/*------------
	    画像関連
	------------*/
	// テクスチャーハンドル
	uint32_t textureHandle = 0;
	// 3Dモデル
	Model* model = nullptr;
	// 2Dスプライト
	Sprite* sprite = nullptr;

	/*------------
	     音楽
	------------*/
	uint32_t soundDataHandle = 0;
	uint32_t voiceHandle = 0;

	WorldTransform worldTransform;
	ViewProjection viewProjection;
	float inputFloat3[3] = {0, 0, 0};

	// デバッグカメラ
	DebugCamera* debugCamera = nullptr;
};

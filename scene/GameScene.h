#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"


#include "Skydome.h"

#include <DebugCamera.h>
#include "Player.h"

#include<vector>
#include<MapChipField.h>

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
	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>


	

	/*-----------
	DebugCamera
	-----------*/
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;


	/*-----------
	　 キャラクタ
	-----------*/
	Player* player = nullptr;

	Skydome* skydome = nullptr;

	MapChipField* mapChipField_;

	/*------------
	テクスチャ&モデル
	------------*/
	uint32_t textureHandle = 0;

	Model* model = nullptr;

	Model* modelBlock_ = nullptr;

	Model* modelSkydome_ = nullptr;

	
	/*--------------
	* ワールド・ビュー
	--------------*/

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	ViewProjection viewProjection_;

};

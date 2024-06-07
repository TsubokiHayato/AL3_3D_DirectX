#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"



#include <DebugCamera.h>
#include "Player.h"
#include<Enemy.h>

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

	//std::unique_ptr<Model> model_;

	//model_ = std::make_unique<Model>();
	//osusume
	/*-----------
	　 キャラクタ
	-----------*/
	Player* player = nullptr;
	Enemy* enemy = nullptr;

	


	/*------------
	テクスチャ&モデル
	------------*/
	
	Model* modelPlayer = nullptr;
	Model* modelEnemy = nullptr;

	
	
	/*--------------
	* ワールド・ビュー
	--------------*/

	
	ViewProjection viewProjection_;


	uint32_t textureHandle = 0;

};

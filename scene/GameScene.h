#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"


#include <DebugCamera.h>
#include "Player.h"
#include"Enemy.h"
#include"skydome.h"

#include<WorldTransform.h>
#include "ViewProjection.h"

#include"RailCamera.h"
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
	

	void CheckAllCollisions();

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>


	RailCamera* railCamera = nullptr;
	Vector3 cameraPos = {0.0f, 0.0f, 1.0f};
	Vector3 cameraRotate = {0.0f,0.0f,0.0f};
	

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

	Skydome* skydome = nullptr;


	/*------------
	テクスチャ&モデル
	------------*/
	
	Model* modelPlayer = nullptr;
	Model* modelEnemy = nullptr;

	Model* modelSkydome = nullptr;
	
	/*--------------
	* ワールド・ビュー
	--------------*/

	
	ViewProjection viewProjection_;
	WorldTransform worldTransform;

	uint32_t textureHandle = 0;

};

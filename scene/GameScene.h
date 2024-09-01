#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"

#include<sstream>
#include<fstream>


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
/// 
enum Scene {
	Title,
	Battle
};
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

	void AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }

	// 弾リストの処理を参考に作成する
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	//ini
	void LoadEnemyPopData();

	//up
	void UpdateEnemyPopCommands();

	void enemyPop(Vector3 translation);

	void RestartEnemyPopCommands();


private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>


	RailCamera* railCamera = nullptr;
	Vector3 cameraPos = {0.0f, 0.0f, -100.0f};
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

	std::list<EnemyBullet*> enemyBullets_;

	/*------------
	テクスチャ&モデル
	------------*/
	Model* modelTitle = nullptr;

	Model* modelPlayer = nullptr;
	Model* modelEnemy = nullptr;
	Model* modelTarget = nullptr;

	Model* modelSkydome = nullptr;
	
	Model* modelEnemyBullet = nullptr;
	Model* modelPlayerBullet = nullptr;

	/*--------------
	* ワールド・ビュー
	--------------*/

	
	ViewProjection viewProjection_;
	WorldTransform worldTransform;

	
	uint32_t soundDataHandle_ = 0;


	std::list<Enemy*> enemies_;

	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool isWait = false;

	// 待機タイマー
	int32_t kWaitTimer_ = 0;

	Scene scene = Title;

	bool shouldRestartCommands = false;
	int killEnemyCount = 15;
};

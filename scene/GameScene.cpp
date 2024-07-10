#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <ImGuiManager.h>
#include <cassert>

void GameScene::CheckAllCollisions() {

	Vector3 playerPos, playerBulletPos, enemyPos, enemyBulletPos;

	const std::list<PlayerBullet*>& playerBullets = player->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_ = GetBullets();

#pragma region Player & EnemyBullets Colision

	// 自キャラの座標
	playerPos = player->GetWorldPos();

	for (EnemyBullet* enemyBullet : enemyBullets) {

		enemyBulletPos = enemyBullet->GetWorldPos();

		float distance = powf((enemyBulletPos.x - playerPos.x), 2.0f) + powf((enemyBulletPos.y - playerPos.y), 2.0f) + powf((enemyBulletPos.z - playerPos.z), 2.0f);
		float length = 1.0f;
		if (distance <= length) {

			player->OnCollision();
			enemyBullet->OnCollision();
		}
	}

#pragma endregion
#pragma region PlayerBullets & Enemy Colision

	// enemyの座標
	enemyPos = enemy->GetWorldPos();

	for (PlayerBullet* playerBullet : playerBullets) {

		playerBulletPos = playerBullet->GetWorldPos();

		float distance = powf((playerBulletPos.x - enemyPos.x), 2.0f) + powf((playerBulletPos.y - enemyPos.y), 2.0f) + powf((playerBulletPos.z - enemyPos.z), 2.0f);
		float length = 1.0f;
		if (distance <= length) {

			enemy->OnCollision();
			playerBullet->OnCollision();
		}
	}

#pragma endregion
#pragma region PlayerBullets & EnemyBullet Colision

	for (PlayerBullet* playerBullet : playerBullets) {

		playerBulletPos = playerBullet->GetWorldPos();

		for (EnemyBullet* enemyBullet : enemyBullets) {

			enemyBulletPos = enemyBullet->GetWorldPos();

			float distance = powf((playerBulletPos.x - enemyBulletPos.x), 2.0f) + powf((playerBulletPos.y - enemyBulletPos.y), 2.0f) + powf((playerBulletPos.z - enemyBulletPos.z), 2.0f);
			float length = 1.0f;
			if (distance <= length) {

				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete debugCamera_;
	delete railCamera;
	delete player;
	delete enemy;
	delete skydome;
	// 3Dモデル削除

	delete modelPlayer;
	delete modelEnemy;
	delete modelSkydome;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/*-------------------
	       　Axis
	    -------------------*/
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	/*--------------
	* ワールド・ビュー
	--------------*/

	viewProjection_.farZ = 1145;

	viewProjection_.Initialize();
	worldTransform.Initialize();

	railCamera = new RailCamera;
	railCamera->Initialize(cameraPos, cameraRotate);

	/*---------
	  SkyDome
	---------*/

	// SkyDome作成
	skydome = new Skydome;

	modelSkydome = Model::CreateFromOBJ("SkyDome", true);

	skydome->Initialize(modelSkydome, &viewProjection_);

	/*---------
	* Chara
	--------*/
	
	// 自キャラの生成
	player = new Player;
	// ポーションの画像
	textureHandle = TextureManager::Load("Recovery_agents.png");

	modelPlayer = Model::Create();

	player->SetParent(&railCamera->GetWorldTransform());
	Vector3 playerPos(0, 0, 10);
	
	// 自キャラの初期化
	player->Initialize(modelPlayer, textureHandle, &viewProjection_,playerPos);

	enemy = new Enemy;
	modelEnemy = Model::Create();
	// Enemyの初期化
	enemy->Initialize(modelEnemy, &viewProjection_);
	enemy->SetGameScene(this);
	enemy->SetPlayer(player);

	/*-----------
	 DEBUG_CAMERA
	-----------*/
	debugCamera_ = new DebugCamera(1280, 720);
}

void GameScene::Update() {

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	/*-----------
	DebugCamera
	-----------*/
#ifdef _DEBUG

	if (input_->TriggerKey(DIK_Q)) {
		isDebugCameraActive_ = true;
	}
#endif

	if (isDebugCameraActive_) {

		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetView();
		viewProjection_.matProjection = debugCamera_->GetProjection();

		//
		viewProjection_.TransferMatrix();
	} else {
		//
		viewProjection_.UpdateMatrix();
	}

	railCamera->Update();

	viewProjection_.matView = railCamera->GetView();
	viewProjection_.matProjection = railCamera->GetProjection();

	viewProjection_.TransferMatrix();

	/*----------
	     3D
	----------*/

	skydome->Update();

	// 自キャラの更新
	player->Update();

	// enemyの更新
	enemy->Update();
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	///

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	///

	/*-----------
	     3D
	-----------*/
	// 自キャラ
	player->Draw();
	// Enemy
	enemy->Draw();
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	skydome->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

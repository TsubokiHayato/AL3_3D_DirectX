#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <ImGuiManager.h>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete railCamera;
	delete player;

	delete skydome;
	// 3Dモデル削除

	delete modelPlayer;
	delete modelEnemy;
	delete modelSkydome;
	delete modelTarget;
	delete modelEnemyBullet;

	delete modelTitle;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (Enemy* enemy_ : enemies_) {
		delete enemy_;
	}
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

	modelTitle = Model::CreateFromOBJ("title", true);

	shouldRestartCommands = true;
	LoadEnemyPopData();

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
	TextureManager::Load("2D_Reticle.png");
	// 自キャラの生成
	player = new Player;
	// ポーションの画像
	// textureHandle = TextureManager::Load("Recovery_agents.png");

	modelPlayer = Model::CreateFromOBJ("float_Head", true);

	modelTarget = Model::Create();

	player->SetParent(&railCamera->GetWorldTransform());
	Vector3 playerPos(0, 0, 30);

	// 自キャラの初期化
	player->Initialize(modelPlayer, modelTarget, &viewProjection_, playerPos);

	modelEnemy = Model::CreateFromOBJ("needle_Body", true);

	modelEnemyBullet = Model::CreateFromOBJ("needle_Body", true);

	killEnemyCount = 15;

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

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy_) {
		if (enemy_->IsDead()) {
			delete enemy_;
			return true;
		}
		return false;
	});

	switch (scene) {
	case Title:
		enemies_.remove_if([](Enemy* enemy_) { return true; });
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			return true;
		});
		LoadEnemyPopData();
		if (input_->PushKey(DIK_SPACE)) {
			scene = Battle;
		}
		break;
	case Battle:

		railCamera->Update();

		viewProjection_.matView = railCamera->GetView();
		viewProjection_.matProjection = railCamera->GetProjection();

		viewProjection_.TransferMatrix();

		/*----------
		     3D
		----------*/

		skydome->Update();

		// 自キャラの更新
		player->Update(viewProjection_.matView, viewProjection_.matProjection);

		UpdateEnemyPopCommands();

		for (Enemy* enemy_ : enemies_) {

			enemy_->Update();
		}

		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Update();
		}
		CheckAllCollisions();
		if (killEnemyCount <= 0) {

			shouldRestartCommands = true;
			Initialize();
			scene = Title;
		}

		break;
	}
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

	switch (scene) {
	case Title:
		modelTitle->Draw(worldTransform, viewProjection_);
		break;
	case Battle:
		// 自キャラ
		player->Draw();

		// Enemy
		for (Enemy* enemy_ : enemies_) {
			enemy_->Draw();
		}

		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}

		break;
	}

	skydome->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	switch (scene) {
	case Battle:
		player->DrawUI();
		break;
	}
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::LoadEnemyPopData() {

	std::ifstream file;
	file.open("enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// コマンドの再実行が必要ならば、リセットを行う
	if (shouldRestartCommands) {
		RestartEnemyPopCommands();
		shouldRestartCommands = false;
	}

	// 待機処理
	if (isWait) {
		kWaitTimer_--;
		if (kWaitTimer_ <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			enemyPop(Vector3(x, y, z));
			// WAITコマンド
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			kWaitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::RestartEnemyPopCommands() {
	// ストリームの位置をリセットして最初から再開
	enemyPopCommands.clear();  // エラーフラグのクリア
	enemyPopCommands.seekg(0); // ストリームの位置を先頭に移動
	isWait = false;            // 待機状態を解除
	kWaitTimer_ = 0;           // タイマーをリセット
}

void GameScene::enemyPop(Vector3 translation) {

	// 敵キャラの生成
	enemy = new Enemy();
	// 敵キャラの初期化
	enemy->Initialize(modelEnemy, modelEnemyBullet, &viewProjection_, translation);
	// 敵キャラにゲームシーンを渡す
	enemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player);

	enemies_.push_back(enemy);
}

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
		float length = 3.0f;
		if (distance <= length) {

			player->OnCollision();

			enemyBullet->OnCollision();
			shouldRestartCommands = true; // コマンドを再実行するフラグを立てる
			scene = Title;                // タイトルに戻る
			return;                       // ここで戻ることでコマンドの読み込みを中断する
		}
	}

#pragma endregion
#pragma region PlayerBullets & Enemy Colision

	// enemyの座標
	for (Enemy* enemy_ : enemies_) {

		enemyPos = enemy_->GetWorldPos();

		for (PlayerBullet* playerBullet : playerBullets) {

			playerBulletPos = playerBullet->GetWorldPos();

			float distance = powf((playerBulletPos.x - enemyPos.x), 2.0f) + powf((playerBulletPos.y - enemyPos.y), 2.0f) + powf((playerBulletPos.z - enemyPos.z), 2.0f);
			float length = 3.0f;
			if (distance <= length) {

				enemy_->OnCollision();
				playerBullet->OnCollision();
				killEnemyCount--;
			}
		}
	}

#pragma endregion
#pragma region PlayerBullets & EnemyBullet Colision

	for (PlayerBullet* playerBullet : playerBullets) {

		playerBulletPos = playerBullet->GetWorldPos();

		for (EnemyBullet* enemyBullet : enemyBullets) {

			enemyBulletPos = enemyBullet->GetWorldPos();

			float distance = powf((playerBulletPos.x - enemyBulletPos.x), 2.0f) + powf((playerBulletPos.y - enemyBulletPos.y), 2.0f) + powf((playerBulletPos.z - enemyBulletPos.z), 2.0f);
			float length = 3.0f;
			if (distance <= length) {

				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion
}

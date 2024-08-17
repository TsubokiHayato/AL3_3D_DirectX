#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	/*-----
	 model
	-----*/
	modelPlayer_Head.reset(Model::CreateFromOBJ("float_Head", true));
	modelPlayer_Body.reset(Model::CreateFromOBJ("float_Body", true));
	modelPlayer_LeftArm.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelPlayer_RightArm.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelHammer.reset(Model::CreateFromOBJ("hammer", true));

	std::vector<Model*> playerModels = {modelPlayer_Head.get(), modelPlayer_Body.get(), modelPlayer_LeftArm.get(), modelPlayer_RightArm.get(), modelHammer.get()};

	modelEnemy_Head.reset(Model::CreateFromOBJ("needle_Body", true));
	modelEnemy_LeftArm.reset(Model::Create());
	modelEnemy_RightArm.reset(Model::Create());

	std::vector<Model*> enemyModels = {
	    modelEnemy_Head.get(),
	    modelEnemy_LeftArm.get(), // 腕は今使ってないから注意
	    modelEnemy_RightArm.get(),
	};

	/*--------------
	* ワールド・ビュー
	--------------*/

	viewProjection_.Initialize();

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize(textureReticle);

	/*---------
	* Chara
	--------*/

	// 自キャラの生成
	player = std::make_unique<Player>();

	player->SetLockOn(lockOn_.get());
	// 自キャラの初期化
	player->Initialize(playerModels, &viewProjection_);

	Vector3 enemiesPos = {
	    0, 1, 1

	};
	enemies_.push_back(std::make_unique<Enemy>());

	for (const std::unique_ptr<Enemy>& enemy_ : enemies_) {
		enemiesPos.x += 3;

		enemiesPos.z += 3;
		enemy_->Initialize(enemyModels, &viewProjection_, enemiesPos);
	}

	textureReticle = TextureManager::Load("2D_Reticle.png");

	/*---------
	  SkyDome
	---------*/
	modelSkyDome.reset(Model::CreateFromOBJ("SkyDome", true));

	skyDome = std::make_unique<SkyDome>();

	skyDome->Initialize(modelSkyDome.get(), &viewProjection_);

	/*---------
	  plane
	---------*/
	modelPlane.reset(Model::CreateFromOBJ("plane", true));

	plane = std::make_unique<Plane>();

	plane->Initialize(modelPlane.get(), &viewProjection_);

	/*-----------
	 CAMERA
	-----------*/
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initialize();

	followCamera->SetTarget(&player->GetWorldTransform());
	followCamera->SetLockOn(lockOn_.get());
	player->SetViewProjection(&followCamera->GetViewProjection());


	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
}

void GameScene::Update() {

	/*-----------
	DebugCamera
	-----------*/
#pragma region camera
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

#pragma endregion
	collisionManager_->UpdateWorldTransform();
	/*----------
	     3D
	----------*/
	// 自キャラの更新
	player->Update();
	skyDome->Update();
	plane->Update();

	for (const std::unique_ptr<Enemy>& enemy_ : enemies_) {
		enemy_->Update();
	}

	followCamera->Update();

	viewProjection_.matView = followCamera->GetView();
	viewProjection_.matProjection = followCamera->GetProjection();

	viewProjection_.TransferMatrix();

	lockOn_->Update(enemies_, viewProjection_);

	
	//衝突判定と応答
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

	for (const std::unique_ptr<Enemy>& enemy_ : enemies_) {
		enemy_->Draw();
	}

	skyDome->Draw();

	plane->Draw();
	collisionManager_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	lockOn_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

	//衝突マネージャーのリセット
	collisionManager_->Reset();

	//コライダーをリストに登録
	collisionManager_->AddCollider(player.get());
	//敵の全てについて
	for (std::unique_ptr<Enemy>& enemy_ : enemies_) {
		collisionManager_->AddCollider(enemy_.get());
	}

	//衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

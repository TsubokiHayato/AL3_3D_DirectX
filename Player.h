#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include"BaseCharacter.h"

#include"optional"


//  行動フェーズ
enum class Phase {
	Approach, // 接近
	Leave     // 離脱
};

enum class Behavior {
	kRoot,
	kAttack,
	kJump
};

class GlobalVariables;

class LockOn;
//自キャラ
class Player : public BaseCharacter {
public:

	// 初期化
	void Initialize(const std::vector<Model*>&models, ViewProjection* viewProjection);
	
	//更新
	void Update();
	
	//描画
	void Draw();


	void SetViewProjection(const ViewProjection* viewProjection) {  viewProjection_=viewProjection; }

	
	Vector3 GetWorldPos() {

		Vector3 result = {};
		result.x = worldTransform_.matWorld_.m[3][0];
		result.y = worldTransform_.matWorld_.m[3][1];
		result.z = worldTransform_.matWorld_.m[3][2];

		return result;
	};

	Vector3 GetCenterPos() const;

	void InitializeFloatingGimmick();

	void UpdateFloatingGimmick();


	WorldTransform& GetWorldTransform() { return worldTransform_; };
	
	void BehaviorRootInitialize();

	void BehaviorAttackInitialize();

	void BehaviorJumpInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackUpdate();

	void BehaviorJumpUpdate();

	void ApplyGlobalVariables();

	void SetLockOn(const LockOn* lockOn) { lockOn_ = lockOn; };

	void OnCollision() override;

private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	WorldTransform worldHeadTransform_;
	WorldTransform worldBodyTransform_;
	WorldTransform worldLArmTransform_;
	WorldTransform worldRArmTransform_;

	WorldTransform worldHammerTransform_;

	


	//3Dモデル
	Model* modelHead_ = nullptr;
	Model* modelBody_ = nullptr;
	Model* modelLeftArm_ = nullptr;
	Model* modelRightArm_ = nullptr;
	Model* modelHammer_ = nullptr;


	//ビューポート
	const ViewProjection* viewProjection_=nullptr;

	
	
	
	float floatingParameter_ = 0.0f;
	int period = 120;
	float step;
	float floatingSwing = 2.0f;

	static const int kModelIndexHead = 0;
	static const int kModelIndexBody = 1;
	static const int kModelIndexL_arm = 2;
	static const int kModelIndexR_arm = 3;
	static const int kModelIndexHammer = 4;

	Phase phase_ = Phase::Approach;

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	bool isAttack = false;



	GlobalVariables* globalVariables;

	Vector3 velocity_ = {};

	const LockOn* lockOn_ = nullptr;
	
};




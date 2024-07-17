#include <Player.h>
#include <cassert>
#include"winApp.h"

#include"MT_Matrix.h"

//初期化
void Player::Initialize(Model* modelHead, Model* modelBody, Model* modelLeftArm, Model* modelRightArm, ViewProjection* viewProjection) {
	
	assert(modelHead);
	assert(modelBody);
	assert(modelLeftArm);
	assert(modelRightArm);

	modelHead_=modelHead;
	modelBody_=modelBody;
	modelLeftArm_=modelLeftArm;
	modelRightArm_=modelRightArm;

	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();



	worldHeadTransform_.parent_ = &worldBodyTransform_;
	worldLArmTransform_.parent_ = &worldBodyTransform_;
	worldRArmTransform_.parent_ = &worldBodyTransform_;

	


	viewProjection_ = viewProjection;

}
// 更新
void Player::Update() { 


	
	
	

	

	


	Matrix4x4 worldMatrix = MakeAffineMatrix(worldBodyTransform_.scale_, worldBodyTransform_.rotation_, worldBodyTransform_.translation_);
	



}
//描画
void Player::Draw() {
	

	modelHead_->Draw(worldTransform_, *viewProjection_);
	modelBody_->Draw(worldTransform_, *viewProjection_);
	modelLeftArm_->Draw(worldTransform_, *viewProjection_);
	modelRightArm_->Draw(worldTransform_, *viewProjection_);
}
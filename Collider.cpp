#include "Collider.h"

void Collider::Initialize() {


	worldTransform.Initialize();

}

void Collider::UpdateWorldTransform() { 
	
	//ワールド座標をワールドトランスフォームに適用
	worldTransform.translation_ = GetCenterPos();
	//ワールド行列を再計算して定数バッファに書き込む
	worldTransform.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {
	
	model->Draw(worldTransform, viewProjection); 

}

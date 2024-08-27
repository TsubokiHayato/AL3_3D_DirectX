#pragma once
#include <vector>
#include <Model.h>
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"assert.h"
#include"Collider.h"

class BaseCharacter : public Collider {

	protected:
	std::vector<Model*> models_;

	WorldTransform worldTransform_;


	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	public:

		virtual void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection);
	    virtual void Update();
	    virtual void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

		//中心座標を入れる変数
	    virtual Vector3 GetCenterPos() const override;
};

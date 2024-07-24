#pragma once
#include"ViewProjection.h"
#include"WorldTransform.h"

class FollowCamera {
	
	public:

	void Initialize();
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	
	private:

	ViewProjection* viewProjection;
	   
	const WorldTransform* target_ = nullptr;
};

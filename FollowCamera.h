#pragma once
#include"ViewProjection.h"
#include"WorldTransform.h"

class FollowCamera {
	
	public:

	void Initialize();
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	Matrix4x4 GetView() { return viewProjection_.matView; }
	Matrix4x4 GetProjection() { return viewProjection_.matProjection; }

	
	
	private:

	ViewProjection viewProjection_;
	   
	const WorldTransform* target_ = nullptr;
};

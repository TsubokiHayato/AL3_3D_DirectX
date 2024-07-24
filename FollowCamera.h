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

	  const ViewProjection& GetViewProjection() const { return viewProjection_; }
	
	private:

	ViewProjection viewProjection_;
	   
	const WorldTransform* target_ = nullptr;
};

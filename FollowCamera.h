#pragma once
#include"ViewProjection.h"
#include"WorldTransform.h"

class LockOn;

class FollowCamera {

public:
	void Initialize();
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	Matrix4x4 GetView() { return viewProjection_.matView; }
	Matrix4x4 GetProjection() { return viewProjection_.matProjection; }

	  const ViewProjection& GetViewProjection() const { return viewProjection_; }

	  void SetLockOn(const LockOn* lockOn) { lockOn_ = lockOn; }
	
	private:

	ViewProjection viewProjection_;
	   
	const WorldTransform* target_ = nullptr;
	const LockOn* lockOn_ = nullptr;

};

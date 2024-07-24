#pragma once


#include<WorldTransform.h>
#include "ViewProjection.h"

class RailCamera {

public:

	void Initialize(Vector3 cameraPos,Vector3 cameraRotate);
	void Update();
	

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	Matrix4x4 GetView() { return viewProjection_.matView; }
	Matrix4x4 GetProjection() { return viewProjection_.matProjection; }
	

private:

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};

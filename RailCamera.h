#pragma once


#include<WorldTransform.h>
#include "ViewProjection.h"

class RailCamera {

public:

	void Initialize(Vector3 cameraPos,Vector3 cameraRotate);
	void Update();
	void Draw();

public:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};

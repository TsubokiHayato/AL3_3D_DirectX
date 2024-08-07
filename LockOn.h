#pragma once

#include"optional"
#include"Sprite.h"
#include"WorldTransform.h"
#include"ViewProjection.h"


#include<memory>
class LockOn {

	public:

	void Initialize();

	void Update();

	void Draw();


	private: 

		std::unique_ptr<Sprite> lockOnMark_;


};

#pragma once
#include <vector>
#include <Model.h>
#include"WorldTransform.h"
#include"ViewProjection.h"


class BaseCharacter {

	protected:
	std::vector<Model*> models_;

	WorldTransform worldTransform_;

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	public:

		virtual void Initialize(const std::vector<Model*>& models);
	    virtual void Update();
	    virtual void Draw(const ViewProjection& viewProjection);


};

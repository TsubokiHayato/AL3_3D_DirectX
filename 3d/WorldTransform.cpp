#include "WorldTransform.h"

#include "WorldTransform_MT.h"

void WorldTransform::UpdateVoidMatrix(WorldTransform* world) { MakeAffineMatrix(world->scale_, world->rotation_, world->translation_); }

Matrix4x4 WorldTransform::UpdateMatrix(WorldTransform* world) { 
	Matrix4x4 result;
	result=MakeAffineMatrix(world->scale_, world->rotation_, world->translation_);
	return result;
}
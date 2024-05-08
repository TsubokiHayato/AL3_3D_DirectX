#include "WorldTransform.h"

#include "WorldTransform_MT.h"

void WorldTransform::UpdateMatrix(WorldTransform* world) { MakeAffineMatrix(world->scale_, world->rotation_, world->translation_); }

Matrix4x4 WorldTransform::UpdateMatMatrix(WorldTransform* world) { 
	Matrix4x4 result = {};
	result=MakeAffineMatrix(world->scale_, world->rotation_, world->translation_); 
	return result;
}
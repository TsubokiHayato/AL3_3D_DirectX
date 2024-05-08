#include "WorldTransform.h"

#include "WorldTransform_MT.h"

void WorldTransform::UpdateMatrix(WorldTransform* world) { MakeAffineMatrix(world->scale_, world->rotation_, world->translation_); }
#include "WorldTransform.h"

#include<WorldTransform_EX.h>

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}

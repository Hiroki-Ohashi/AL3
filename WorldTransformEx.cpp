#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {

	// アフィン変換行列をワールド行列に代入
	matWorld_ = math_->MakeAffineMatrix(scale_, rotation_, translation_);

	// 行列を定数バッファに転送
	TransferMatrix();
}
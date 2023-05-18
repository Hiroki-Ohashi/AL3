#include "WorldTransform.h"
#include "MathFunction.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>

void WorldTransform::UpdateMatrix() {

	// スケーリング行列
	Matrix4x4 scale = MakeScaleMatrix(scale_);

	// X軸周りの回転行列
	Matrix4x4 rotateX = MakeRotateXMatrix(rotation_.x);
	// Y軸周りの回転行列
	Matrix4x4 rotateY = MakeRotateYMatrix(rotation_.y);
	// Z軸周りの回転行列
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotation_.z);
	// 回転行列
	Matrix4x4 rotateXYZ = Multiply(
	    MakeRotateXMatrix(rotation_.x),
	    Multiply(
	        MakeRotateYMatrix(rotation_.y),
	        MakeRotateZMatrix(rotation_.z)));

	// 平行移動行列
	Matrix4x4 translate = MakeTranslateMatrix(translation_);

	// アフィン変換行列をワールド行列に代入
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 行列を定数バッファに転送
	TransferMatrix();
}
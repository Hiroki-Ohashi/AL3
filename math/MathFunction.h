#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

// ベクトル変換
Vector3 TransfomNormal(const Vector3& v, const Matrix4x4& m);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 正規化
Vector3 Normalize(const Vector3& v);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
#include "MathFunction.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>



// ベクトル変換
Vector3 TransfomNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 MakeScaleMatrix;
	MakeScaleMatrix.m[0][0] = scale.x;
	MakeScaleMatrix.m[0][1] = 0.0f;
	MakeScaleMatrix.m[0][2] = 0.0f;
	MakeScaleMatrix.m[0][3] = 0.0f;

	MakeScaleMatrix.m[1][0] = 0.0f;
	MakeScaleMatrix.m[1][1] = scale.y;
	MakeScaleMatrix.m[1][2] = 0.0f;
	MakeScaleMatrix.m[1][3] = 0.0f;

	MakeScaleMatrix.m[2][0] = 0.0f;
	MakeScaleMatrix.m[2][1] = 0.0f;
	MakeScaleMatrix.m[2][2] = scale.z;
	MakeScaleMatrix.m[2][3] = 0.0f;

	MakeScaleMatrix.m[3][0] = 0.0f;
	MakeScaleMatrix.m[3][1] = 0.0f;
	MakeScaleMatrix.m[3][2] = 0.0f;
	MakeScaleMatrix.m[3][3] = 1.0f;

	return MakeScaleMatrix;
}
// X軸周りの回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 MakeRotateXMatrix;
	MakeRotateXMatrix.m[0][0] = 1;
	MakeRotateXMatrix.m[0][1] = 0;
	MakeRotateXMatrix.m[0][2] = 0;
	MakeRotateXMatrix.m[0][3] = 0;

	MakeRotateXMatrix.m[1][0] = 0;
	MakeRotateXMatrix.m[1][1] = std::cos(radian);
	MakeRotateXMatrix.m[1][2] = std::sin(radian);
	MakeRotateXMatrix.m[1][3] = 0;

	MakeRotateXMatrix.m[2][0] = 0;
	MakeRotateXMatrix.m[2][1] = -std::sin(radian);
	MakeRotateXMatrix.m[2][2] = std::cos(radian);
	MakeRotateXMatrix.m[2][3] = 0;

	MakeRotateXMatrix.m[3][0] = 0;
	MakeRotateXMatrix.m[3][1] = 0;
	MakeRotateXMatrix.m[3][2] = 0;
	MakeRotateXMatrix.m[3][3] = 1;

	return MakeRotateXMatrix;
}
// Y軸周りの回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 MakeRotateYMatrix;
	MakeRotateYMatrix.m[0][0] = std::cos(radian);
	MakeRotateYMatrix.m[0][1] = 0;
	MakeRotateYMatrix.m[0][2] = -std::sin(radian);
	MakeRotateYMatrix.m[0][3] = 0;

	MakeRotateYMatrix.m[1][0] = 0;
	MakeRotateYMatrix.m[1][1] = 1;
	MakeRotateYMatrix.m[1][2] = 0;
	MakeRotateYMatrix.m[1][3] = 0;

	MakeRotateYMatrix.m[2][0] = std::sin(radian);
	MakeRotateYMatrix.m[2][1] = 0;
	MakeRotateYMatrix.m[2][2] = std::cos(radian);
	MakeRotateYMatrix.m[2][3] = 0;

	MakeRotateYMatrix.m[3][0] = 0;
	MakeRotateYMatrix.m[3][1] = 0;
	MakeRotateYMatrix.m[3][2] = 0;
	MakeRotateYMatrix.m[3][3] = 1;

	return MakeRotateYMatrix;
}
// Z軸周りの回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 MakeRotateZMatrix;
	MakeRotateZMatrix.m[0][0] = std::cos(radian);
	MakeRotateZMatrix.m[0][1] = std::sin(radian);
	MakeRotateZMatrix.m[0][2] = 0;
	MakeRotateZMatrix.m[0][3] = 0;

	MakeRotateZMatrix.m[1][0] = -std::sin(radian);
	MakeRotateZMatrix.m[1][1] = std::cos(radian);
	MakeRotateZMatrix.m[1][2] = 0;
	MakeRotateZMatrix.m[1][3] = 0;

	MakeRotateZMatrix.m[2][0] = 0;
	MakeRotateZMatrix.m[2][1] = 0;
	MakeRotateZMatrix.m[2][2] = 1;
	MakeRotateZMatrix.m[2][3] = 0;

	MakeRotateZMatrix.m[3][0] = 0;
	MakeRotateZMatrix.m[3][1] = 0;
	MakeRotateZMatrix.m[3][2] = 0;
	MakeRotateZMatrix.m[3][3] = 1;

	return MakeRotateZMatrix;
}
// 回転行列
Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 m4;
	m4.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	             m1.m[0][3] * m2.m[3][0];
	m4.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	             m1.m[0][3] * m2.m[3][1];
	m4.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	             m1.m[0][3] * m2.m[3][2];
	m4.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	             m1.m[0][3] * m2.m[3][3];

	m4.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	             m1.m[1][3] * m2.m[3][0];
	m4.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	             m1.m[1][3] * m2.m[3][1];
	m4.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	             m1.m[1][3] * m2.m[3][2];
	m4.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	             m1.m[1][3] * m2.m[3][3];

	m4.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	             m1.m[2][3] * m2.m[3][0];
	m4.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	             m1.m[2][3] * m2.m[3][1];
	m4.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	             m1.m[2][3] * m2.m[3][2];
	m4.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	             m1.m[2][3] * m2.m[3][3];

	m4.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	             m1.m[3][3] * m2.m[3][0];
	m4.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	             m1.m[3][3] * m2.m[3][1];
	m4.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	             m1.m[3][3] * m2.m[3][2];
	m4.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	             m1.m[3][3] * m2.m[3][3];
	return m4;
}
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 MakeTranslateMatrix;
	MakeTranslateMatrix.m[0][0] = 1.0f;
	MakeTranslateMatrix.m[0][1] = 0.0f;
	MakeTranslateMatrix.m[0][2] = 0.0f;
	MakeTranslateMatrix.m[0][3] = 0.0f;

	MakeTranslateMatrix.m[1][0] = 0.0f;
	MakeTranslateMatrix.m[1][1] = 1.0f;
	MakeTranslateMatrix.m[1][2] = 0.0f;
	MakeTranslateMatrix.m[1][3] = 0.0f;

	MakeTranslateMatrix.m[2][0] = 0.0f;
	MakeTranslateMatrix.m[2][1] = 0.0f;
	MakeTranslateMatrix.m[2][2] = 1.0f;
	MakeTranslateMatrix.m[2][3] = 0.0f;

	MakeTranslateMatrix.m[3][0] = translate.x;
	MakeTranslateMatrix.m[3][1] = translate.y;
	MakeTranslateMatrix.m[3][2] = translate.z;
	MakeTranslateMatrix.m[3][3] = 1.0f;

	return MakeTranslateMatrix;
}
//  アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 m4;
	Matrix4x4 m1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m1.m[i][j] = 0;
		}
	}
	m1.m[0][0] = 1;
	m1.m[1][1] = std::cos(rotate.x);
	m1.m[1][2] = std::sin(rotate.x);
	m1.m[2][1] = -std::sin(rotate.x);
	m1.m[2][2] = std::cos(rotate.x);
	m1.m[3][3] = 1;

	Matrix4x4 m2;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m2.m[i][j] = 0;
		}
	}
	m2.m[0][0] = std::cos(rotate.y);
	m2.m[1][1] = 1;
	m2.m[2][2] = std::cos(rotate.y);
	m2.m[0][2] = -std::sin(rotate.y);
	m2.m[2][0] = std::sin(rotate.y);
	m2.m[3][3] = 1;

	Matrix4x4 m3;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m3.m[i][j] = 0;
		}
	}
	m3.m[0][0] = std::cos(rotate.z);
	m3.m[2][2] = 1;
	m3.m[1][1] = std::cos(rotate.z);
	m3.m[1][0] = -std::sin(rotate.z);
	m3.m[0][1] = std::sin(rotate.z);
	m3.m[3][3] = 1;

	Matrix4x4 xyz = Multiply(m1, Multiply(m2, m3));

	m4.m[0][0] = xyz.m[0][0] * scale.x;
	m4.m[0][1] = xyz.m[0][1] * scale.x;
	m4.m[0][2] = xyz.m[0][2] * scale.x;

	m4.m[1][0] = xyz.m[1][0] * scale.y;
	m4.m[1][1] = xyz.m[1][1] * scale.y;
	m4.m[1][2] = xyz.m[1][2] * scale.y;

	m4.m[2][0] = xyz.m[2][0] * scale.z;
	m4.m[2][1] = xyz.m[2][1] * scale.z;
	m4.m[2][2] = xyz.m[2][2] * scale.z;

	m4.m[3][0] = translate.x;
	m4.m[3][1] = translate.y;
	m4.m[3][2] = translate.z;
	m4.m[0][3] = 0;
	m4.m[1][3] = 0;
	m4.m[2][3] = 0;
	m4.m[3][3] = 1.0f;

	return m4;
};
// 正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 Normalize;

	float mag = 1 / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	Normalize = {v.x * mag, v.y * mag, v.z * mag};

	return Normalize;
};

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	float m4s;
	Matrix4x4 m4;
	m4s = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	      m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	      m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
	      m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	      m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	      m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
	      m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	      m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	      m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
	      m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	      m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	      m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	      m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	      m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	      m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
	      m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	      m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	      m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
	      m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	      m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	      m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
	      m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	      m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	      m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	m4.m[0][0] = 1 / m4s *
	             (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	              m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	              m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	m4.m[0][1] = 1 / m4s *
	             (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
	              m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	              m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	m4.m[0][2] = 1 / m4s *
	             (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	              m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	              m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	m4.m[0][3] = 1 / m4s *
	             (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
	              m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	              m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	// 2
	m4.m[1][0] = 1 / m4s *
	             (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
	              m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	              m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	m4.m[1][1] = 1 / m4s *
	             (+m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	              m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	              m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	m4.m[1][2] = 1 / m4s *
	             (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
	              m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	              m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	m4.m[1][3] = 1 / m4s *
	             (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	              m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	              m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

	m4.m[2][0] = 1 / m4s *
	             (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	              m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	              m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	m4.m[2][1] = 1 / m4s *
	             (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
	              m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	              m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	m4.m[2][2] = 1 / m4s *
	             (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	              m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	              m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	m4.m[2][3] = 1 / m4s *
	             (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
	              m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	              m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

	m4.m[3][0] = 1 / m4s *
	             (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
	              m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	              m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	m4.m[3][1] = 1 / m4s *
	             (+m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	              m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	              m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	m4.m[3][2] = 1 / m4s *
	             (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
	              m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	              m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	m4.m[3][3] = 1 / m4s *
	             (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	              m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	              m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return m4;
};
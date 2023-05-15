#include "Player.h"
#include "ImGuiManager.h"
#include "Matrix4x4.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <cassert>
#include <cmath>

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
	Matrix4x4 MakeAffineMatrix;

	MakeRotateXMatrix(rotate.x);
	MakeRotateYMatrix(rotate.y);
	MakeRotateZMatrix(rotate.z);

	Matrix4x4 XYZ = Multiply(
	    MakeRotateXMatrix(rotate.x),
	    Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	MakeAffineMatrix.m[0][0] = XYZ.m[0][0] * scale.x;
	MakeAffineMatrix.m[0][1] = XYZ.m[0][1] * scale.x;
	MakeAffineMatrix.m[0][2] = XYZ.m[0][2] * scale.x;
	MakeAffineMatrix.m[0][3] = 0;
	MakeAffineMatrix.m[1][0] = XYZ.m[1][0] * scale.y;
	MakeAffineMatrix.m[1][1] = XYZ.m[1][1] * scale.y;
	MakeAffineMatrix.m[1][2] = XYZ.m[1][2] * scale.y;
	MakeAffineMatrix.m[1][3] = 0;
	MakeAffineMatrix.m[2][0] = XYZ.m[2][0] * scale.z;
	MakeAffineMatrix.m[2][1] = XYZ.m[2][1] * scale.z;
	MakeAffineMatrix.m[2][2] = XYZ.m[2][2] * scale.z;
	MakeAffineMatrix.m[2][3] = 0;
	MakeAffineMatrix.m[3][0] = translate.x;
	MakeAffineMatrix.m[3][1] = translate.y;
	MakeAffineMatrix.m[3][2] = translate.z;
	MakeAffineMatrix.m[3][3] = 1;

	return MakeAffineMatrix;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y -= move.y;

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// スケーリング行列
	Matrix4x4 scale = MakeScaleMatrix(worldTransform_.scale_);

	// X軸周りの回転行列
	Matrix4x4 rotateX = MakeRotateXMatrix(worldTransform_.rotation_.x);
	// Y軸周りの回転行列
	Matrix4x4 rotateY = MakeRotateYMatrix(worldTransform_.rotation_.y);
	// Z軸周りの回転行列
	Matrix4x4 rotateZ = MakeRotateZMatrix(worldTransform_.rotation_.z);
	// 回転行列
	Matrix4x4 rotateXYZ = Multiply(
	    MakeRotateXMatrix(worldTransform_.rotation_.x),
	    Multiply(
	        MakeRotateYMatrix(worldTransform_.rotation_.y),
	        MakeRotateZMatrix(worldTransform_.rotation_.z)));

	// 平行移動行列
	Matrix4x4 translate = MakeTranslateMatrix(worldTransform_.translation_);

	// アフィン変換行列をワールド行列に代入
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player pos");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", &worldTransform_.translation_.x, -18.0f, 1.0f);
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
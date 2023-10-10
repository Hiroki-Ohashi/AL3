#include "RailCamera.h"
#include "MathFunction.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 pos, Vector3 rot) { 
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 10000.0f;
	viewProjection_.Initialize();

	// ワールドトランスフォームの初期設定
	worldTransform_.translation_.x = pos.x;
	worldTransform_.translation_.y = pos.y;
	worldTransform_.translation_.z = pos.z - 50;

	worldTransform_.rotation_ = rot;
}

void RailCamera::Update() { 

	//worldTransform_.translation_.z -= 0.1f;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	// float3スライダー
	ImGui::SliderFloat3("Position", &worldTransform_.translation_.x, -18.0f, 34.0f);
	// float3スライダー
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotation_.x, -18.0f, 34.0f);
	ImGui::End();
}


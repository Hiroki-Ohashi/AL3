#pragma once
#include "Model.h"
#include "WorldTransform.h"

class RailCamera {
public:
	void Initialize(Vector3 pos, Vector3 rot);

	void Update();

	const ViewProjection GetViewProjection() { return viewProjection_; }
	// ワールド座標を取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};

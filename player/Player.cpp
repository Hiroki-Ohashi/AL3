
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include <player/Player.h>
void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインタチェック
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	viewProjection.Initialize();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle_ = TextureManager::Load("Reticle.png");

	// スプライト生成
	sprite2DReicle_ = Sprite::Create(textureReticle_, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5});
}

// デストラクタ
Player::~Player() {
	// bullet_の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	delete sprite2DReicle_;
}

void Player::Update() {

	// デスフラグんお立った弾を排除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 回転処理
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

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

	// ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y -= (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y -= move.y;

	// 移動限界座標
	const float kMoveLimitX = 20;
	const float kMoveLimitY = 10;

	// 範囲超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	//{
	//	// 自機から3Dレティクルへの距離
	//	const float kDistancePlayerTo3DReticle = 50.0f;
	//	// 自機から3Dレティクルへのオフセット(Z+向き)
	//	Vector3 offset = {0, 0, 1.0f};
	//	// 自機のワールド行列の回転を反映
	//	offset = TransformNormal(offset, worldTransform_.matWorld_);
	//	// ベクトルの長さを変える
	//	offset.x = Normalize(offset).x * kDistancePlayerTo3DReticle;
	//	offset.y = Normalize(offset).y * kDistancePlayerTo3DReticle;
	//	offset.z = Normalize(offset).z * kDistancePlayerTo3DReticle;
	//	// 3Dレティクルの座標を設定
	//	worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
	//	worldTransform3DReticle_.UpdateMatrix();
	//}

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		// Vector3 positionReticle = Get3DWorldPosition();

		//// ビューポート行列
		// Matrix4x4 matViewport =
		//     MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		//// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		// Matrix4x4 matViewProjectionViewport =
		//     Multiply(viewProjection.matView, Multiply(viewProjection.matProjection,
		//     matViewport));

		//// ワールド→スクリーン座標変換
		// positionReticle = Transform(positionReticle, matViewProjectionViewport);

		//// スプライトのレティクルに座標設定
		// sprite2DReicle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	//{
	//	// ビューポート行列
	//	Matrix4x4 matViewport =
	//	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//	POINT mousePosition;

	//	// マウス座標（スクリーン座標）を取得する
	//	GetCursorPos(&mousePosition);

	//	// クライアントエリア座標に変換する
	//	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//	ScreenToClient(hwnd, &mousePosition);

	//	// スプライトのレティクルに座標設定
	//	sprite2DReicle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));

	//	// ビュープロジェクションビューポート合成行列
	//	Matrix4x4 matVPV =
	//	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//	// 合成行列の逆行列を計算する

	//	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//	// スクリーン座標
	//	Vector3 posNear = Vector3(static_cast<float>(mousePosition.x), (float)mousePosition.y, 0);
	//	Vector3 posFar = Vector3(static_cast<float>(mousePosition.x), float(mousePosition.y), 1);
	//	// スクリーン座標系からワールド座標系へ
	//	posNear = Transform(posNear, matInverseVPV);
	//	posFar = Transform(posFar, matInverseVPV);

	//	// マウスレイの方向
	//	Vector3 mouseDierection;
	//	mouseDierection.x = posFar.x - posNear.x;
	//	mouseDierection.y = posFar.y - posNear.y;
	//	mouseDierection.z = posFar.z - posNear.z;
	//	mouseDierection = Normalize(mouseDierection);
	//	// カメラから照準オブジェクトの距離
	//	const float kDistanceTextObject = 100.0f;
	//	worldTransform3DReticle_.translation_.x = mouseDierection.x * kDistanceTextObject;
	//	worldTransform3DReticle_.translation_.y = mouseDierection.y * kDistanceTextObject;
	//	worldTransform3DReticle_.translation_.z = mouseDierection.z * kDistanceTextObject;

	//	worldTransform3DReticle_.UpdateMatrix();

	//	ImGui::Begin("Player");
	//	ImGui::Text("2DReticle:(%2.2f,%2.2f)", mousePosition.x, mousePosition.y);
	//	ImGui::Text("Near:(%2.2f,%2.2f)", posNear.x, posNear.y);
	//	ImGui::Text("Far:(%2.2f,%2.2f)", posFar.x, posFar.y);
	//	ImGui::Text(
	//	    "3DReticle:(%2.2f,%2.2f,%2.2f)", worldTransform3DReticle_.translation_.x,
	//	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

	//	ImGui::Text(
	//	    " player : (% 2.2f,%2.2f,%2.2f)", worldTransform_.translation_.x,
	//	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	//	ImGui::End();
	//}

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReicle_->GetPosition();

	XINPUT_STATE joyState1;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState1)) {
		spritePosition.x += (float)joyState1.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState1.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を繁栄
		sprite2DReicle_->SetPosition(spritePosition);
	}

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(static_cast<float>(spritePosition.x), (float)spritePosition.y, 0);
	Vector3 posFar = Vector3(static_cast<float>(spritePosition.x), float(spritePosition.y), 1);
	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// スティックレイの方向
	Vector3 spriteDierection;
	spriteDierection.x = posFar.x - posNear.x;
	spriteDierection.y = posFar.y - posNear.y;
	spriteDierection.z = posFar.z - posNear.z;
	spriteDierection = Normalize(spriteDierection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTextObject = 100.0f;
	worldTransform3DReticle_.translation_.x = spriteDierection.x * kDistanceTextObject;
	worldTransform3DReticle_.translation_.y = spriteDierection.y * kDistanceTextObject;
	worldTransform3DReticle_.translation_.z = spriteDierection.z * kDistanceTextObject;

	worldTransform3DReticle_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player pos");
	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", &worldTransform_.translation_.x);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", &worldTransform_.translation_.x, -20.0f, 20.0f);
	// テキスト
	ImGui::Text("PlayerBullet : Space");
	// テキスト
	ImGui::Text("DebugCamera : Enter");
	ImGui::End();
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	model_->Draw(worldTransform3DReticle_, viewProjection_);

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::DrawUI() { sprite2DReicle_->Draw(); }

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに併せて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = Get3DWorldPosition().x - GetWorldPosition().x;
		velocity.y = Get3DWorldPosition().y - GetWorldPosition().y;
		velocity.z = Get3DWorldPosition().z - GetWorldPosition().z;

		velocity.x = Normalize(velocity).x * kBulletSpeed;
		velocity.y = Normalize(velocity).y * kBulletSpeed;
		velocity.z = Normalize(velocity).z * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録
		bullets_.push_back(newBullet);
	}

	XINPUT_STATE joyState;

	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに併せて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 自機から照準オブジェクトへのベクトル
		velocity.x = Get3DWorldPosition().x - GetWorldPosition().x;
		velocity.y = Get3DWorldPosition().y - GetWorldPosition().y;
		velocity.z = Get3DWorldPosition().z - GetWorldPosition().z;

		velocity.x = Normalize(velocity).x * kBulletSpeed;
		velocity.y = Normalize(velocity).y * kBulletSpeed;
		velocity.z = Normalize(velocity).z * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録
		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::Get3DWorldPosition() { // ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

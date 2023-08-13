#include "BaseEnemyState.h"
#include "Input.h"
#include "Model.h"
#include "enemy/enemy.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ImGuiManager.h"
#include <cassert>


void EnemyStateLeave::Update(Enemy* pEnemy) {
	pEnemy->SetVelocity(0.5, 0.5, 0);

	// 移動 (ベクトルを加算)
	pEnemy->Move();
}
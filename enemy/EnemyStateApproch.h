#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "BaseEnemyState.h"

class Enemy;

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};
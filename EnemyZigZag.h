#pragma once
#include "Enemy.h"

class EnemyZigZag : public Enemy {
    float angle;

public:
    EnemyZigZag(Texture2D tex);

    void Update() override;
    void Draw() override;
    float GetRadius() const override;
    void Reset() override;

    int GetScoreValue() const override { return 100; }
};

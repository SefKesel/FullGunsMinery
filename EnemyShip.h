#pragma once
#include "Entity.h"
#include "Bala.h"
#include <vector>

class EnemyShip : public Entity {
public:
    Texture2D textura;
    int vida;

    std::vector<Bala> balas;

    float shootTimer;
    float speedX;
    float directionX;

    float baseY;
    float verticalRange;

    Vector2 playerPos;

    EnemyShip(Texture2D tex);

    void SetPlayerPosition(Vector2 pos);

    void Update() override;
    void Draw() override;

    Vector2 GetPosition() const override;
    float GetRadius() const override;
};

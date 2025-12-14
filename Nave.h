#pragma once
#include "raylib.h"
#include "Entity.h"
#include "Bala.h"
#include <vector>

class Nave : public Entity {
public:
    Texture2D textura;
    int vida;
    std::vector<Bala> balas;

    Nave(Texture2D tex);

    void Update() override;
    void Draw() override;
    float GetRadius() const override;
};

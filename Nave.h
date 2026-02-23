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

    // Fire rate power-up API
    void ApplyFireRatePowerUp(float multiplier, float duration);

private:
    float fireInterval = 0.25f; // segundos entre disparos (base)
    float fireCooldown = 0.0f;  // cuenta regresiva hasta próximo disparo
    float fireRateMultiplier = 1.0f; // multiplicador activo por power-up
    float powerupTimer = 0.0f; // tiempo restante del power-up
    float bulletSizeMultiplier = 1.0f; // multiplicador de tamaño de bala
};

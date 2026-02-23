#include "Nave.h"
#include <vector>

Nave::Nave(Texture2D tex) : textura(tex), vida(3) {
    posicion = {400, 500};
    balas.resize(10);
}

void Nave::Update() {
    if (IsKeyDown(KEY_LEFT))  posicion.x -= 6;
    if (IsKeyDown(KEY_RIGHT)) posicion.x += 6;
    if (IsKeyDown(KEY_UP))    posicion.y -= 6;
    if (IsKeyDown(KEY_DOWN))  posicion.y += 6;

    float dt = GetFrameTime();

    // Manejo de power-up (temporizador)
    if (powerupTimer > 0.0f) {
        powerupTimer -= dt;
        if (powerupTimer <= 0.0f) {
            fireRateMultiplier = 1.0f;
            bulletSizeMultiplier = 1.0f;
            powerupTimer = 0.0f;
        }
    }

    // Disparo con cadencia (mantener SPACE para disparar continuamente)
    if (IsKeyDown(KEY_SPACE)) {
        fireCooldown -= dt;
        if (fireCooldown <= 0.0f) {
            for (auto& b : balas) {
                if (!b.activa) {
                    Vector2 origen = { posicion.x, posicion.y };
                    Vector2 direccion = { 0, -1 };
                    b.Disparar(origen, direccion, YELLOW, bulletSizeMultiplier);
                    fireCooldown = fireInterval / fireRateMultiplier;
                    break;
                }
            }
        }
    } else {
        // cuando no se presiona, seguimos descontando cooldown para evitar ráfagas inmediatas
        fireCooldown -= dt;
    }

    for (auto& b : balas)
        b.Update();
}

void Nave::Draw() {
    DrawTexturePro(
        textura,
        {0, 0, (float)textura.width, (float)textura.height},
        {posicion.x - textura.width * 0.05f,
         posicion.y - textura.height * 0.05f,
         textura.width * 0.1f,
         textura.height * 0.1f},
        {0, 0},
        0,
        WHITE
    );

    for (auto& b : balas)
        b.Draw();
}

float Nave::GetRadius() const {
    return (textura.width * 0.1f) / 2.0f;
}

void Nave::ApplyFireRatePowerUp(float multiplier, float duration) {
    fireRateMultiplier = multiplier;
    powerupTimer = duration;
    // Permite disparar inmediatamente con la nueva cadencia
    fireCooldown = 0.0f;
    // Aumentar tamaño de las balas mientras dure el power-up
    bulletSizeMultiplier = 2.0f;
}

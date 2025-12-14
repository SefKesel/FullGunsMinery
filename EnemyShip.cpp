#include "EnemyShip.h"
#include <cmath>

EnemyShip::EnemyShip(Texture2D tex)
    : textura(tex), vida(3),
      shootTimer(0.0f),
      speedX(2.0f), directionX(1.0f),
      verticalRange(40.0f) {

    baseY = GetRandomValue(40, 120);
    posicion = {(float)GetScreenWidth() / 2, baseY};

    balas.resize(6);
}

void EnemyShip::SetPlayerPosition(Vector2 pos) {
    playerPos = pos;
}

void EnemyShip::Update() {
    // Movimiento lateral
    posicion.x += speedX * directionX;
    if (posicion.x < 60 || posicion.x > GetScreenWidth() - 60)
        directionX *= -1;

    // Movimiento vertical oscilante
    posicion.y = baseY + sinf(GetTime()) * verticalRange;

    // Disparo hacia el jugador
    shootTimer += GetFrameTime();
    if (shootTimer >= 1.2f) {
        for (auto& b : balas) {
            if (!b.activa) {
                Vector2 centro = { posicion.x, posicion.y + 20 };
                Vector2 dir = {
                    playerPos.x - centro.x,
                    playerPos.y - centro.y
                };

                b.Disparar(centro, dir, RED);
                shootTimer = 0.0f;
                break;
            }
        }
    }

    for (auto& b : balas)
        b.Update();
}

void EnemyShip::Draw() {
    DrawTexturePro(
        textura,
        {0, 0, (float)textura.width, (float)textura.height},
        {posicion.x,
         posicion.y,
         textura.width * 0.1f,
         textura.height * 0.1f},
        {textura.width * 0.05f,
         textura.height * 0.05f},
        180,
        WHITE
    );

    for (auto& b : balas)
        b.Draw();
}

Vector2 EnemyShip::GetPosition() const {
    return posicion;
}

float EnemyShip::GetRadius() const {
    return textura.width * 0.05f;
}

#include "EnemyZigZag.h"
#include <cmath>

EnemyZigZag::EnemyZigZag(Texture2D tex) : Enemy(tex) {
    Reset();
}

void EnemyZigZag::Reset() {
    posicion = {(float)GetRandomValue(50, 750), -50};
    velocidad = GetRandomValue(120, 180) / 100.0f; // más lento
    angle = 0.0f;
    rotacion = 0.0f;
}

void EnemyZigZag::Update() {
    angle += 0.05f;
    posicion.x += sinf(angle) * 2.0f;
    posicion.y += velocidad;

    rotacion += rotSpeed * 0.4f; // rotación más lenta

    if (posicion.y > GetScreenHeight())
        Reset();
}

void EnemyZigZag::Draw() {
    DrawTexturePro(
        textura,
        {0, 0, (float)textura.width, (float)textura.height},
        {posicion.x,
         posicion.y,
         textura.width * escala,
         textura.height * escala},
        {(textura.width * escala) / 2.0f,
         (textura.height * escala) / 2.0f},
        rotacion,
        WHITE
    );
}

float EnemyZigZag::GetRadius() const {
    return (textura.width * escala) / 2.0f;
}

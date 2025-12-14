#include "Enemy.h"

Enemy::Enemy(Texture2D tex)
    : textura(tex), rotacion(0.0f) {

    rotSpeed = GetRandomValue(10, 40) / 10.0f; // rotación más lenta
    escala = GetRandomValue(5, 15) / 100.0f; // tamaño 0.05x a 0.15x
}

#pragma once
#include "raylib.h"
#include "Collider.h"


class Entity : public Collider {
public:
Vector2 posicion;


virtual void Update() = 0;
virtual void Draw() = 0;


Vector2 GetPosition() const override {
return posicion;
}


virtual float GetRadius() const override = 0;
virtual ~Entity() = default;
};
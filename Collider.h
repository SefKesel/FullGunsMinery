#pragma once
#include "raylib.h"


class Collider {
public:
virtual Vector2 GetPosition() const = 0;
virtual float GetRadius() const = 0;


bool CheckCollision(const Collider* other) const {
return CheckCollisionCircles(
GetPosition(), GetRadius(),
other->GetPosition(), other->GetRadius()
);
}


virtual ~Collider() = default;
};
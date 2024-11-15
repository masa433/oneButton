#include "collision.h"
#include "common.h"

bool hitCheckCircle(VECTOR3 pos1, float r1, VECTOR3 pos2, float r2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float dz = pos1.z - pos2.z; // Z軸の差を計算
    float distanceSquared = dx * dx + dy * dy + dz * dz;
    float radiusSum = r1 + r2;

    return distanceSquared <= radiusSum * radiusSum;
}

bool hitCheck(PLAYER* obj1, RING* obj2) {
    VECTOR3 pos1 = { obj1->position.x + obj1->offset.x,
                    obj1->position.y + obj1->offset.y,
                    0.0f }; // プレイヤーはZ=0と仮定
    float r1 = obj1->radius;

    VECTOR3 pos2 = obj2->position + obj2->offset;
    float r2 = obj2->radius;

    return hitCheckCircle(pos1, r1, pos2, r2);
}

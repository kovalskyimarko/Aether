#include "Aether/boxCollider.hpp"
#include "Aether/gameObject.hpp"
#include <algorithm>
#include <cmath>

namespace Aether {
Vec3 BoxCollider::GetCenter() const {
  if (!gameObject)
    return {0.0f, 0.0f, 0.0f};

  const Transform &t = gameObject->GetTransform();

  // Scale the local offset by object scale
  Vec3 scaledOffset = {offset.x * t.scale.x, offset.y * t.scale.y,
                       offset.z * t.scale.z};

  // If rotation is applied, rotate the offset into world space
  if (t.rotation.x != 0.0f || t.rotation.y != 0.0f || t.rotation.z != 0.0f) {
    Mat4 rot = Mat4::RotateY(ToRadians(t.rotation.y)) *
               Mat4::RotateX(ToRadians(t.rotation.x)) *
               Mat4::RotateZ(ToRadians(t.rotation.z));

    // Transform offset by rotation
    float rx = rot.m[0][0] * scaledOffset.x + rot.m[1][0] * scaledOffset.y +
               rot.m[2][0] * scaledOffset.z;
    float ry = rot.m[0][1] * scaledOffset.x + rot.m[1][1] * scaledOffset.y +
               rot.m[2][1] * scaledOffset.z;
    float rz = rot.m[0][2] * scaledOffset.x + rot.m[1][2] * scaledOffset.y +
               rot.m[2][2] * scaledOffset.z;
    scaledOffset = {rx, ry, rz};
  }

  return t.position + scaledOffset;
}

Vec3 BoxCollider::GetMin() const {
  if (!gameObject)
    return {0.0f, 0.0f, 0.0f};

  const Transform &t = gameObject->GetTransform();
  Vec3 center = GetCenter();

  // Half extents in local space
  float hx = (std::abs(size.x * t.scale.x)) * 0.5f;
  float hy = (std::abs(size.y * t.scale.y)) * 0.5f;
  float hz = (std::abs(size.z * t.scale.z)) * 0.5f;

  // If no rotation, standard AABB
  if (t.rotation.x == 0.0f && t.rotation.y == 0.0f && t.rotation.z == 0.0f) {
    return center - Vec3{hx, hy, hz};
  }

  // For rotated entities, compute the world-aligned enclosing AABB extents
  Mat4 rot = Mat4::RotateY(ToRadians(t.rotation.y)) *
             Mat4::RotateX(ToRadians(t.rotation.x)) *
             Mat4::RotateZ(ToRadians(t.rotation.z));

  float worldHx = std::abs(rot.m[0][0]) * hx + std::abs(rot.m[1][0]) * hy +
                  std::abs(rot.m[2][0]) * hz;
  float worldHy = std::abs(rot.m[0][1]) * hx + std::abs(rot.m[1][1]) * hy +
                  std::abs(rot.m[2][1]) * hz;
  float worldHz = std::abs(rot.m[0][2]) * hx + std::abs(rot.m[1][2]) * hy +
                  std::abs(rot.m[2][2]) * hz;

  return center - Vec3{worldHx, worldHy, worldHz};
}

Vec3 BoxCollider::GetMax() const {
  if (!gameObject)
    return {0.0f, 0.0f, 0.0f};

  const Transform &t = gameObject->GetTransform();
  Vec3 center = GetCenter();

  float hx = (std::abs(size.x * t.scale.x)) * 0.5f;
  float hy = (std::abs(size.y * t.scale.y)) * 0.5f;
  float hz = (std::abs(size.z * t.scale.z)) * 0.5f;

  if (t.rotation.x == 0.0f && t.rotation.y == 0.0f && t.rotation.z == 0.0f) {
    return center + Vec3{hx, hy, hz};
  }

  Mat4 rot = Mat4::RotateY(ToRadians(t.rotation.y)) *
             Mat4::RotateX(ToRadians(t.rotation.x)) *
             Mat4::RotateZ(ToRadians(t.rotation.z));

  float worldHx = std::abs(rot.m[0][0]) * hx + std::abs(rot.m[1][0]) * hy +
                  std::abs(rot.m[2][0]) * hz;
  float worldHy = std::abs(rot.m[0][1]) * hx + std::abs(rot.m[1][1]) * hy +
                  std::abs(rot.m[2][1]) * hz;
  float worldHz = std::abs(rot.m[0][2]) * hx + std::abs(rot.m[1][2]) * hy +
                  std::abs(rot.m[2][2]) * hz;

  return center + Vec3{worldHx, worldHy, worldHz};
}

bool BoxCollider::IsColliding(const BoxCollider *other) const {
  if (!other)
    return false;
  return IsColliding(*other);
}

bool BoxCollider::IsColliding(const BoxCollider &other) const {
  if (!gameObject || !other.gameObject)
    return false;

  Vec3 aMin = GetMin();
  Vec3 aMax = GetMax();

  Vec3 bMin = other.GetMin();
  Vec3 bMax = other.GetMax();

  return (aMax.x >= bMin.x && aMin.x <= bMax.x) &&
         (aMax.y >= bMin.y && aMin.y <= bMax.y) &&
         (aMax.z >= bMin.z && aMin.z <= bMax.z);
}
} // namespace Aether
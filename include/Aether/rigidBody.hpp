#pragma once

#include "Aether/component.hpp"
#include "Aether/math.hpp"

namespace Aether
{
    class RigidBody : public Component
    {
    public:
        Vec3 velocity = {0.0f, 0.0f, 0.0f};
        Vec3 forces = {0.0f, 0.0f, 0.0f};

        float linearDamping = 0.05f; // Air resistance [0 = frictionless, 1 = heavy drag]
        bool useGravity = true;
        float gravityScale = 1.0f;

        float terminalVelocity = 50.0f; // Positive scalar (clamps downwards speed to -terminalVelocity)
        bool isKinematic = false;       // If true, forces and gravity do not affect this body

        RigidBody() = default;

        void FixedUpdate() override;
        void Update() override; // Fallback if FixedUpdate loop isn't implemented in Scene

        void AddForce(const Vec3& force);
        void AddImpulse(const Vec3& impulse); // Instant velocity change independent of dt
        void ClearForces();

        void SetMass(float newMass);
        float GetMass() const { return mass; }
        float GetInverseMass() const { return invMass; }

    private:
        float mass = 1.0f;
        float invMass = 1.0f;
        void StepSimulation(float dt);
    };
}
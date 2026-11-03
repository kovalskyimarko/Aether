#include "Aether/rigidBody.hpp"
#include "Aether/gameObject.hpp"
#include "Aether/Aether.hpp"
#include <algorithm>

namespace Aether
{
    void RigidBody::SetMass(float newMass)
    {
        mass = newMass;
        if (mass > 0.0001f)
        {
            invMass = 1.0f / mass;
        }
        else
        {
            mass = 0.0f;
            invMass = 0.0f; // Static/infinite mass body
        }
    }

    void RigidBody::AddForce(const Vec3& force)
    {
        if (isKinematic || invMass == 0.0f) return;
        forces += force;
    }

    void RigidBody::AddImpulse(const Vec3& impulse)
    {
        if (isKinematic || invMass == 0.0f) return;
        velocity += impulse * invMass;
    }

    void RigidBody::ClearForces()
    {
        forces = {0.0f, 0.0f, 0.0f};
    }

    void RigidBody::FixedUpdate()
    {
        // Preferred path: fixed 60Hz timestep from engine loop
        StepSimulation(time.GetDeltaTime());
    }

    void RigidBody::Update()
    {
        // Active if scene loop doesn't dispatch FixedUpdate separately
        StepSimulation(time.GetDeltaTime());
    }

    void RigidBody::StepSimulation(float dt)
    {
        if (!gameObject || isKinematic || invMass == 0.0f || dt <= 0.0f)
        {
            ClearForces();
            return;
        }

        if (useGravity)
        {
            forces.y -= 9.81f * gravityScale * mass;
        }

        // Symplectic Euler Velocity Update: v += (F / m) * dt
        Vec3 acceleration = forces * invMass;
        velocity += acceleration * dt;

        if (linearDamping > 0.0f)
        {
            float dragFactor = std::clamp(1.0f - linearDamping * dt, 0.0f, 1.0f);
            velocity.x *= dragFactor;
            velocity.z *= dragFactor;
        }

        if (velocity.y < -terminalVelocity)
        {
            velocity.y = -terminalVelocity;
        }

        //  Symplectic Euler Position Update: x += v_new * dt
        gameObject->GetTransform().position += velocity * dt;

        ClearForces();
    }
}

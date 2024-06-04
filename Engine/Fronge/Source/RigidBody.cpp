#include "RigidBody.h"

#include "GameObject.h"
#include "PhysicsManager.h"

#include <b2_polygon_shape.h>
#include <b2_fixture.h>

#pragma region Constructors/Destructor
fro::RigidBody::RigidBody(Reference<GameObject> const parentingGameObject)
	: Behaviour(parentingGameObject)
{
	m_Body.SetGravityScale(0.0f);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::RigidBody::fixedUpdate(float const)
{
	b2Vec2 const& translation{ m_Body.GetPosition() };
	float const rotation{ m_Body.GetAngle() };

	Reference<Transform> transform{ m_ParentingGameObject.get().getComponent<Transform>() };
	transform.get().setWorldTranslation({ translation.x, translation.y });
	transform.get().setWorldRotation(rotation);
}

void fro::RigidBody::setType(b2BodyType const type)
{
	m_Body.SetType(type);
}

void fro::RigidBody::setVelocity(glm::vec2 const& velocity)
{
	m_Body.SetLinearVelocity({ velocity.x, velocity.y });
}

void fro::RigidBody::addBoxCollider(std::string const& name, glm::vec2 const& size)
{
	b2PolygonShape boxShape{};
	boxShape.SetAsBox(size.x / 2, size.y / 2);

	m_spColliders.emplace(name, m_Body.CreateFixture(&boxShape, 1.0f));
}

void fro::RigidBody::removeBoxCollider(std::string const& name)
{
	m_spColliders.erase(name);
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
b2Body& fro::RigidBody::createBody() const
{
	b2BodyDef bodyDefinition{};

	glm::vec2 const& worldTranslation{ m_ParentingGameObject.get().getComponent<Transform>().get().getWorldTransform().getTranslation() };
	bodyDefinition.position.x = worldTranslation.x;
	bodyDefinition.position.y = worldTranslation.y;

	bodyDefinition.userData.pointer = reinterpret_cast<uintptr_t>(this);

	PhysicsManager& physicsManager{ PhysicsManager::getInstance() };
	return *physicsManager.m_World.CreateBody(&bodyDefinition);
}
#pragma endregion PrivateMethods
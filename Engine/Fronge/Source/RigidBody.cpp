#include "RigidBody.h"

#include "GameObject.h"
#include "PhysicsManager.h"

#include <b2_polygon_shape.h>
#include <b2_fixture.h>

#pragma region Constructors/Destructor
fro::RigidBody::RigidBody(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::RigidBody::setGravityScale(float const scale)
{
	m_Body.SetGravityScale(scale);
}

void fro::RigidBody::setType(Type const type)
{
	b2BodyType bodyType{};
	switch (type)
	{
	case Type::dynamicBody:
		bodyType = b2BodyType::b2_dynamicBody;
		break;

	case Type::staticBody:
		bodyType = b2BodyType::b2_staticBody;
		break;
	}

	m_Body.SetType(bodyType);
}

void fro::RigidBody::setVelocity(glm::vec2 velocity)
{
	m_Body.SetLinearVelocity({ velocity.x, velocity.y });
}

std::set<fro::Reference<fro::RigidBody>> const& fro::RigidBody::getOverlappingRigidBodies() const
{
	return m_sOverlappingRigidBodies;
}

void fro::RigidBody::addBoxShape(std::string name, glm::vec2 const& size, bool shouldCollide)
{
	b2PolygonShape boxShape{};
	boxShape.SetAsBox(size.x / 2, size.y / 2);

	b2FixtureDef fixtureDefinition{};
	fixtureDefinition.isSensor = shouldCollide;
	fixtureDefinition.shape = &boxShape;

	m_mpColliders.emplace(std::move(name), m_Body.CreateFixture(&fixtureDefinition));
}

void fro::RigidBody::removeBoxShape(std::string const& name)
{
	m_mpColliders.erase(name);
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
b2Body& fro::RigidBody::createBody() const
{
	b2BodyDef bodyDefinition{};

	glm::vec2 const& worldTranslation{ m_ParentingGameObject.get().getWorldTransform().getTranslation() };
	bodyDefinition.position.x = worldTranslation.x;
	bodyDefinition.position.y = worldTranslation.y;
	bodyDefinition.fixedRotation = true;

	bodyDefinition.userData.pointer = reinterpret_cast<uintptr_t>(this);

	return *PhysicsManager::getInstance().m_World.CreateBody(&bodyDefinition);
}
#pragma endregion PrivateMethods
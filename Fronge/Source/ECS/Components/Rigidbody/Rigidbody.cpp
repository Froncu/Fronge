#include "froch.hpp"

#include "ECS/Entity/Entity.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "Implementation/ColliderImpl.hpp"
#include "Implementation/RigidbodyImpl.hpp"
#include "Rigidbody.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
	Rigidbody::Rigidbody()
		: mOnComponentAttachEvent
		{
			[smartThis = Reference{ this }](Entity& entity, Component& component, std::type_index const& typeIndex)
			{
				auto& parentingEntity{ smartThis->mParentingEntity };

				if (&*smartThis == &component)
				{
					parentingEntity = entity;
					if (not parentingEntity->hasComponentAttached<Transform>())
						return false;

					auto const& parentingScene{ parentingEntity->getParentingScene() };
					if (not parentingScene.valid())
						return false;

					auto const& activeScene{ SceneManager::getActiveScene() };
					if (not activeScene.valid())
						return false;

					if (&*parentingScene not_eq &*SceneManager::getActiveScene())
						return false;

					smartThis->mImplementation->getb2Body().SetEnabled(true);
					return true;
				}

				if (parentingEntity.valid() and &*parentingEntity == &entity and typeIndex == typeid(Transform))
				{
					smartThis->mImplementation->getb2Body().SetEnabled(true);
					return true;
				}

				return false;
			}, EntityManager::getComponentAttachEvent()
		}
		, mOnComponentDetachEvent
		{
			[smartThis = Reference{ this }](Entity const& entity, Component& component, std::type_index const& typeIndex)
			{
				auto& parentingEntity{ smartThis->mParentingEntity };

				if (&*smartThis == &component)
				{
					parentingEntity.reset();
					smartThis->mImplementation->getb2Body().SetEnabled(false);
					return true;
				}

				if (parentingEntity.valid() and &*parentingEntity == &entity and typeIndex == typeid(Transform))
				{
					smartThis->mImplementation->getb2Body().SetEnabled(false);
					return true;
				}

				return false;
			}, EntityManager::getComponentDetachEvent()
		}
		, mOnAddedToSceneEvent
		{
			[smartThis = Reference{ this }](Entity const& entity, Scene const& scene)
			{
				auto const& parentingEntity{ smartThis->mParentingEntity };

				if (not parentingEntity.valid())
					return false;

				if (&*parentingEntity not_eq &entity)
					return false;

				auto const& activeScene{ SceneManager::getActiveScene() };
				if (not activeScene.valid())
					return false;

				if (&*activeScene not_eq &scene)
					return false;
				
				smartThis->mImplementation->getb2Body().SetEnabled(true);
				return true;
			}, EntityManager::getAddedToSceneEvent()
		}
		, mOnRemovedFromSceneEvent
		{
			[smartThis = Reference{ this }](Entity const& entity, Scene const&)
			{
				auto const& parentingEntity{ smartThis->mParentingEntity };

				if (not parentingEntity.valid())
					return false;

				if (&*parentingEntity not_eq &entity)
					return false;

				smartThis->mImplementation->getb2Body().SetEnabled(false);
				return true;
			}, EntityManager::getRemovedFromSceneEvent()
		}
		, mOnActiveSceneChangedEvent
		{
			[smartThis = Reference{ this }](Reference<Scene> const&, Reference<Scene> const& activeScene)
			{
				auto const& parentingEntity{ smartThis->mParentingEntity };

				if (not parentingEntity.valid())
					return false;

				auto const& parentingScene{ parentingEntity->getParentingScene() };
				if (not parentingScene.valid())
					return false;

				if (not activeScene.valid())
					return false;

				smartThis->mImplementation->getb2Body().SetEnabled(&*parentingScene == &*activeScene);
				return true;
			}, SceneManager::getActiveSceneChangedEvent()
		}
		, mImplementation{ std::make_unique<Implementation>() }
	{
		auto& body{ mImplementation->getb2Body() };
		body.SetEnabled(false);
		body.GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);
	}

	Rigidbody::Rigidbody(Rigidbody const& other)
		: Component(other)

		, mBeginContactEvent{ other.mBeginContactEvent }
		, mEndContactEvent{ other.mEndContactEvent }
		, mOnComponentAttachEvent{ other.mOnComponentAttachEvent }
		, mOnComponentDetachEvent{ other.mOnComponentDetachEvent }
		, mOnAddedToSceneEvent{ other.mOnAddedToSceneEvent }
		, mOnRemovedFromSceneEvent{ other.mOnRemovedFromSceneEvent }
		, mOnActiveSceneChangedEvent{ other.mOnActiveSceneChangedEvent }
		, mParentingEntity{ other.mParentingEntity }
		, mImplementation{ std::make_unique<Implementation>(other.mImplementation->getb2BodyDef()) }
	{
		mColliders.clear();
		for (auto const& otherCollider : other.mColliders)
			mColliders.push_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(),
				otherCollider.getImplementation().getb2FixtureDef()));

		mImplementation->getb2Body().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);
	}

	Rigidbody::Rigidbody(Rigidbody&& other) noexcept
		: Component(std::move(other))

		, mBeginContactEvent{ std::move(other.mBeginContactEvent) }
		, mEndContactEvent{ std::move(other.mEndContactEvent) }
		, mOnComponentAttachEvent{ std::move(other.mOnComponentAttachEvent) }
		, mOnComponentDetachEvent{ std::move(other.mOnComponentDetachEvent) }
		, mOnAddedToSceneEvent{ std::move(other.mOnAddedToSceneEvent) }
		, mOnRemovedFromSceneEvent{ std::move(other.mOnRemovedFromSceneEvent) }
		, mOnActiveSceneChangedEvent{ std::move(other.mOnActiveSceneChangedEvent) }
		, mParentingEntity{ std::move(other.mParentingEntity) }
		, mImplementation{ std::move(other.mImplementation) }
		, mColliders{ std::move(other.mColliders) }
	{
		mImplementation->getb2Body().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);
	}

	Rigidbody::~Rigidbody()
	{
	}

	Rigidbody& Rigidbody::operator=(Rigidbody const& other)
	{
		if (this == &other)
			return *this;

		Component::operator=(other);

		mBeginContactEvent = other.mBeginContactEvent;
		mEndContactEvent = other.mEndContactEvent;
		mOnComponentAttachEvent = other.mOnComponentAttachEvent;
		mOnComponentDetachEvent = other.mOnComponentDetachEvent;
		mOnAddedToSceneEvent = other.mOnAddedToSceneEvent;
		mOnRemovedFromSceneEvent = other.mOnRemovedFromSceneEvent;
		mOnActiveSceneChangedEvent = other.mOnActiveSceneChangedEvent;
		mParentingEntity = other.mParentingEntity;
		mImplementation = std::make_unique<Implementation>(other.mImplementation->getb2BodyDef());

		mColliders.clear();
		for (auto const& otherCollider : other.mColliders)
			mColliders.push_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(),
				otherCollider.getImplementation().getb2FixtureDef()));

		mImplementation->getb2Body().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);

		return *this;
	}

	Rigidbody& Rigidbody::operator=(Rigidbody&& other) noexcept
	{
		if (this == &other)
			return *this;

		Component::operator=(std::move(other));

		mBeginContactEvent = std::move(other.mBeginContactEvent);
		mEndContactEvent = std::move(other.mEndContactEvent);
		mOnComponentAttachEvent = std::move(other.mOnComponentAttachEvent);
		mOnComponentDetachEvent = std::move(other.mOnComponentDetachEvent);
		mOnAddedToSceneEvent = std::move(other.mOnAddedToSceneEvent);
		mOnRemovedFromSceneEvent = std::move(other.mOnRemovedFromSceneEvent);
		mOnActiveSceneChangedEvent = std::move(other.mOnActiveSceneChangedEvent);
		mParentingEntity = std::move(other.mParentingEntity);
		mImplementation = std::move(other.mImplementation);
		mColliders = std::move(other.mColliders);

		mImplementation->getb2Body().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);

		return *this;
	}

	Rigidbody::Implementation& Rigidbody::getImplementation() const
	{
		return *mImplementation;
	}

	void Rigidbody::setType(Type const type)
	{
		switch (type)
		{
		case Type::STATIC:
			mImplementation->getb2Body().SetType(b2BodyType::b2_staticBody);
			break;

		case Type::KINEMATIC:
			mImplementation->getb2Body().SetType(b2BodyType::b2_kinematicBody);
			break;

		case Type::DYNAMIC:
			mImplementation->getb2Body().SetType(b2BodyType::b2_dynamicBody);
			break;
		}
	}

	void Rigidbody::setFixedRotation(bool const lockRotation)
	{
		mImplementation->getb2Body().SetFixedRotation(lockRotation);
	}

	void Rigidbody::setLinearVelocity(Vector2<double> const velocity)
	{
		mImplementation->getb2Body().SetLinearVelocity({ static_cast<float>(velocity.x), static_cast<float>(velocity.y) });
	}

	void Rigidbody::setAngularVelocity(double const velocity)
	{
		mImplementation->getb2Body().SetAngularVelocity(static_cast<float>(velocity));
	}

	void Rigidbody::applyForce(Vector2<double> const force)
	{
		mImplementation->getb2Body().ApplyForceToCenter({ static_cast<float>(force.x), static_cast<float>(force.y) }, true);
	}

	void Rigidbody::applyTorque(double const torque)
	{
		mImplementation->getb2Body().ApplyTorque(static_cast<float>(torque), true);
	}

	void Rigidbody::applyLinearImpulse(Vector2<double> const impulse)
	{
		mImplementation->getb2Body().ApplyLinearImpulseToCenter({ static_cast<float>(impulse.x), static_cast<float>(impulse.y) }, true);
	}

	void Rigidbody::applyAngularImpulse(double const impulse)
	{
		mImplementation->getb2Body().ApplyAngularImpulse(static_cast<float>(impulse), true);
	}

	Rigidbody::Type Rigidbody::getType() const
	{
		b2BodyType type{ mImplementation->getb2Body().GetType() };
		switch (type)
		{
		case b2_staticBody:
			return Type::STATIC;

		case b2_kinematicBody:
			return Type::KINEMATIC;

		case b2_dynamicBody:
			return Type::DYNAMIC;

		default:
			exception("b2BodyType with value {} not supported by Fronge!",
				static_cast<int>(type));
		}
	}

	bool Rigidbody::isFixedRotation() const
	{
		return mImplementation->getb2Body().IsFixedRotation();
	}

	Vector2<double> Rigidbody::getLinearVelocity() const
	{
		auto const linearVelocity{ mImplementation->getb2Body().GetLinearVelocity() };
		return { static_cast<double>(linearVelocity.x), static_cast<double>(linearVelocity.y) };
	}

	double Rigidbody::getAngularVelocity() const
	{
		return static_cast<double>(mImplementation->getb2Body().GetAngularVelocity());
	}

	Collider& Rigidbody::addCollider()
	{
		auto const b2Shape{ Collider::Implementation::createb2Shape(Rectangle<double>{ 0.0, 0.0, 32.0, 32.0 }) };
		b2FixtureDef fixtureDefinition{};
		fixtureDefinition.shape = b2Shape.get();

		return mColliders.emplace_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(), fixtureDefinition));
	}
}
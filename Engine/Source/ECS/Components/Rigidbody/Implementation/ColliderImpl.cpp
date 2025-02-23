#include "froch.hpp"

#include "ColliderImpl.hpp"
#include "RigidbodyImpl.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
	std::unique_ptr<b2Shape> Collider::Implementation::createb2Shape(
		std::variant<Rectangle<double>, Polygon<double>, Chain<double>, Circle<double>> const& shape)
	{
		std::unique_ptr<b2Shape> convertedShape{};

		VariantVisitor
		{
			[&convertedShape](Rectangle<double> const& rectangle)
			{
				auto const halfWidth{ rectangle.width / 2 };
				auto const halfHeight{ rectangle.height / 2 };

				std::array<b2Vec2, 4> vertices
				{
					b2Vec2{ static_cast<float>(rectangle.x - halfWidth), static_cast<float>(rectangle.y - halfHeight) },
					b2Vec2{ static_cast<float>(rectangle.x + halfWidth), static_cast<float>(rectangle.y - halfHeight) },
					b2Vec2{ static_cast<float>(rectangle.x + halfWidth), static_cast<float>(rectangle.y + halfHeight) },
					b2Vec2{ static_cast<float>(rectangle.x - halfWidth), static_cast<float>(rectangle.y + halfHeight) },
				};

				auto polygonShape{ std::make_unique<b2PolygonShape>() };
				polygonShape->Set(&vertices.front(), static_cast<int32>(vertices.size()));

				convertedShape = std::move(polygonShape);
			},

			[&convertedShape](Polygon<double> const& convexPolygon)
			{
				std::vector<b2Vec2> convertedPolygon(convexPolygon.vertices.size());
				for (std::size_t index{}; index < convertedPolygon.size(); ++index)
				{
					convertedPolygon[index].x = static_cast<float>(convexPolygon.vertices[index].x);
					convertedPolygon[index].y = static_cast<float>(convexPolygon.vertices[index].y);
				}

				auto polygonShape{ std::make_unique<b2PolygonShape>() };
				polygonShape->Set(&convertedPolygon.front(), static_cast<int32>(convertedPolygon.size()));

				convertedShape = std::move(polygonShape);
			},

			[&convertedShape](Chain<double> const& chain)
			{
				std::vector<b2Vec2> convertedChain(chain.vertices.size());
				for (std::size_t index{}; index < convertedChain.size(); ++index)
				{
					convertedChain[index].x = static_cast<float>(chain.vertices[index].x);
					convertedChain[index].y = static_cast<float>(chain.vertices[index].y);
				}

				auto chainShape{ std::make_unique<b2ChainShape>() };
				chainShape->CreateChain(&convertedChain.front(), static_cast<int32>(convertedChain.size()),
					convertedChain.front(),
					convertedChain.back());

				convertedShape = std::move(chainShape);
			},

			[&convertedShape](Circle<double> const& circle)
			{
				auto circleShape{ std::make_unique<b2CircleShape>() };
				circleShape->m_radius = static_cast<float>(circle.radius);
				circleShape->m_p.x = static_cast<float>(circle.center.x);
				circleShape->m_p.y = static_cast<float>(circle.center.y);

				convertedShape = std::move(circleShape);
			}

		}(shape);

		return convertedShape;
	}

	Collider::Implementation::Implementation(b2Body& body, b2FixtureDef const& fixtureDefinition)
		: mFixture
		{
			body.CreateFixture(&fixtureDefinition),
			std::bind(&b2Body::DestroyFixture, &body, std::placeholders::_1)
		}
	{
	}

	b2Fixture& Collider::Implementation::getb2Fixture() const
	{
		return *mFixture;
	}

	b2FixtureDef Collider::Implementation::getb2FixtureDef() const
	{
		b2FixtureDef fixtureDefinition{};
		fixtureDefinition.shape = mFixture->GetShape();
		fixtureDefinition.density = mFixture->GetDensity();
		fixtureDefinition.friction = mFixture->GetFriction();
		fixtureDefinition.restitution = mFixture->GetRestitution();
		fixtureDefinition.restitutionThreshold = mFixture->GetRestitutionThreshold();
		fixtureDefinition.isSensor = mFixture->IsSensor();
		fixtureDefinition.filter = mFixture->GetFilterData();

		return fixtureDefinition;
	}
}
#if not defined fro_GAME_OBJECT_H
#define fro_GAME_OBJECT_H

#include "BaseReferencable.h"
#include "Behaviour.h"
#include "Defines.hpp"
#include "Event.hpp"
#include "GUI.h"
#include "Renderable.h"
#include "TransformationMatrix2D.h"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace fro
{
	template<typename ComponentType>
	concept ComponentDerived = std::derived_from<ComponentType, Component>;

	class GameObject final : public BaseReferencable
	{
	public:
		GameObject(std::string name, std::string tag);
		GameObject(GameObject&& other) noexcept;

		~GameObject() = default;

		GameObject& operator=(GameObject&& other) noexcept;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void lateUpdate(float const deltaSeconds) const;

		void render() const;
		void display() const;

		void setActive(bool const isActive);
		void setParent(Reference<GameObject> const parent, bool const keepWorldTransform = true);

		void localTransform(TransformationMatrix2D const& transformation);
		void localTranslate(glm::vec2 const& translation);
		void localRotate(float const rotation);
		void localScale(glm::vec2 const& scale);
		void worldTransform(TransformationMatrix2D const& transformation);
		void worldTranslate(glm::vec2 const& translation);
		void worldRotate(float const rotation);
		void worldScale(glm::vec2 const& scale);

		void setLocalTransformation(TransformationMatrix2D const& transformation);
		void setLocalTranslation(glm::vec2 const& translation);
		void setLocalRotation(float const rotation);
		void setLocalScale(glm::vec2 const& scale);
		void setWorldTransformation(TransformationMatrix2D const& transformation);
		void setWorldTranslation(glm::vec2 const& translation);
		void setWorldRotation(float const rotation);
		void setWorldScale(glm::vec2 const& scale);

		fro_NODISCARD bool isActive() const;
		fro_NODISCARD std::string_view getName() const;
		fro_NODISCARD std::string_view getTag() const;
		fro_NODISCARD bool owns(Reference<GameObject> const gameObject) const;
		fro_NODISCARD Reference<GameObject> getParent() const;
		fro_NODISCARD std::vector<Reference<GameObject>> const& getChildren() const;
		fro_NODISCARD Reference<GameObject> getChild(std::string_view const name) const;
		fro_NODISCARD TransformationMatrix2D const& getLocalTransform() const;
		fro_NODISCARD TransformationMatrix2D const& getWorldTransform() const;

		template<ComponentDerived ComponentType>
		Reference<ComponentType> addComponent() noexcept
		{
			if (m_mpComponents.contains(typeid(ComponentType)))
				return {};

			auto const resultPair{ m_mpComponents.emplace(typeid(ComponentType),
				std::make_unique<ComponentType>(*this)) };

			Reference<ComponentType> const addedComponent
			{
				static_cast<ComponentType* const>(resultPair.first->second.get())
			};

			if constexpr (std::derived_from<ComponentType, Behaviour>)
				m_vBehaviours.push_back(addedComponent);
			if constexpr (std::derived_from<ComponentType, Renderable>)
				m_vRenderables.push_back(addedComponent);
			if constexpr (std::derived_from<ComponentType, GUI>)
				m_vGUIs.push_back(addedComponent);

			return addedComponent;
		}

		template<ComponentDerived ComponentType>
		bool removeComponent() noexcept
		{
			Reference<ComponentType> const foundComponent{ getComponent<ComponentType>() };
			if (not foundComponent.valid())
				return false;

			if constexpr (std::derived_from<ComponentType, Behaviour>)
				m_vBehaviours.erase(
					std::remove(m_vBehaviours.begin(), m_vBehaviours.end(), foundComponent),
					m_vBehaviours.end());
			if constexpr (std::derived_from<ComponentType, Renderable>)
				m_vRenderables.erase(
					std::remove(m_vRenderables.begin(), m_vRenderables.end(), foundComponent),
					m_vRenderables.end());
			if constexpr (std::derived_from<ComponentType, GUI>)
				m_vGUIs.erase(
					std::remove(m_vGUIs.begin(), m_vGUIs.end(), foundComponent),
					m_vGUIs.end());

			return m_mpComponents.erase(typeid(ComponentType));
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD Reference<ComponentType> getComponent() const noexcept
		{
			auto const iterator{ m_mpComponents.find(typeid(ComponentType)) };
			if (iterator == m_mpComponents.end())
				return {};

			return static_cast<ComponentType* const>(iterator->second.get());
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD Reference<ComponentType> forceGetComponent() noexcept
		{
			Reference<ComponentType> foundComponent{ getComponent<ComponentType>() };
			if (not foundComponent.valid())
				foundComponent = addComponent<ComponentType>();

			return foundComponent;
		}

	private:
		GameObject(GameObject const&) = delete;

		GameObject& operator=(GameObject const&) = delete;

		void setWorldTransformDirty();
		void calculateLocalTransform();
		void calculateWorldTransform() const;

		std::string m_Name;
		std::string m_Tag;

		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_mpComponents{};

		TransformationMatrix2D m_LocalTransform{};
		mutable TransformationMatrix2D m_WorldTransform{};

		std::vector<Reference<Behaviour>> m_vBehaviours{};
		std::vector<Reference<Renderable>> m_vRenderables{};
		std::vector<Reference<GUI>> m_vGUIs{};

		std::vector<Reference<GameObject>> m_vChildren{};
		Reference<GameObject> m_Parent{};

		mutable bool m_IsWorldTransformDirty{};

		bool m_IsActive{ true };
	};
}

#endif
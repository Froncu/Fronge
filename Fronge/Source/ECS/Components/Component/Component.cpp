#include "froch.hpp"

#include "Component.hpp"
#include "ECS/Entity/Entity.hpp"

namespace fro
{
    Component::Component()
        : mOnComponentAttachEvent
        {
            [this](Entity& entity, Component& component, std::type_index const&)
            {
                if (this not_eq &component)
                    return false;
            
                mEntity = entity;
                return true;
            }, Entity::sComponentAttachEvent
        }
        , mOnComponentDetachEvent
        {
            [this](Entity const&, Component& component, std::type_index const&)
            {
                if (this not_eq &component)
                    return false;

                mEntity.reset();
                return true;
            }, Entity::sComponentDetachEvent
        }
    {
    }

    Reference<Entity> fro::Component::getEntity() const
    {
        return mEntity;
    }
}
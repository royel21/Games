#ifndef __ECS_H__
#define __ECS_H__

#include <array>
#include <bitset>
#include <iostream>

namespace Plutus
{
    class Component;
    class Entity;

    using ComponentTypeId = std::size_t;

    inline ComponentTypeId getUniqueComponentId()
    {
        static ComponentTypeId lastId = 0u;
        return lastId++;
    }
    template <typename T>
    inline ComponentTypeId getComponentTypeId() noexcept
    {
        static_assert(std::is_base_of<Component, T>::value, "Type not base on component");
        static const ComponentTypeId TypeId = getUniqueComponentId();
        return TypeId;
    }

    constexpr std::size_t MAX_ENTIRIES = 5000;
    constexpr std::size_t MAX_COMPONENT = 32;

    using ComponentBitset = std::bitset<MAX_COMPONENT>;
    using ComponentList = std::array<Component *, MAX_COMPONENT>;
} // namespace Plutus
#endif // __ECS_H__
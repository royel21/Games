#ifndef __ENTITY2_H__
#define __ENTITY2_H__

#include "ECS.h"
#include "vector"
#include <memory>

namespace Plutus
{
    class Component;
    class Entity2
    {
    public:
        Entity2() {}
        ~Entity2() {}

        template <typename T, typename... TArgs>
        inline T &addComponent(TArgs &&... args)
        {
            T *comp(new T(std::forward<TArgs>(args...)));

            comp->entity = this;

            std::unique_ptr<Component> ptr{comp};

            components.emplace_back(std::move(ptr));

            if (com->init())
            {
                compList[getComponentTypeId()] = comp;
                compBitset[getComponentTypeId()] = true;
            }
            return ptr;
        }

    private:
        ComponentList compList;
        ComponentBitset compBitset;
        std::vector<std::unique_ptr<Component>> components;
    };
} // namespace Plutus
#endif // __ENTITY2_H__
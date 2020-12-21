#ifndef __LAYER_H__
#define __LAYER_H__

#include "entt.hpp"

namespace Plutus
{
    class Entity;

    class Layer
    {
    public:
        Layer(const std::string &_name);
        void setName(const std::string &_name) { name = _name; }
        Entity createEntity(const std::string &name);

    private:
        std::string name;
        entt::registry mRegistry;
        friend class Entity;
    };
} // namespace Plutus

#endif // __LAYER_H__
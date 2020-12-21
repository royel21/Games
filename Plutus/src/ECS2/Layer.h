#ifndef __LAYER_H__
#define __LAYER_H__

#include "entt.hpp"

namespace Plutus
{
    class Layer
    {
    public:
        Layer(const std::string &_name);
        void setName(const std::string &_name) { name = _name; }
        void addEntity(entt::entity entId) { entities.push_back(entId); }

    private:
        std::string name;
        std::vector<entt::entity> entities;
    };
} // namespace Plutus

#endif // __LAYER_H__
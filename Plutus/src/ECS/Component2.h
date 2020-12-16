#ifndef __COMPONENT2_H__
#define __COMPONENT2_H__
namespace Plutus
{
    class Entity2;

    class Component2
    {
    public:
        Component2() = default;
        ~Component2() = default;

        Entity2 *entity;
        virtual bool init() { return true; }
        virtual bool draw(){};
        virtual bool update();
    };

} // namespace Plutus
#endif // __COMPONENT2_H__
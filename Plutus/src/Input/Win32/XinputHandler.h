#ifndef __XINPUTHANDLER_H__
#define __XINPUTHANDLER_H__

#include <unordered_map>

struct ButtonMap;

namespace Plutus
{
    void updateGamePadState(std::unordered_map<unsigned int, ButtonMap> &gamepadList);

} // namespace Plutus

#endif // __XINPUTHANDLER_H__
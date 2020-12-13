#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <unordered_map>
#include "glm/glm.hpp"

namespace Plutus
{

	class InputManager
	{
	public:
		static InputManager *getInstance();

		void update();

		void pressKey(unsigned int keyId);

		void releaseKey(unsigned int keyId);

		void setMouseCoords(float x, float y);

		bool onKeyDown(unsigned int keyId);

		bool onKeyPressed(unsigned int keyId);

		glm::vec2 getMouseCoords() const { return m_mouseCoords; }

	private:
		InputManager();
		static InputManager *manager;

		std::unordered_map<unsigned int, bool> m_keyMap;

		std::unordered_map<unsigned int, bool> m_prevKeyMap;

		glm::vec2 m_mouseCoords;

		bool wasKeyDown(unsigned int keyId);
	};
} // namespace Plutus

#endif

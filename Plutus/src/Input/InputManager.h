#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <unordered_map>
#include "glm/glm.hpp"

namespace Plutus
{

	class InputManager
	{
	private:
		glm::vec2 m_mouseCoords;
		int m_mouseWheel = false;
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_prevKeyMap;
		bool mouseMove = false;

	public:
		static InputManager *getInstance();

		void update();

		void pressKey(unsigned int keyId);

		void releaseKey(unsigned int keyId);

		void setMouseCoords(float x, float y);

		bool onKeyDown(unsigned int keyId);

		bool onKeyPressed(unsigned int keyId);

		void setMouseWheel(int dir) { m_mouseWheel = dir; }
		void setMouseMove(bool move) { mouseMove = move; }
		bool getMouseIsMoving() { return mouseMove; }

		int getMouseWheel() { return m_mouseWheel; }

		glm::vec2 getMouseCoords() const { return m_mouseCoords; }

	private:
		InputManager();
		bool wasKeyDown(unsigned int keyId);
	};
} // namespace Plutus

#endif

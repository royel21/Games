#include "InputManager.h"
#include "Log/Logger.h"

namespace Plutus
{
	InputManager *InputManager::manager;

	InputManager::InputManager() : m_mouseCoords(0, 0), m_keyMap(0)
	{
	}

	InputManager *InputManager::getInstance()
	{
		if (!manager)
		{
			manager = new InputManager();
		}
		return manager;
	}

	// save the key state before register new key event
	void InputManager::update()
	{
		//Loop through keymap and copy it to prevKeyMap.
		for (auto &key : m_keyMap)
		{
			m_prevKeyMap[key.first] = key.second;
		}
	}
	//Register a key is pressed
	void InputManager::pressKey(unsigned int keyId)
	{
		m_keyMap[keyId] = true;
	}
	//Register a key was released
	void InputManager::releaseKey(unsigned int keyId)
	{
		m_keyMap[keyId] = false;
	}
	//Set the mouse X,Y position on Screen
	void InputManager::setMouseCoords(float x, float y)
	{
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	//return the state of the key on last frame
	bool InputManager::wasKeyDown(unsigned int keyId)
	{
		auto it = m_prevKeyMap.find(keyId);
		//Check if key was found and return it state else return false
		return it != m_prevKeyMap.end() ? it->second : false;
	}

	// return true if key is being hold down
	bool InputManager::onKeyDown(unsigned int keyId)
	{
		return m_keyMap[keyId];
	}

	//return single key press
	bool InputManager::onKeyPressed(unsigned int keyId)
	{
		//return true if key was not down last frame
		return onKeyDown(keyId) && !wasKeyDown(keyId);
	}
} // namespace Plutus
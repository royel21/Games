#ifndef _CAMERA2D_H
#define _CAMERA2D_H

#include "glm/glm.hpp"

namespace Plutus
{

	class Camera2D
	{
	private:
		int m_screenWidth;
		int m_screenHeight;
		bool m_needsMatrixUpdate;
		float m_scale;
		glm::vec2 m_position;
		glm::mat4 m_cameraMatrix;
		glm::mat4 m_orthoMatrix;
		glm::vec2 screenOrigin;
		glm::vec2 screenEnd;

	public:
		Camera2D();
		~Camera2D();
		void init(int screenWidth, int screenHeight);

		void update();
		glm::vec2 convertScreenToWold(glm::vec2 screenCoords);
		//Setters
		void setPosition(const glm::vec2 &newPosition)
		{
			m_position = newPosition;
			m_needsMatrixUpdate = true;
		}
		void setScale(float newScale)
		{
			m_scale = newScale;
			m_needsMatrixUpdate = true;
		}

		//Getters
		glm::vec2 getPosition() { return m_position; }
		glm::mat4 getCameraMatrix() { return m_cameraMatrix; }
		float getScale() { return m_scale; }

		bool isBoxInView(const glm::vec2 position, const glm::vec2 dim);

		const glm::vec2 &getScreenOrigin() { return screenOrigin; }
		const glm::vec2 &getScreenEnd() { return screenEnd; }
		const glm::vec2 getScaleScreen() { return glm::vec2(m_screenWidth, m_screenHeight) / m_scale; }
	};
} // namespace Plutus

#endif

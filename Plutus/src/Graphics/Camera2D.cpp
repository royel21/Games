#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Plutus
{
	Camera2D::Camera2D() : m_screenWidth(500),
						   m_screenHeight(500),
						   m_needsMatrixUpdate(true),
						   m_scale(1.0f),
						   m_position(0.0f, 0.0f),
						   m_cameraMatrix(1.0f),
						   m_orthoMatrix(1.0f)
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		//this is for convert the opengl espace -1.0 - 1.0 to 0 - screenW and height
		m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight, 1.0f, -1.0f);
		m_needsMatrixUpdate = true;
	}

	void Camera2D::update()
	{
		if (m_needsMatrixUpdate == true)
		{

			//camera translation
			glm::vec3 translate(-m_position.x + (m_screenWidth >> 1), -m_position.y + (m_screenHeight >> 1), 0.0f);
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

			//Camera Scale
			glm::vec3 scale(m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

			m_needsMatrixUpdate = false;

			screenOrigin = (translate / scale) * -1.0f;
			screenEnd = translate / scale;
		}
	}
	//Convert screen coordination to opengl coordination and return it
	glm::vec2 Camera2D::convertScreenToWold(glm::vec2 screenCoords)
	{
		//inver Y direction;
		// screenCoords.y = m_screenHeight - screenCoords.y;
		//Make that 0 is the center
		screenCoords -= glm::vec2(m_screenWidth >> 1, m_screenHeight >> 1);

		//Scale the coordinates
		screenCoords /= m_scale;

		//Translate with the camera position
		screenCoords += m_position;

		return screenCoords;
	}
	glm::vec2 Camera2D::convertScreenToWoldInv(glm::vec2 screenCoords)
	{
		//inver Y direction;
		screenCoords.y = m_screenHeight - screenCoords.y;
		//Make that 0 is the center
		screenCoords -= glm::vec2(m_screenWidth >> 1, m_screenHeight >> 1);

		//Scale the coordinates
		screenCoords /= m_scale;

		//Translate with the camera position
		screenCoords += m_position;

		return screenCoords;
	}

	bool Camera2D::isBoxInView(const glm::vec2 position, const glm::vec2 dim)
	{
		glm::vec2 scaleDim = getScaleScreen();

		const float MIN_DISTANCE_X = dim.x / 2.0f + scaleDim.x / 2.0f;
		const float MIN_DISTANCE_Y = dim.y / 2.0f + scaleDim.y / 2.0f;

		glm::vec2 centerPos = position + dim / 2.0f;

		glm::vec2 distVec = centerPos - m_position;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		return false;
	}
} // namespace Plutus
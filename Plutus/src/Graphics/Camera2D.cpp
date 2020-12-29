#include "Camera2D.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Log/Logger.h"

namespace Plutus
{
	Camera2D::Camera2D() : m_screenWidth(500),
						   m_screenHeight(500),
						   m_needsMatrixUpdate(true),
						   m_scale(1.0f),
						   mCamPos(0.0f, 0.0f),
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
			m_cameraMatrix = glm::translate(m_orthoMatrix, glm::vec3(-mCamPos.x, -mCamPos.y, 0.0f));

			//Camera Scale
			glm::vec3 scale(m_scale, m_scale, 0.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;
			m_needsMatrixUpdate = false;
		}
	}
	//Convert screen coordination to opengl coordination and return it
	glm::vec2 Camera2D::convertScreenToWold(glm::vec2 coords)
	{
		//inver Y axis of the coords;
		coords.y = m_screenHeight - coords.y;

		coords -= glm::vec2(m_screenWidth >> 1, m_screenHeight >> 1);
		//have to scale the coordinate and the camera current pos
		coords /= m_scale;
		auto camScale = mCamPos / m_scale;

		//Translate with the camera position
		coords += mCamPos;

		coords += glm::vec2(m_screenWidth >> 1, m_screenHeight >> 1);
		// LOG_I("{0:.0f} {1:.0f}", coords.x, coords.y);
		return coords;
	}
	glm::vec2 Camera2D::convertScreenToWoldInv(glm::vec2 coords)
	{
		//inver Y direction;
		coords.y = m_screenHeight - coords.y;
		//Make that 0 is the center
		coords -= glm::vec2(m_screenWidth >> 1, m_screenHeight >> 1);

		//Scale the coordinates
		coords /= m_scale;

		//Translate with the camera position
		coords += mCamPos;

		return coords;
	}

	bool Camera2D::isBoxInView(const glm::vec2 position, const glm::vec2 dim)
	{
		glm::vec2 scaleDim = getScaleScreen();

		const float MIN_DISTANCE_X = dim.x / 2.0f + scaleDim.x / 2.0f;
		const float MIN_DISTANCE_Y = dim.y / 2.0f + scaleDim.y / 2.0f;

		glm::vec2 centerPos = position + dim / 2.0f;

		glm::vec2 distVec = centerPos - mCamPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		return false;
	}
} // namespace Plutus
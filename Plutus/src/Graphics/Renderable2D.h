#ifndef _RENDERERABLE2D_H
#define _RENDERERABLE2D_H

#include <glm/glm.hpp>
#include <vector>
#include "vertex.h"

namespace Plutus
{
	class Renderable2D
	{
	protected:
		glm::vec2 mPosition;
		glm::vec2 mSize;
		glm::vec4 mUVCoord;
		ColorRGBA8 mColor;
		GLuint mTid;
		float m_angle;

	public:
		Renderable2D() : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTid(0), m_angle(0)
		{
		}

		Renderable2D(
			const glm::vec2 &position,
			const glm::vec2 &size,
			const glm::vec4 &uvCoord,
			const ColorRGBA8 &color,
			uint32_t tid,
			float angle = 0) : mPosition(position), mSize(size), mUVCoord(uvCoord), mColor(color), mTid(tid), m_angle(angle)
		{
		}

		virtual ~Renderable2D() {}

		inline const glm::vec2 &getPosition() { return mPosition; }
		inline const glm::vec2 &getSize() { return mSize; }
		inline const ColorRGBA8 &getColor() { return mColor; }
		inline const glm::vec4 &getUV() { return mUVCoord; }
		inline GLuint getTexturedId() { return mTid; }
		inline float getAngle() { return m_angle; }

		void setPosition(const glm::vec2 &position) { mPosition = position; }
		void setSize(const glm::vec2 &size) { mSize = size; }
		void setColor(const ColorRGBA8 &color) { mColor = color; }
		void setUV(const glm::vec4 &uvCoord) { mUVCoord = uvCoord; }
		void setTexturedId(uint32_t tid) { mTid = tid; }
		void setAngle(float angle) { m_angle = angle; }
	};
} // namespace Plutus
#endif

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
		bool mFlipX = false;
		bool mFlipY = false;
		float mRotation = 0.0f;

	public:
		Renderable2D() : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTid(0), mRotation(0.0f)
		{
		}

		Renderable2D(
			const glm::vec2 &position,
			const glm::vec2 &size,
			const glm::vec4 &uvCoord,
			const ColorRGBA8 &color,
			uint32_t tid,
			float angle = 0.0f)
			: mPosition(position), mSize(size), mUVCoord(uvCoord), mColor(color), mTid(tid), mRotation(angle)
		{
		}

		virtual ~Renderable2D() {}

		const glm::vec2 &getPosition() { return mPosition; }
		const glm::vec2 &getSize() { return mSize; }
		const ColorRGBA8 &getColor() { return mColor; }
		const glm::vec4 &getUV() { return mUVCoord; }

		GLuint getTexturedId() { return mTid; }

		bool getFlipX() { return mFlipX; }
		bool getFlipY() { return mFlipY; }
		bool getRotation() { return mRotation; }

		void setFlipX(bool f) { mFlipX = f; };
		void setFlipY(bool f) { mFlipY = f; };
		void setRotation(float angle) { mRotation = angle; }
		void setPosition(const glm::vec2 &position) { mPosition = position; }
		void setSize(const glm::vec2 &size) { mSize = size; }
		void setColor(const ColorRGBA8 &color) { mColor = color; }
		void setUV(const glm::vec4 &uvCoord) { mUVCoord = uvCoord; }
		void setTexturedId(uint32_t tid) { mTid = tid; }
	};
} // namespace Plutus
#endif

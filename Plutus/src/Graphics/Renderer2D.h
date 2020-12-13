#ifndef _RENDERER2D_H
#define _RENDERER2D_H

#include <glm.hpp>
#include <vector>

#include "Renderable2d.h"

namespace Plutus
{
	class RenderBatch2D
	{
	public:
		RenderBatch2D(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset), numVertices(NumVertices), texture(Texture)
		{
		}
		GLuint offset = 0;
		GLuint numVertices = 0;
		GLuint texture = 0;
	};

	class Renderer2D
	{
	protected:
		std::vector<glm::mat4> mTransformationStack;
		glm::mat4* mTransformationback;
	public:
		Renderer2D()
		{
			mTransformationStack.push_back(glm::mat4(1));
			mTransformationback = &mTransformationStack.back();
		}

		virtual void begin() = 0;
		virtual void submit(Renderable2D* renderable) = 0;
		virtual void end() {};
		virtual void flush() = 0;

	};
}

#endif

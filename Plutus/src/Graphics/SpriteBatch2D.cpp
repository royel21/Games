
#include <algorithm>
#include <iostream>

#include "ECS/Transform2DComponent.h"
#include "ECS/Component.h"
#include "SpriteBatch2D.h"
#include "Log/Logger.h"

namespace Plutus
{
	SpriteBatch2D::SpriteBatch2D()
	{
	}

	SpriteBatch2D::~SpriteBatch2D()
	{
		mRenderBatches.clear();
		mRenderables.clear();
		glDeleteBuffers(1, &mVBO);
	}

	void SpriteBatch2D::init()
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);

		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
		//Shader position
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid *)NULL);
		//Shader UV "Texture coordinate"
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid *)offsetof(Vertex, Vertex::uv));
		//Shader Color
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid *)offsetof(Vertex, Vertex::color));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint *indices = new GLuint[RENDERER_INDICES_SIZE];

		int offest = 0;
		for (size_t i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i + 0] = offest + 0;
			indices[i + 1] = offest + 1;
			indices[i + 2] = offest + 2;

			indices[i + 3] = offest + 2;
			indices[i + 4] = offest + 3;
			indices[i + 5] = offest + 0;

			offest += 4;
		}

		mIBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);
	}

	void SpriteBatch2D::begin()
	{
		mRenderBatches.clear();
		mRenderables.clear();
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		mBuffer = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void SpriteBatch2D::submit(Renderable2D *renderable)
	{
		mRenderables.push_back(renderable);
	}

	void SpriteBatch2D::end()
	{
		std::stable_sort(mRenderables.begin(), mRenderables.end(), compareTexture);

		if (mRenderables.size())
			mRenderBatches.emplace_back(0, 0, mRenderables[0]->getTexturedId());

		for (size_t cg = 0; cg < mRenderables.size(); cg++)
		{
			auto renderable = mRenderables[cg];
			if (cg > 0 && renderable->getTexturedId() != mRenderables[cg - 1]->getTexturedId())
			{
				mRenderBatches.emplace_back(mIndexCount, 6, renderable->getTexturedId());
			}
			else
			{
				mRenderBatches.back().numVertices += 6;
			}
			const glm::vec2 &position = renderable->getPosition();
			const glm::vec2 &size = renderable->getSize();
			const ColorRGBA8 &c = renderable->getColor();
			glm::vec4 uv = glm::vec4(renderable->getUV());

			if (renderable->getFlipX())
			{
				uv.x = 1 - uv.x;
				uv.z = 1 - uv.z;
			}

			if (renderable->getFlipY())
			{
				uv.y = 1 - uv.y;
				uv.w = 1 - uv.w;
			}

			// Bottom Left corner
			mBuffer->setPosition(position.x, position.y);
			mBuffer->setUV(uv.x, uv.w);
			mBuffer->color = c;
			mBuffer++;
			// Top Left corner
			mBuffer->setPosition(position.x, position.y + size.y);
			mBuffer->setUV(uv.x, uv.y);
			mBuffer->color = c;
			mBuffer++;
			// Top Right corner
			mBuffer->setPosition(position.x + size.x, position.y + size.y);
			mBuffer->setUV(uv.z, uv.y);
			mBuffer->color = c;
			mBuffer++;
			// Bottom Right corner
			mBuffer->setPosition(position.x + size.x, position.y);
			mBuffer->setUV(uv.z, uv.w);
			mBuffer->color = c;
			mBuffer++;

			mIndexCount += 6;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch2D::flush()
	{
		glBindVertexArray(mVAO);
		mIBO->bind();
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
			glDrawElements(GL_TRIANGLES, mRenderBatches[i].numVertices, GL_UNSIGNED_INT, (void *)(mRenderBatches[i].offset * sizeof(GLuint)));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		mIBO->unbind();
		glBindVertexArray(0);
		mIndexCount = 0;
	}

	glm::vec2 SpriteBatch2D::rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);

		return newV;
	}

	bool SpriteBatch2D::compareTexture(Renderable2D *a, Renderable2D *b)
	{
		return (a->getTexturedId() < b->getTexturedId());
	}
} // namespace Plutus
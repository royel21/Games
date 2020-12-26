
#include "SpriteBatch2D.h"

#include "SDL.h"
#include <algorithm>
#include <iostream>

#include "Log/Logger.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"

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
		delete mIBO;
	}

	void SpriteBatch2D::init()
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
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
		delete indices;
	}

	void SpriteBatch2D::begin(uint32_t renderableCount)
	{
		mRenderBatches.clear();
		mRenderables.clear();
		mRenderables.reserve(renderableCount);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	}

	void SpriteBatch2D::submit(Sprite *renderable)
	{
		mRenderables.push_back(renderable);
	}

	void SpriteBatch2D::end()
	{
		// uint32_t start = SDL_GetTicks();
		std::stable_sort(mRenderables.begin(), mRenderables.end(), compareTexture);
		// vertices.reserve(mRenderables.size() * 4);
		// LOG_I("time: {0}", SDL_GetTicks() - start);

		if (mRenderables.size())
			mRenderBatches.emplace_back(0, 0, mRenderables[0]->mTexId);

		for (size_t cg = 0; cg < mRenderables.size(); cg++)
		{
			auto renderable = mRenderables[cg];
			if (cg > 0 && renderable->mTexId != mRenderables[cg - 1]->mTexId)
			{
				mRenderBatches.emplace_back(mIndexCount, 6, renderable->mTexId);
			}
			else
			{
				mRenderBatches.back().numVertices += 6;
			}
			const glm::vec2 &position = renderable->mPosition;
			const glm::vec2 &size = renderable->mSize;
			const ColorRGBA8 &c = renderable->mColor;
			glm::vec4 uv = glm::vec4(renderable->mUVCoord);

			if (renderable->mFlipX)
			{
				uv.x = 1 - uv.x;
				uv.z = 1 - uv.z;
			}

			if (renderable->mFlipY)
			{
				uv.y = 1 - uv.y;
				uv.w = 1 - uv.w;
			}
			vertices.emplace_back(position.x, position.y, uv.x, uv.w, c);
			vertices.emplace_back(position.x, position.y + size.y, uv.x, uv.y, c);
			vertices.emplace_back(position.x + size.x, position.y + size.y, uv.z, uv.y, c);
			vertices.emplace_back(position.x + size.x, position.y, uv.z, uv.w, c);

			mIndexCount += 6;
		}

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		mIBO->bind();
		glBindVertexArray(mVAO);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
			glDrawElements(GL_TRIANGLES, mRenderBatches[i].numVertices, GL_UNSIGNED_INT, (void *)(mRenderBatches[i].offset * sizeof(GLuint)));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIBO->unbind();
		mIndexCount = 0;
		vertices.clear();
	}

	glm::vec2 SpriteBatch2D::rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);

		return newV;
	}

	bool SpriteBatch2D::compareTexture(Sprite *a, Sprite *b)
	{
		return (a->mTexId < b->mTexId);
	}

	void SpriteBatch2D::submit(Transform *trans, Sprite *sprite)
	{
		glm::vec4 uv = glm::vec4(sprite->mUVCoord);
		GLuint textId = sprite->mTexId;
		if (sprite->mFlipX)
		{
			uv.x = 1 - uv.x;
			uv.z = 1 - uv.z;
		}

		if (sprite->mFlipY)
		{
			uv.y = 1 - uv.y;
			uv.w = 1 - uv.w;
		}
		vertices2[textId].emplace_back(trans->position.x, trans->position.y, uv.x, uv.w, sprite->mColor);
		vertices2[textId].emplace_back(trans->position.x, trans->position.y + trans->size.y, uv.x, uv.y, sprite->mColor);
		vertices2[textId].emplace_back(trans->position.x + trans->size.x, trans->position.y + trans->size.y, uv.z, uv.y, sprite->mColor);
		vertices2[textId].emplace_back(trans->position.x + trans->size.x, trans->position.y, uv.z, uv.w, sprite->mColor);
	}

	void SpriteBatch2D::end2()
	{
		// uint32_t start = SDL_GetTicks();
		int i = 0;
		GLuint offset = 0;
		GLuint vertSize = 0;
		for (auto vers : vertices2)
		{
			vertSize += vers.second.size();
			mRenderBatches2[vers.first].texture = vers.first;
			mRenderBatches2[vers.first].offset = offset;
			mRenderBatches2[vers.first].numVertices = ((vers.second.size() / 4) * 6);
			offset = (i + 1) * mRenderBatches2[vers.first].numVertices;
		}

		glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		mIBO->bind();
		glBindVertexArray(mVAO);
		for (size_t i = 0; i < mRenderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
			glDrawElements(GL_TRIANGLES, mRenderBatches[i].numVertices, GL_UNSIGNED_INT, (void *)(mRenderBatches[i].offset * sizeof(GLuint)));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		mIBO->unbind();
		mIndexCount = 0;
		vertices2.clear();
	}
} // namespace Plutus
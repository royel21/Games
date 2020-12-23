#ifndef _SpriteBatch2D_H
#define _SpriteBatch2D_H

#include <vector>

#include "IndexBuffer.h"
#include "vertex.h"

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_VERTEX_SIZE sizeof(Vertex)
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE RENDERER_SPRITE_SIZE *RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2

namespace Plutus
{
	class SpriteComponent;

	class RenderBatch2D
	{
	public:
		RenderBatch2D(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture)
		{
		}
		GLuint offset = 0;
		GLuint numVertices = 0;
		GLuint texture = 0;
	};

	class SpriteBatch2D
	{
	private:
		GLuint mVAO;
		GLuint mVBO;
		IndexBuffer *mIBO;
		GLsizei mIndexCount;
		Vertex *mBuffer;
		std::vector<SpriteComponent *> mRenderables;
		std::vector<RenderBatch2D> mRenderBatches;
		std::vector<Vertex> vertices;

	public:
		SpriteBatch2D();
		~SpriteBatch2D();
		void init();
		void begin(uint32_t renderableCount);

		void submit(SpriteComponent *renderable);
		void end();
		void resizeBuffer(size_t size) { mRenderables.reserve(mRenderables.size() + size); }

	private:
		static glm::vec2 rotatePoint(glm::vec2 pos, float angle);
		static bool compareTexture(SpriteComponent *a, SpriteComponent *b);
	};

} // namespace Plutus
#endif // !_BATHRENDERER2D_H

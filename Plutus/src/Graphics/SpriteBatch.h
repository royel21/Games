#ifndef _SPRITEBATCH_H
#define _SPRITEBATCH_H

#include <glm/glm.hpp>
#include <vector>

#include "GLheaders.h"
#include "vertex.h"

namespace Plutus
{
	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph
	{
	public:
		Glyph() : texture(), depth(0), topLeft(), bottomLeft(), bottomRight(), topRight(){};
		Glyph(
			const glm::vec4 &destRec,
			const glm::vec4 &uvRect,
			const GLuint m_texture,
			float m_depth,
			const ColorRGBA8 &color);
		Glyph(
			const glm::vec4 &destRec,
			const glm::vec4 &uvRect,
			const GLuint m_texture,
			float m_depth,
			const ColorRGBA8 &color,
			float angle);

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex bottomRight;
		Vertex topRight;

	private:
		glm::vec2 rotatePoint(glm::vec2 pos, float angle);
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture)
		{
		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	private:
		GLuint m_vbo;
		GLuint m_vao;

		GlyphSortType m_sortType;
		std::vector<Glyph> m_glyphs; //Those are the actual glyph
		std::vector<RenderBatch> m_renderBatches;
		std::vector<Glyph *> m_glyphPonters; //For sorting

	public:
		SpriteBatch();
		~SpriteBatch();

		void init();
		void setActiveTexture(uint8_t slot);
		//Default Sort Type TEXTURE
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

		void end();
		//Basic draw
		void draw(
			const glm::vec4 &destRec,
			const glm::vec4 &uvRect,
			const GLuint texture,
			float depth,
			const ColorRGBA8 &color);
		//Draw with rotation
		void draw(
			const glm::vec4 &destRec,
			const glm::vec4 &uvRect,
			const GLuint texture,
			float depth,
			const ColorRGBA8 &color,
			float angle);
		//Draw with rotation and direction
		void draw(
			const glm::vec4 &destRec,
			const glm::vec4 &uvRect,
			const GLuint texture,
			float depth,
			const ColorRGBA8 &color,
			const glm::vec2 &dir);

		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyph();

		static bool compareFrontToBack(Glyph *a, Glyph *b);
		static bool compareBackToFront(Glyph *a, Glyph *b);
		static bool compareTexture(Glyph *a, Glyph *b);
	};

} // namespace Plutus

#endif
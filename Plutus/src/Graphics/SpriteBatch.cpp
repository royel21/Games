#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>

namespace Plutus {


	Glyph::Glyph(
		const glm::vec4& destRec,
		const glm::vec4& uvRect,
		const GLuint m_texture,
		float m_depth,
		const ColorRGBA8& color)
	{
		texture = m_texture;
		depth = m_depth;

		topLeft.color = color;
		topLeft.setPosition(destRec.x, destRec.y + destRec.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRec.x, destRec.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRec.x + destRec.z, destRec.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRec.x + destRec.z, destRec.y + destRec.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	Glyph::Glyph(
		const glm::vec4& destRec,
		const glm::vec4& uvRect,
		const GLuint m_texture,
		float m_depth,
		const ColorRGBA8& color,
		float angle)
	{
		glm::vec2 halfDim(destRec.z / 2, destRec.w / 2);

		glm::vec2 tl(-halfDim.x, halfDim.y);
		glm::vec2 bl(-halfDim.x, -halfDim.y);
		glm::vec2 br(halfDim.x, -halfDim.y);
		glm::vec2 tr(halfDim.x, halfDim.y);

		tl = rotatePoint(tl, angle) + halfDim;
		bl = rotatePoint(bl, angle) + halfDim;
		br = rotatePoint(br, angle) + halfDim;
		tr = rotatePoint(tr, angle) + halfDim;

		texture = m_texture;
		depth = m_depth;

		topLeft.color = color;
		topLeft.setPosition(destRec.x + tl.x, destRec.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRec.x + bl.x, destRec.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRec.x + br.x, destRec.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRec.x + tr.x, destRec.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);

		return newV;
	}

	SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0), m_sortType(Plutus::GlyphSortType::TEXTURE)
	{
	}

	SpriteBatch::~SpriteBatch()
	{
		if (m_vbo)
			glDeleteBuffers(1, &m_vbo);
		if (m_vao)
			glDeleteVertexArrays(1, &m_vao);
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::setActiveTexture(uint8_t slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
	}

	void SpriteBatch::begin(GlyphSortType sortType)
	{
		m_sortType = sortType;
		m_renderBatches.clear();
		m_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		m_glyphPonters.resize(m_glyphs.size());
		for (size_t i = 0; i < m_glyphs.size(); i++)
		{
			m_glyphPonters[i] = &m_glyphs[i];
		}

		sortGlyph();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRec, const glm::vec4& uvRect, const GLuint texture, float depth, const ColorRGBA8& color)
	{
		m_glyphs.emplace_back(destRec, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRec, const glm::vec4& uvRect, const GLuint texture, float depth, const ColorRGBA8& color, float angle)
	{
		m_glyphs.emplace_back(destRec, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRec, const glm::vec4& uvRect, const GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir)
	{
		const glm::vec2 right(1.0f, 0.0f);

		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0.0f) angle = -angle;
		m_glyphs.emplace_back(destRec, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(m_vao);
		for (size_t i = 0; i < m_renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(m_glyphs.size() * 6);

		if (m_glyphs.empty()) return;

		int offset = 0;
		int cv = 0; //Current Vertex
		//Create and add a new RenderBatch
		m_renderBatches.emplace_back(offset, 6, m_glyphPonters[0]->texture);

		vertices[cv++] = m_glyphPonters[0]->topLeft;
		vertices[cv++] = m_glyphPonters[0]->bottomLeft;
		vertices[cv++] = m_glyphPonters[0]->bottomRight;
		vertices[cv++] = m_glyphPonters[0]->bottomRight;
		vertices[cv++] = m_glyphPonters[0]->topRight;
		vertices[cv++] = m_glyphPonters[0]->topLeft;

		offset += 6;

		for (size_t cg = 1; cg < m_glyphPonters.size(); cg++) {
			if (m_glyphPonters[cg]->texture != m_glyphPonters[cg - 1]->texture) {

				m_renderBatches.emplace_back(offset, 6, m_glyphPonters[cg]->texture);
			}
			else {
				m_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = m_glyphPonters[cg]->topLeft;
			vertices[cv++] = m_glyphPonters[cg]->bottomLeft;
			vertices[cv++] = m_glyphPonters[cg]->bottomRight;
			vertices[cv++] = m_glyphPonters[cg]->bottomRight;
			vertices[cv++] = m_glyphPonters[cg]->topRight;
			vertices[cv++] = m_glyphPonters[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//Orphan the buffer first
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		//Create the Vertex Array Buffer
		if (!m_vao) {
			glGenVertexArrays(1, &m_vao);
		}
		glBindVertexArray(m_vao);

		//Create the Vertex Array Buffer
		if (!m_vbo) {
			glGenBuffers(1, &m_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		/* Tell OpenGL that we want to use vertexPosition and its location is 0*/
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		/* Tell OpenGL that we want to use vertexUV and its location is 2*/
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		/* Tell OpenGL that we want to use vertexColor and its location is1*/
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyph()
	{
		switch (m_sortType)
		{
		case Plutus::GlyphSortType::NONE:
			break;
		case Plutus::GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphPonters.begin(), m_glyphPonters.end(), compareFrontToBack);
			break;
		case Plutus::GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphPonters.begin(), m_glyphPonters.end(), compareBackToFront);
			break;
		case Plutus::GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphPonters.begin(), m_glyphPonters.end(), compareTexture);
			break;
		default:
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}
}
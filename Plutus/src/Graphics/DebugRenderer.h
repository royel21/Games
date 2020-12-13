#ifndef _DEBUGRENDERER_H
#define _DEBUGRENDERER_H

#include "vertex.h"
#include "Shader.h"
#include "GLheaders.h"
#include "glm/glm.hpp"
#include <vector>

namespace Plutus
{
	class DebugRender
	{
	public:
		DebugRender();
		~DebugRender();
		void init();
		void end();
		void drawLine(const glm::vec2 &a, const glm::vec2 &b, const ColorRGBA8 &color);
		void drawBox(const glm::vec4 &destRect, const ColorRGBA8 &color, float angle);
		void drawCircle(const glm::vec2 &center, const ColorRGBA8 &color, float radius);
		void render(const glm::mat4 &projectionMatrix, float lineWidth);
		void dispose();
		void resizeBuffer(unsigned int size)
		{
			m_vertexs.reserve(size);
		};
		struct DebugVertex
		{
			glm::vec2 position;
			ColorRGBA8 color;
		};

	private:
		Shader m_shader;
		std::vector<DebugVertex> m_vertexs;
		std::vector<GLuint> m_indices;
		GLuint m_vbo, m_vao, m_ibo;
		uint32_t m_numElements = 0;
	};
} // namespace Plutus

#endif // !_DEBUGRENDERER_H
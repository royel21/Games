#ifndef _DEBUGRENDERER_H
#define _DEBUGRENDERER_H

#include "vertex.h"
#include "Shader.h"
#include "GLheaders.h"
#include "glm/glm.hpp"
#include <vector>

namespace Plutus
{
	class Camera2D;
	class Window;

	struct DebugVertex
	{
		glm::vec2 position;
		ColorRGBA8 color;
	};

	class DebugRender
	{
	public:
		DebugRender();
		~DebugRender();
		void init(Window *win, Camera2D *_camera);
		void drawLine(const glm::vec2 &a, const glm::vec2 &b, const ColorRGBA8 &color);
		void drawBox(const glm::vec4 &destRect, const ColorRGBA8 &color, float angle);
		void drawCircle(const glm::vec2 &center, const ColorRGBA8 &color, float radius);
		void render(const glm::mat4 &projectionMatrix, float lineWidth);
		void drawGrid();
		void end();
		void dispose();

		void setGridSize(int gridWidth, int gridHeight);

		inline void resizeBuffer(unsigned int size) { m_vertexs.reserve(size); }

		glm::vec2 getSquareCoords(glm::vec2 mousePos);

	private:
		Shader m_shader;
		std::vector<DebugVertex> m_vertexs;
		std::vector<GLuint> m_indices;
		GLuint m_vbo, m_vao, m_ibo;
		uint32_t m_numElements = 0;
		Camera2D *m_camera = nullptr;
		Window *m_win = nullptr;
		int m_width = 0;
		int m_height = 0;
	};
} // namespace Plutus

#endif // !_DEBUGRENDERER_H
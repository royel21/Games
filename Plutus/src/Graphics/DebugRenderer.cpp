#include "DebugRenderer.h"
#include "Camera2D.h"
#include "Window.h"

#include "Log/Logger.h"

const float PI = 3.14159265359f;

namespace
{
	const char *VERT_SRC = R"(#version 330
		layout(location = 0) in vec2 vertexPosition;
		layout(location = 1) in vec4 vertexColor;
		
		out vec2 fragmentPosition;
		out vec4 fragmentColor;

		uniform mat4 P;

		void main() {
			//Set the x,y position on the screen
			gl_Position.xy = (P * vec4(vertexPosition, 0, 1.0)).xy;
			//the z position is zero since we are in 2D
			gl_Position.z = 0.0;
    
			//Indicate that the coordinates are normalized
			gl_Position.w = 1.0;
    
			fragmentColor = vertexColor;
			fragmentPosition = vertexPosition;
		})";

	const char *FRAG_SRC = R"(#version 330 core
			//The fragment shader operates on each pixel in a given polygon

			in vec2 fragmentPosition;
			in vec4 fragmentColor;

			//This is the 3 component float vector that gets outputted to the screen
			//for each pixel.
			out vec4 color;

			void main() {

				color = fragmentColor;
			})";
} // namespace

namespace Plutus
{
	DebugRender *DebugRender::geInstances()
	{
		static DebugRender *instance = new DebugRender();
		return instance;
	}

	DebugRender::DebugRender() : m_vao(0), m_vbo(0), m_ibo(0)
	{
	}

	DebugRender::~DebugRender()
	{
		dispose();
	}
	void DebugRender::init(Window *win, Camera2D *_camera)
	{
		m_camera = _camera;
		m_win = win;
		m_shader.CreateProgWithShader(VERT_SRC, FRAG_SRC);
		m_shader.setAtribute("vertexPosition");
		m_shader.setAtribute("vertexColor");

		//Set up buffer
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}
	void DebugRender::end()
	{
		//Set Up Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, m_vertexs.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexs.size() * sizeof(DebugVertex), m_vertexs.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Set up Index Buffer Array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numElements = (uint32_t)m_indices.size();
		m_indices.clear();
		m_vertexs.clear();
	}

	void DebugRender::drawLine(const glm::vec2 &a, const glm::vec2 &b, const ColorRGBA8 &color = ColorRGBA8())
	{
		uint32_t i = (uint32_t)m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + 2);

		m_vertexs[i].position = a;
		m_vertexs[i].color = color;
		m_vertexs[i + 1].position = b;
		m_vertexs[i + 1].color = color;

		m_indices.push_back(i);
		m_indices.push_back(i + 1);
	}

	glm::vec2 rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);

		return newV;
	}

	void DebugRender::drawBox(const glm::vec4 &destRect, const ColorRGBA8 &color, float angle)
	{
		uint32_t i = (uint32_t)m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + 4);

		glm::vec2 halfDim(destRect.z / 2.0f, destRect.w / 2.0f);

		glm::vec2 tl(-halfDim.x, halfDim.y);
		glm::vec2 bl(-halfDim.x, -halfDim.y);
		glm::vec2 br(halfDim.x, -halfDim.y);
		glm::vec2 tr(halfDim.x, halfDim.y);

		glm::vec2 positionOffset(destRect.x, destRect.y);

		m_vertexs[i].position = rotatePoint(tl, angle) + halfDim + positionOffset;
		m_vertexs[i + 1].position = rotatePoint(bl, angle) + halfDim + positionOffset;
		m_vertexs[i + 2].position = rotatePoint(br, angle) + halfDim + positionOffset;
		m_vertexs[i + 3].position = rotatePoint(tr, angle) + halfDim + positionOffset;

		for (uint32_t j = i; j < i + 4; j++)
			m_vertexs[j].color = color;

		m_indices.reserve(m_indices.size() + 8);

		m_indices.push_back(i);
		m_indices.push_back(i + 1);

		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);

		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);

		m_indices.push_back(i + 3);
		m_indices.push_back(i);
	}

	void DebugRender::drawCircle(const glm::vec2 &center, const ColorRGBA8 &color, float radius)
	{
		static const uint32_t NUM_VERTS = 100;

		uint32_t start = (uint32_t)m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + NUM_VERTS);
		for (size_t i = 0; i < NUM_VERTS; i++)
		{
			float angle = ((float)i / NUM_VERTS) * 2.0f * PI;
			m_vertexs[start + i].position.x = cos(angle) * radius + center.x;
			m_vertexs[start + i].position.y = sin(angle) * radius + center.y;
			m_vertexs[start + i].color = color;
		}
		//Set up indixed drawing
		m_indices.reserve(m_indices.size() + NUM_VERTS * 2);
		for (uint32_t i = 0; i < NUM_VERTS - 1; i++)
		{
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}

		m_indices.push_back(start + NUM_VERTS - 1);
		m_indices.push_back(start);
	}

	void DebugRender::render(const glm::mat4 &projectionMatrix, float lineWidth)
	{

		m_shader.enable();
		m_shader.setUniformMat4("camera", projectionMatrix);

		glLineWidth(lineWidth);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_shader.disable();
	}
	void DebugRender::dispose()
	{
		if (m_vao)
			glDeleteVertexArrays(1, &m_vao);
		if (m_vbo)
			glDeleteBuffers(1, &m_vao);
		if (m_ibo)
			glDeleteBuffers(1, &m_vao);
		m_shader.dispose();
	}
	void DebugRender::drawGrid()
	{
		if (isDraw)
		{
			ColorRGBA8 color;
			color.a = 255;
			glm::vec2 scaleScreen = m_camera->getScaleScreen();

			glm::vec2 screenStart = m_camera->getPosition() - (scaleScreen / 2.0f);
			glm::vec2 screenEnd = m_camera->getPosition() + (scaleScreen / 2.0f);
			glm::vec2 lineStart;
			glm::vec2 lineEnd;

			// LOG_I("SC: {0} SC:{1}", scaleScreen.x, scaleScreen.y);

			int sizeX = static_cast<int>((scaleScreen.x) / m_width) + 2;
			int sizeY = static_cast<int>((scaleScreen.y) / m_height) + 2;

			float x = floor(screenStart.x / m_width);
			float y = floor(screenStart.y / m_height);

			glm::vec2 cPos(x * m_width, y * m_height);

			for (int x = 0; x <= sizeX; x++)
			{
				lineStart.x = cPos.x + (x * m_width);
				lineStart.y = cPos.y;

				lineEnd.x = cPos.x + (x * m_width);
				lineEnd.y = screenEnd.y;
				drawLine(lineStart, lineEnd, color);
			}

			for (int y = 0; y <= sizeY; y++)
			{
				lineStart.x = cPos.x;
				lineStart.y = cPos.y + (y * m_height);

				lineEnd.x = screenEnd.x;
				lineEnd.y = cPos.y + (y * m_height);
				drawLine(lineStart, lineEnd, color);
			}
			end();

			render(m_camera->getCameraMatrix(), 1.0f);
		}
	}

	glm::vec2 DebugRender::getSquareCoords(glm::vec2 mousePos)
	{
		glm::vec2 cmpos = m_camera->convertScreenToWoldInv(mousePos);
		glm::vec2 center(m_win->getScreenWidth() / 2, m_win->getScreenHeight() / 2);
		cmpos += center;

		int x = (int)floor(cmpos.x / m_width);
		int y = (int)floor(cmpos.y / m_height);

		glm::vec2 coords(
			(float)(x * m_width) - center.x,
			(float)(y * m_height) - center.y);
		return coords;
	}

	void DebugRender::setGridSize(int gridWidth, int gridHeight)
	{
		m_width = gridWidth;
		m_height = gridHeight;
	}
	glm::vec2 DebugRender::getGridSize()
	{
		return glm::vec2(m_width, m_height);
	}
} // namespace Plutus
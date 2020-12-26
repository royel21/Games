#ifndef _TEXTLAYER_H
#define _TEXTLAYER_H

#include <map>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <glm/glm.hpp>

#include "Graphics/GLheaders.h"
#include "Graphics/Shader.h"

namespace Plutus
{

	class Shader;
	class Window;

	struct TextCharacter
	{
		GLuint TextureID;	// ID handle of the glyph texture
		glm::ivec2 Size;	// Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		GLuint Advance;		// Horizontal offset to advance to next glyph
		TextCharacter() : TextureID(0), Size(1), Bearing(1), Advance(0){};
		TextCharacter(GLuint texid, glm::ivec2 sz, glm::ivec2 bring, GLuint adv) : TextureID(texid), Size(sz), Bearing(bring), Advance(adv){};
	};

	class TextLayer
	{
	private:
		GLuint pVAO;
		GLuint pVBO;
		float pWidth;
		float pHeight;

		Window *pWindow;
		Shader pShader;

		std::map<char, TextCharacter> pCharset;
		GLushort mActiveTexture;
		glm::vec4 color;

	public:
		TextLayer();
		~TextLayer();

		bool Init(
			Window *window,
			std::string fontPath,
			uint32_t fontSize);

		void setActiveTexture(GLushort slot) { mActiveTexture = slot; }

		inline void setColor(float r, float g, float b, float a) { color = glm::vec4(r, g, b, a); };
		inline void setColor(float r, float g, float b) { color = glm::vec4(r, g, b, 1); };

		void drawString(std::string text, GLfloat x, GLfloat y, GLfloat scale);
		void drawString(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color);
		void updateCamera(int screenWidth, int screenHeight);

	private:
		void initFreetypeFont(std::string fontPath, uint32_t fontSize);
	};
} // namespace Plutus

#endif
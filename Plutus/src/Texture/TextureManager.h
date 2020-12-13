
#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include <iostream>
#include <map>

#include "Graphics/GLheaders.h"

namespace Plutus
{

	struct GLTexture
	{
		uint32_t id;
		int width;
		int height;
	};

	class TextureManager
	{
	private:
		static GLuint minFilter;
		static GLuint magFilter;
		static std::map<std::string, GLTexture> m_textureMap;

	public:
		static void destroyTextures();
		static GLTexture getTexture(std::string filepath, GLshort id = 0);
		static void setTextureFilter(GLuint filter, bool min)
		{
			min ? minFilter = filter : magFilter = filter;
		}

	private:
		static GLTexture loadImage(std::string filePath, GLshort id);
	};
} // namespace Plutus

#endif
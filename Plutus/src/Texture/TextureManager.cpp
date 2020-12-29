#include <vector>
#include "TextureManager.h"
#include "stb_image.h"

namespace Plutus
{
	std::map<std::string, GLTexture> TextureManager::m_textureMap;
	GLuint TextureManager::minFilter = GL_NEAREST;
	GLuint TextureManager::magFilter = GL_NEAREST;

	void TextureManager::destroyTextures()
	{
		for (auto &tm : m_textureMap)
		{
			glDeleteTextures(1, &tm.second.id);
		}
		m_textureMap.clear();
	}

	GLTexture TextureManager::getTexture(std::string texturePath, GLshort id)
	{
		auto mit = m_textureMap.find(texturePath);

		//check if the texture exist if not load a new one
		if (mit == m_textureMap.end())
		{
			//Load the texture
			GLTexture newTexture = loadImage(texturePath, id);

			//Insert it into the map
			m_textureMap.insert(make_pair(texturePath, newTexture));
			return newTexture;
		}
		return mit->second;
	}

	GLTexture TextureManager::loadImage(std::string filePath, GLshort id = 0)
	{
		GLTexture texture = {};

		int BPP;
		uint8_t *out = stbi_load(filePath.c_str(), &texture.width, &texture.height, &BPP, 4);

		glGenTextures(1, &texture.id);
		//link the image to a texture in the gpu texture array

		glBindTexture(GL_TEXTURE_2D, texture.id);
		//flag to render the image
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		//Load the image to the memory of the gpu
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out);

		//See https://en.wikipedia.org/wiki/Mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
		//unlink the texture
		glBindTexture(GL_TEXTURE_2D, 0);
		//delete the image buffer from memory
		if (out)
		{
			stbi_image_free(out);
		}

		return texture;
	}
} // namespace Plutus
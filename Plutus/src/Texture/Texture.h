#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "TextureManager.h"

namespace Plutus {

	class Texture {
	private:
		GLTexture m_Texture;
		GLshort m_actId;
	public:
		Texture() : m_Texture(), m_actId(0) {};
		Texture(std::string name, GLshort id = 0) {
			m_actId = id;
			m_Texture = TextureManager::getTexture(name, id);
		}

		~Texture() { glDeleteTextures(1, &m_Texture.id); }

		void bind() {
			glActiveTexture(GL_TEXTURE0+ m_actId);
			glBindTexture(GL_TEXTURE_2D, m_Texture.id);
		}

		void unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		GLuint  getId() { return m_Texture.id; }
	};
}
#endif
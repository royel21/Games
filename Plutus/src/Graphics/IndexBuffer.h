#ifndef _INDEXBUFFER_H
#define _INDEXBUFFER_H

#include "GLheaders.h"

namespace Plutus
{
	class IndexBuffer
	{
	private:
		GLuint mIboID;
		GLuint mCount;

	public:
		IndexBuffer(GLushort *data, GLuint count);
		IndexBuffer(GLuint *data, GLuint count);
		~IndexBuffer();

		void bind() const;

		void unbind() const;

		GLuint getCount() const { return mCount; }
	};
} // namespace Plutus
#endif
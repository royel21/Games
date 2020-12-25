#include "IndexBuffer.h"

namespace Plutus
{
	IndexBuffer::IndexBuffer(GLushort *data, GLuint count) : mIboID(0), mCount(count)
	{
		glGenBuffers(1, &mIboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(GLuint *data, GLuint count) : mIboID(0), mCount(count)
	{
		glGenBuffers(1, &mIboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &mIboID);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboID);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace Plutus
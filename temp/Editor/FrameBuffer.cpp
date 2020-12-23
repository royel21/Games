#include "FrameBuffer.h"
#include "glad/glad.h"
#include "assert.h"

void FrameBuffer::init(float width, float height, bool reset)
{
    mSize = glm::vec2(width, height);
    if (reset)
    {
        glDeleteFramebuffers(1, &fboId);
        glDeleteTextures(1, &textId);
        glDeleteRenderbuffers(1, &rboId);
    }
    //Grenerate a frame buffer
    glGenFramebuffers(1, &fboId);
    //bind the buffer for imediate use
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    //Genenerate a texture for draw our buffer
    glGenTextures(1, &textId);
    glBindTexture(GL_TEXTURE_2D, textId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //See https://en.wikipedia.org/wiki/Mipmap
    // glGenerateMipmap(GL_TEXTURE_2D);
    //attach the buffer to the texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textId, 0);
    // Create render buffer for store the depth info
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        assert("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &fboId);
    glDeleteTextures(1, &textId);
    glDeleteRenderbuffers(1, &rboId);
}

void FrameBuffer::bind()
{
    if (isDirty)
    {
        init(mSize.x, mSize.y, true);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glBindTexture(GL_TEXTURE_2D, textId);
    glViewport(0, 0, mSize.x, mSize.y);
}

void FrameBuffer::unBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

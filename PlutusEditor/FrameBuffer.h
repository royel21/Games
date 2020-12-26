#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "types.h"
#include "glm/glm.hpp"

class FrameBuffer
{
public:
    FrameBuffer(){};
    ~FrameBuffer();
    void init(float width, float height, bool reset = false);
    void bind();
    void unBind();

    void resize(float w, float h)
    {
        isDirty = true;
        mSize = glm::vec2(w, h);
    }

    glm::vec2 getSize() { return mSize; }
    void setDirty() { isDirty = true; };

    uint32 getTextureId() const
    {
        return textId;
    }

private:
    //Framebuffer Id
    uint32 fboId = 0;
    //texture Id
    uint32 textId = 0;
    //renderbuffer Id
    uint32 rboId = 0;
    bool isDirty = false;
    glm::vec2 mSize;
};

#endif
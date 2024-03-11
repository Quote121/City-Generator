#pragma once

class FrameBuffer
{
private:
    unsigned int FBO;

    unsigned int frameTexture;
public:
    FrameBuffer(unsigned int windownWidth, unsigned int windowHeight);
    ~FrameBuffer();

};


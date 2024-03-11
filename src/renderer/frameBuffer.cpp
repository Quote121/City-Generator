#include <frameBuffer.hpp>
#include <glad/glad.h>

FrameBuffer::FrameBuffer(unsigned int windowWidth, unsigned int windowHeight)
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Generate the texture as an information buffer
    glGenTextures(1, &frameTexture);
    glBindTexture(GL_TEXTURE_2D, frameTexture);

    // Creating one with int as the data type as in the toutorial
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32I, windowWidth, windowHeight, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the colour attachement to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);


}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &FBO);
}



#include "texture.hpp"

void TextureLoader::loadTexture(const char *path, unsigned int &textureID, GLenum format)
{
    // no of textures, gens id
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Mipmaps
    // texture images of progressively smaller sizes (twice as small as other)
    // glGenerateMipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading a texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); // unsigned chars (bytes)

    if (data) {
        // Now we generate the texture with glTexImage2D // page 60 for extra details
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); // 2nd arg specifies the mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        
        std::cout << "Texture loaded WxH: " << width << " x " << height << " pixels" << std::endl;

    }
    else {
        std::cout << "Failed to load texture at: " << path << std::endl;
    }
    // free image memory
    stbi_image_free(data);
}

void TextureLoader::activateTexture(unsigned int texture, unsigned int id)
{
    // GL_TEXTURE0 + id // acts as a offset from GL_TEXTURE0
    // read glActiveTexture to make more sence
    glActiveTexture(GL_TEXTURE0+id);
    glBindTexture(GL_TEXTURE_2D, texture);
}

#include <glad/glad.h>
#include <string>
#include <iostream>

#include "stb_image/stb_image.h"

class TextureLoader {

public:
    
    ////////////////////////////
    // Method used to load textures
    // ==== Args ====
    // path - path to texture
    // texture id - the value that will be associated with this texture
    // format - either GL_RBG or GL_RBGA for now as we have images with alpha channels
    // ==== Returns ====
    // void
    // updates textureID that is passed by reference
    ///////////////////////////
    static void loadTexture(const char *path, unsigned int &textureID, GLenum format);
    
    ////////////////////////////
    // Method used to activate and bind textures
    // ==== Args ====
    // texture - the associated texture id
    // id - specifies which texture unit to make active
    ///////////////////////////
    static void activateTexture(unsigned int texture, unsigned int id);

};
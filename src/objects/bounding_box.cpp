#include <bounding_box.hpp>

#include <resourceManager.hpp>
#include <config.hpp>

void BoundingBox::SetupBuffers()
{
    float vertices[]{
        // Top square
        //  x  y  z
        max[0], max[1], max[2], 
        //  x  y -z
        max[0], max[1], min[2],
        // -x  y -z
        min[0], max[1], min[2],
        // -x  y  z
        min[0], max[1], max[2],

        // Bottom square
        //  x -y  z
        max[0], min[1], max[2],
        //  x -y -z
        max[0], min[1], min[2],
        // -x -y -z
        min[0], min[1], min[2],
        // -x -y  z
        min[0], min[1], max[2],

        // Forward right vertical
        max[0], max[1], max[2], 
        max[0], min[1], max[2],
        // Back right vertical
        max[0], max[1], min[2],
        max[0], min[1], min[2],
        // Back left vertical
        min[0], max[1], min[2],
        min[0], min[1], min[2],
        // Forward left vertical
        min[0], max[1], max[2],
        min[0], min[1], max[2]
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind
}


BoundingBox::BoundingBox()
{
    // Set min and max to the greatest and smallest values respectivley upon initalization
    min = glm::vec3(std::numeric_limits<float>::max());
    max = glm::vec3(-std::numeric_limits<float>::max());

    boundingBoxShader = ResourceManager::getInstance()->LoadShader(paths::bb_defaultVertShaderPath, paths::bb_defaultFragShaderPath);
    
    // Setup VAO and VBOs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

BoundingBox::~BoundingBox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// When creating the object we use this method
void BoundingBox::StreamVertexUpdate(glm::vec3& point)
{
    min = glm::min(min, point);
    max = glm::max(max, point);
}
void BoundingBox::StreamVertexUpdate(float x, float y, float z)
{
    min = glm::min(min, {x, y, z});
    max = glm::max(max, {x, y, z});
}

// When we update the bounding box after creation we use this method
void BoundingBox::Update(glm::vec3& point)
{
    min = glm::min(min, point);
    max = glm::max(max, point);

    // Since we need to update the vertices in the buffer objects
    SetupBuffers();
}

// Draw function maybe for a wireframe like draw

// The setup for the box VAO and VBO will be done everytime the update() is called
void BoundingBox::Draw()
{
    glBindVertexArray(VAO);

    // TODO change this to one glDrawElements call

    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    glDrawArrays(GL_LINES, 8, 8);

    glBindVertexArray(0);

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOG(ERROR, "OpenGL BoundingBox::Draw() Error: " << error);
    }
}

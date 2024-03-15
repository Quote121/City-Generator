#include <bounding_box.hpp>

#include <resourceManager.hpp>
#include <config.hpp>
#include <renderer.hpp>

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
        min[0], min[1], max[2]
    };

    unsigned int indices[] = {
        // Top square
        0, 1, 1, 2, 2, 3, 3, 0,
        // Bottom square
        4, 5, 5, 6, 6, 7, 7, 4,
        // Vertical lines
        0, 4, 1, 5, 2, 6, 3, 7
    };

    VertexBufferLayout vbl;
    vbl.AddFloat(3);

    EBO->SetData(indices, 24);

    VBO->SetData<float>(vertices, 24);
    VAO->AddBuffer(VBO, &vbl);
}


BoundingBox::BoundingBox()
{
    // Set min and max to the greatest and smallest values respectivley upon initalization
    min = glm::vec3(std::numeric_limits<float>::max());
    max = glm::vec3(-std::numeric_limits<float>::max());

    boundingBoxShader = ResourceManager::getInstance()->LoadShader(paths::bb_defaultVertShaderPath, paths::bb_defaultFragShaderPath);
    
    VAO = new VertexArray();
    VBO = new VertexBuffer();
    EBO = new IndexBuffer();
}

BoundingBox::~BoundingBox()
{
    delete(VAO);
    delete(VBO);
    delete(EBO);
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
void BoundingBox::Draw()
{
    Renderer::GetInstance()->DrawIndices(VAO, EBO, GL_LINES);
}


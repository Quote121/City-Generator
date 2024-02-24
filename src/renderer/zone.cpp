#include <zone.hpp>
#include <config.hpp>
#include <renderer.hpp>

Zone::Zone(const std::array<glm::vec3,4>& array, const float width)
{
    VAO = new VertexArray();
    VBO = new VertexBuffer();

    UpdateVertices(array, width);
}

Zone::~Zone()
{
    delete(VAO);
    delete(VBO);
}

// Method that updates and draws the rectangle for the zone
void Zone::UpdateVertices(const std::array<glm::vec3, 4>& array, const float width)
{
    std::array<float, 12> verts;

    for (unsigned int i = 0; i < array.size(); i++)
    {
        verts[i*3] = array[i].x;
        verts[(i*3)+1] = array[i].y;
        verts[(i*3)+2] = array[i].z;
    }

    VertexBufferLayout vbl;
    vbl.AddFloat(3);

    VBO->SetData<float>(verts.data(), verts.size());
    VAO->AddBuffer(VBO, &vbl);
}

void Zone::Draw()
{
    Renderer::GetInstance()->DrawArrays(VAO, 4, GL_LINE_LOOP); 
}


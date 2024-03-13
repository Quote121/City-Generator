#pragma once

#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "vertexBuffer.hpp"
#include <glm/glm.hpp>
#include <config.hpp>

// Defined outside of Glad as we cant include several times
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007

// For max 40 vert circle roads // TODO check these values
#define ROAD_MAX_VERTICES 48 // no dupes
#define ROAD_MAX_INDICES 138 // for the 46 GL_TRIANGLES
// Max bytes for a buffer, 6 is for xyz aPos and xyz normals
constexpr unsigned int ROAD_MAX_VERT_BUFFER_SIZE_BYTES = ROAD_MAX_VERTICES * 6 * sizeof(float);
constexpr unsigned int ROAD_MAX_VERT_BUFFER_SIZE = ROAD_MAX_VERTICES * 6;
constexpr unsigned int ROAD_MAX_IND_BUFFER_SIZE_BYTES = ROAD_MAX_INDICES * sizeof(float);

struct InstanceObject
{
    void* address;
    unsigned long renderID;
};

template<typename T>
class InstanceRenderer
{
private:
    std::vector<InstanceObject> objects;
    std::vector<float> matrices;

public:
    // @brief Add another object
    // @param object pointer to add
    void Append(T object);

    // @brief Remove an object
    // @param object pointer to remove
    void Remove(T object);

    // @brief Remove all objects
    void Clear(void);

    // @brief Update one of the objects
    // @brief object pointer to update
    void Update(T object);

    // @brief Render to screen
    void Draw(void);

};

// Batch renderer, for now will only batch render roads as we have no other objects that need this. (Others needs instancing, sprites and buildings)
class BatchRenderer
{
private:

    VertexBuffer* VBO;
    VertexArray* VAO;
    IndexBuffer* EBO;
    unsigned int IBO; // Indirect buffer

    // indirectCommands are put into
    // GL_DRAW_INDIRECT_BUFFER
    
    // Update method for updating a specific set of vertices and indices 
public:
    BatchRenderer();
    ~BatchRenderer();

    // Update all roads vertices and indices
    void UpdateAll(void);

    // Update one set of road vertices and indices
    // Roads call an update and pass their renderID which is used to find them in the List
    void Update(const unsigned int renderID, std::vector<float> const* vertices, std::vector<unsigned int> const* indices);

    void DrawBatch(glm::mat4 view, glm::mat4 projection) const; 

};


class Renderer
{
private:

    // Batches for rendering
    // Each batch contains a VAO, VBO, EBO and shader, they all share the same shader
    // Each object in the batch should know which batch they are in. And where in the buffers they are
    // An object can directly access and update the batch
    // std::vector<Batch> Batches;

    static Renderer* pInstance;  

    glm::vec3 backgroundColour = DEFAULT_BACKGROUND_COLOUR; 

    Renderer() = default;
    ~Renderer();

public:
    // Singleton
    Renderer(Renderer &other) = delete;
    void operator=(const Renderer &) = delete;
    static Renderer* GetInstance();

    void SetClearScreenColour(glm::vec3 colour);

    void ClearScreen(void) const;
    // Make sure shaders are bound before calls to draw, mode is the primative to draw, default gl_triangle
    void DrawIndices(const VertexArray* vao, const IndexBuffer* ebo, unsigned int mode = GL_TRIANGLES);
    // @params count - number of arrays to draw with primative from vao
    void DrawArrays(const VertexArray* vao, unsigned int count, unsigned int mode = GL_TRIANGLES);

    void DrawBatches();
};


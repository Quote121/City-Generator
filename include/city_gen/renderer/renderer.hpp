#pragma once

#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "vertexBuffer.hpp"

// Defined outside of Glad as we cant include several times
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007

// Max buffer sizes

// 40 point sides 22 vertices for each semi-circle
// 8 for middle
//
// 44 + 8 = 52 vertices (4 duplicates) for a road
//
// Indices:
//
// 20 sections for each semi-circle
// 4 sections for middle
// 44 triangles, 44*3 = 132 indices
#define ROAD_MAX_VERTICES 52 // 4 dupes
// #define ROAD_MAX_VERTICES 48 // no dupes
#define ROAD_MAX_INDICES 132 // for the 44 GL_TRIANGLES


// Indrect command structure
struct DrawElementsIndirectCommand {
    unsigned int count;
    unsigned int instanceCount; // not used
    unsigned int firstIndex;
    unsigned int baseVertex;    // not used
    unsigned int baseInstance;  // not used
};

// Batch renderer, for now will only batch render roads as we have no other objects that need this. (Others needs instancing, sprites and buildings)
class Batch
{
private:
    // List of commands for each road (will be mostly the same)
    std::vector<DrawElementsIndirectCommand> indirectCommands;

    VertexBuffer* VBO;
    VertexArray* VAO;
    IndexBuffer* EBO;
    unsigned int IBO; // Indirect buffer

    // indirectCommands are put into
    // GL_DRAW_INDIRECT_BUFFER
    
    // Update method for updating a specific set of vertices and indices 
public:
    Batch();
    ~Batch();

    // Update all roads vertices and indices
    void UpdateAll(void);

    // Update one set of road vertices and indices
    void Update(void);

    void DrawBatch(void) const; 

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

    Renderer() = default;
    ~Renderer();

public:
    // Singleton
    Renderer(Renderer &other) = delete;
    void operator=(const Renderer &) = delete;
    static Renderer* GetInstance();


    void ClearScreen(void) const;
    // Make sure shaders are bound before calls to draw, mode is the primative to draw, default gl_triangle
    void DrawIndices(const VertexArray* vao, const IndexBuffer* ebo, unsigned int mode = GL_TRIANGLES);
    // @params count - number of arrays to draw with primative from vao
    void DrawArrays(const VertexArray* vao, unsigned int count, unsigned int mode = GL_TRIANGLES);

    void DrawBatches();
};


#pragma once

#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "vertexBuffer.hpp"
#include <glm/glm.hpp>
#include <config.hpp>
#include <road_object.hpp>

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

template<class T>
class InstanceRenderer
{
private:
    // Dynamic arrays of the objects and matrices of each object
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

    // @brief sometimes we will need to update all matrix data
    void UpdateAll(void);

    // @brief Render to screen
    void Draw(void);

    // @brief Get the object the instancerenderer is using
    // @return A pointer to the first object in the instance renderer, if empty then nullptr
    const T GetInstanceType(void) const;

    // @brief Get number of objects in renderer
    // @returns size_t number of objects in instance renderer
    const size_t size(void) const;
};

// Batch renderer is only setup to draw simple geometry such as roads
class BatchRenderer
{
private:
    VertexBuffer* VBO;
    VertexArray* VAO;
    IndexBuffer* EBO;
public:
    BatchRenderer();
    ~BatchRenderer();

    // @brief Update all roads vertices and indices
    void UpdateAll(void);

    // @brief Update one road's vertices and indices
    // @args renderID - the renderID owned by the road
    // @args vertices - pointer to a dynamic array of the vertices of the road
    // @args indices - pointer to a dynamic array of the indices of the road
    void Update(const unsigned int renderID, std::vector<float> const* vertices, std::vector<unsigned int> const* indices);

    // @brief Draws the current batch
    // @args view - view matrix of the camera
    // @args projection - projection matrix of the camera
    void DrawBatch(glm::mat4 view, glm::mat4 projection) const; 

    // @brief Deletes a roads vertices and indices from the batch renderer
    // @args road - a pointer to the road object
    void Delete(const RoadObject* road);
};


class Renderer
{
private:
    // Batches for rendering
    // Each batch contains a VAO, VBO, EBO and shader, they all share the same shader
    // Each object in the batch should know which batch they are in. And where in the buffers they are
    // An object can directly access and update the batch
    // std::vector<Batch> Batches;

    glm::vec3 backgroundColour = DEFAULT_BACKGROUND_COLOUR; 

    // Singleton
    static Renderer* pInstance;  
    Renderer() = default;
    ~Renderer();
public:
    // Singleton
    Renderer(Renderer &other) = delete;
    void operator=(const Renderer &) = delete;
    static Renderer* GetInstance();

    // @brief Set the glfw clear screen colour upon each new frame
    // @args glm::vec3 colour - rgb colour
    void SetClearScreenColour(glm::vec3 colour);

    // @brief Clear the screen for rendering the next frame
    void ClearScreen(void) const;

    // @brief draw the indices bound by the VAO and EBO
    // @args vao - vertex array data
    // @args ebo - index array data
    // @args mode - the draw mode, default triangles
    // @note Make sure shaders are bound before this call is made
    void DrawIndices(const VertexArray* vao, const IndexBuffer* ebo, unsigned int mode = GL_TRIANGLES);

    // @brief draw the vertices specificed in the vao
    // @args vao - vertex array with the vertex data
    // @args count - number of primatives to draw
    // @args mode - the draw mode, default triangles
    // @notes Make sure shaders are setup and bound before calling 
    void DrawArrays(const VertexArray* vao, unsigned int count, unsigned int mode = GL_TRIANGLES);
};


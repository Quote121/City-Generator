// #pragma once

// #include <base_object.hpp>

// enum LightType
// {
//     DIRECTIONAL,
//     POINT,
//     SPOTLIGHT
// };


// class LightObject : public BaseObject<LightObject>
// {
// private:
//     glm::vec3 lightColour = {1.0f, 1.0f, 1.0f};
//     float intensity = 1.0f;
//     LightType lightType;

// public:
//     LightObject(Shader *shader_in,
//                 LightType lightType_in) :
//                 BaseObject()
//     {
        
//     }
    
//     static std::string GetStringLightType(LightType type)
//     {
//         switch (type)
//         {
//         case (LightType::DIRECTIONAL):
//             return std::string("DIRECTIONAL");
//         case (LightType::POINT):
//             return std::string("POINT");
//         case (LightType::SPOTLIGHT):
//             return std::string("SPOTLIGHT");
//         default:
//             LOG(ERROR, "Invalid LightType for GetStringLightType(LightType type)");
//             return nullptr;
//         }
//     }


//     void Draw(glm::mat4 view, glm::mat4 projection) override
//     {

//     }

// };
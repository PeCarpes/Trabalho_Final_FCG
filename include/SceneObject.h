#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Matrices.h>

#include <ObjModel.h>
#include <Texture.h>
#include <Shader.h>
#include <Camera.h>
#include <Bezier.h>

#include <stack>

class SceneObject
{
private:
    glm::vec4 position;
    glm::vec3 rotation; // Stored in RADIANS
    glm::vec3 scale;
    glm::vec4 upVector;

    bool is_bobbing = false;
    bool useViewMatrix = true; // If false, uses the identity matrix
    GLuint GpuProgramID;
    std::string name;
    ObjModel objModel;

    GLuint texture_id;
    GLuint object_id;

    Texture3D texture;
    const Camera &cam;
    Shader shader;

    Bezier bobbing_curve;
    float bobbing_cps = 1.0f; // Cycles per second for bobbing

    bool isCollidable = false;

    public:
    void setPosition(const glm::vec3 &newPosition);
    void addPosition(const glm::vec3 &deltaPosition);
    void setRotationX(float degrees);
    void setRotationY(float degrees);
    void setRotationZ(float degrees);
    void setRotation(glm::vec3 newRotation);
    void addRotation(glm::vec3 deltaRotation);
    void setScaleX(float factor);
    void setScaleY(float factor);
    void setScaleZ(float factor);
    void setScale(const glm::vec3 &newScale);
    void setID(int newID);
    
    void setUpVector(const glm::vec4 &newUp);
    void setBobbingCurve(const Bezier &curve) { bobbing_curve = curve; }
    void setBobbingCPS(float cps) { bobbing_cps = cps; is_bobbing = true; }
    void bob(void);    

    void setHeight(float height); // Changes ALL the dimensions of the object to match the height
    
    void setTexture(const std::string &filename)
    {
        texture.LoadTextureImage(filename.c_str());
        texture_id = texture.getTextureID();
    }
    
    void setTexture(GLuint textureID)
    {
        texture_id = textureID;
    }

    void setTexture(const Texture3D &tex)
    {
        texture = tex;
        texture_id = texture.getTextureID();
    }

    float height() const;

    std::string getName() const;
    glm::vec4 getPosition() const;

    void draw() const;

    SceneObject(const ObjModel &model, const std::string &name, Shader shader, const Camera &cam, bool useViewMatrix = true, bool collidable = false);
};
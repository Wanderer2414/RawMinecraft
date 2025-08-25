#ifndef GLTF_MODEL_H
#define GLTF_MODEL_H

#include "Color.h"
#include "FlyweightStorage.h"
#include "GLTFAnimation.h"
#include "GLTFMesh.h"

namespace MyCraft {
    class GLTFModel: public MyBase::FlyWeightObject {
    public:
        GLTFModel(const std::string& modelPath = "");
        ~GLTFModel();

        virtual glm::mat4x3 getShape()              const   = 0;
        virtual glm::vec3   getPosition()           const   = 0;
        virtual void look(const glm::vec3& position)        = 0;
        virtual bool attack()                               = 0;
        virtual void see(const glm::vec3& direction)        = 0;
        virtual void move(const glm::vec3& direction)       = 0;
        virtual void rotate(const glm::vec3& direction)     = 0;
        virtual void setPosition(const glm::vec3& position) = 0;
        virtual bool apply()                                = 0;
        void setBaseColor(const MyBase::Color& color);
    protected:
        void draw() const;
        GLTFStaticMesh::SetNode& States();
        GLTFAnimation& Animations(const std::string& name);
    private:
        GLTFStaticMesh::SetNode *__states;
        MyBase::Color   __baseColor;
        GLuint          __baseColorBuffer;
        MyBase::FlyWeightCore* create(const std::string& src) const override;
    };
}

#endif
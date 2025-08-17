
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
        virtual void attack()                               = 0;
        virtual void see(const glm::vec3&)                  = 0;
        virtual void move(const glm::vec3& direction)       = 0;
        virtual void rotate(const glm::vec3& direction)     = 0;
        virtual void setPosition(const glm::vec3& position) = 0;
        virtual bool apply()                                = 0;
    protected:
        void draw() const;
        GLTFStaticMesh::SetNode& States();
        GLTFAnimation& Animations(const std::string& name);
    private:
        MyBase::FlyWeightCore* create(const std::string& src) const override;
    };
}

#include "FlyweightStorage.h"
#include "GLTFMesh.h"

#include <string>
namespace MyCraft {
    class GLTFModel: public MyBase::FlyWeightObject {
    public:
        GLTFModel(const std::string& modelPath = "");
        ~GLTFModel();

        void Draw(const glm::mat4& modelMatrix) const;

        glm::mat4x3 getShape();

        void setPosition(const glm::vec3& position);
        void move(const glm::vec3& offset);
        void setDimensions(const glm::vec3& dimensions);
        void setScale(float scale);
        void scale(float factor);

        void setRotation(float angle, const glm::vec3& axis);
        void rotate(float angle, const glm::vec3& axis); 

        glm::vec3 getPosition() const;
        glm::vec3 getDimensions() const;
        float getScale() const;
        float getRotationAngle() const;
        glm::vec3 getRotationAxis() const;

    private:
        glm::vec3 m_position;
        glm::vec3 m_dimensions;      
        float m_scale;       
        float m_rotationAngle;
        glm::vec3 m_rotationAxis;
        MyBase::FlyWeightCore* create(const std::string& src) const override;
    };
}
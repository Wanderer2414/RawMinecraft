#ifndef CAMERA_H
#define CAMERA_H
#include "Controller3D.h"
#include "Global.h"
#include "Ray.h"
namespace MyBase3D {
    class Form3D;
    #define CAMERA_DISTANCE 4.0f
    class Camera: public Controller3D {
    public:
        Camera();
        ~Camera();

        virtual void    move(const float& x, const float& y, const float& z),
                        rotate(const float& vertical_angle, const float& horizontal_angle),

                        setPosition(const glm::vec3& position),
                        setPosition(const float& x, const float& y, const float& z),

                        setCameraDirection(const glm::vec3& position, const glm::vec3& center),
                        setPerpective(const float& angle, const float& aspect, const float& near, const float& far);

        glm::vec3       getHorizontalVector() const,
                        getCenter() const,
                        getDirection() const,
                        getPosition() const;

        Ray3f           getSight() const;
        operator GLuint();
        friend class    Form3D;
    protected:
        virtual         handle_function(handle) override;
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
        virtual void    update() override;
    private:
        sf::Vector2i    __windowCenter;
        glm::vec3       __position, __delta;
        double          __verticalAngle;
        sf::VertexArray __direction;
        glm::mat4       __view, 
                        __projection, 
                        __clipPlane;

        GLuint          __camera;
        sf::Vector2f    transfer(const glm::vec3& vector) const;
    };
};
#endif
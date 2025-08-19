#include "Player/ModelController.h"
#include "Player/InventoryModule.h"
#include "Camera.h"
#include "ControlCenter.h"
#include "HealthBar.h"
#include "HealthModule.h"
#include "Item.h"
#include "Message.h"
#include "Global.h"
#include "ModelController.h"
#include "Player/Model.h"
#include "Sun.h"
#include "WorldRender.h"

namespace MyCraft {
    namespace Player {
        ModelController::ModelController(): __direction(0, -1, 0), __isChanged(false),
            __eye_direction(0, -1, 0),
            __isDrawable(true), HealthModule(100) {
            __speed = 0.2;
            __speedControl.setDuration(30);


            add(new MoveCommand(this));
            add(new FallCommand(this));
            add(new StopFallCommand(this));
            add(new JumpCommand(this));
            add (new ResetCameraCommand(this));
            add(new PrepareOpenInventoryCommand(this));
            add(new ReceiveItemCommand(this));
            add(new HoldItemCommand(this));
            add(new DiveCommand(*this));
            add(new OnGroundCommand(*this));
            add(new DamageCommand(*this));
        }
        ModelController::~ModelController() {}
        bool ModelController::catchEvent(GLFWwindow* window) {
            __isChanged = MyCraft::ModelController::catchEvent(window) || __isChanged;

            glm::vec2 delta = MyBase::ControlCenter::getInstance().getCursorPos(window);
        
            if (delta.x != 0 || delta.y != 0) {
                MyBase::ControlCenter::CenteringMouse(window);
                seeRotate(-delta.x, delta.y);
                __isChanged = true;
            }
            
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                leftAttack();
                __isChanged = true;
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
                rightAttack();
                __isChanged = true;
            }
            return __isChanged;
        }
        bool ModelController::__moveManage(GLFWwindow* window) {
            if (__speedControl.get()) {
                __speedControl.restart();

                glm::vec3 dir(0);
                if (glfwGetKey(window, GLFW_KEY_A)) dir.y -= 1;
                if (glfwGetKey(window, GLFW_KEY_D)) dir.y += 1;
                if (glfwGetKey(window, GLFW_KEY_W)) dir.x += 1;
                else __speed = 0.2;
                if (glfwGetKey(window, GLFW_KEY_S)) dir.x -= 1;
                
                if (glm::length(dir)) {
                    dir = __toAbsoluteCoordinate(dir);
                    if (isSwim() && dir.z) send(new RequestJumpMessage(getShape(), __speed*(dir.z)/abs(dir.z)/2));
                    else dir.z = 0;
                    move(dir);
                }

                if (glfwGetKey(window, GLFW_KEY_SPACE)) {
                    //Jump here
                    send(new RequestJumpMessage(getShape(), 0.35));
                }
                //Auto fall
                send(new RequestFallMessage(getShape(), getZVelocity()));

            }
            return __isChanged;
        }
        
        bool ModelController::handle(GLFWwindow* window) {
            __isChanged = MyCraft::ModelController::handle(window) || __isChanged;

            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                if (!isCrouch()) {
                    __speed = 0.05;
                    __isChanged = true;
                    Player::Model::crouch();
                    send(new MyBase::SetCameraMessage(getEyePosition(), __eye_direction));
                }
            }
            else if (isCrouch()) {
                __speed = 0.2;
                __isChanged = true;
                Player::Model::uncrouch();
                send(new MyBase::SetCameraMessage(getEyePosition(), __eye_direction));
            }

            if (!isCrouch() && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
                __speed = 0.4;
            }

            __isChanged = __moveManage(window) || __isChanged;
            __isChanged = Player::Model::reset() || __isChanged;
            __isChanged = Player::Model::apply() || __isChanged;
            return __isChanged;
        }

        void ModelController::see(const glm::vec3& dir) {
            __see(dir);
        }
        void ModelController::look(const glm::vec3& position) {
            __look(position);
        }
        void ModelController::rotate(const glm::vec3& dir) {
            __rotate(dir);
        }
        void ModelController::move(const glm::vec3& dir) {
            glm::vec3 direction = glm::normalize(dir)*__speed;
            rotate(direction);
            send(new RequestGotoMessage(getShape(), direction));        
        }
        glm::vec3 ModelController::getPosition() const {
            return Player::Model::getPosition();
        }
        glm::vec3 ModelController::getDirection() const {
            return __eye_direction;
        }
        glm::vec3 ModelController::__toAbsoluteCoordinate(const glm::vec3& delta) const {
            glm::vec3 d = delta.x*__eye_direction;
            d += delta.y*glm::normalize(glm::cross(__eye_direction, glm::vec3(0, 0, 1)));
            return d;
        }
        glm::mat4x3 ModelController::getShape() const {
            return Player::Model::getShape();
        }

        void ModelController::__damage() {
            send(new UpdateHealthBarMessage(getHealthPercent()));
        }
        void ModelController::__dead() {
            
        }
        void ModelController::__heal() {
            
        }
        void ModelController::reset() {
            __isChanged = false;
        }
        void ModelController::leftAttack() {
            if (Player::Model::left_attack()) {
                send(new PlaceMessage(getPosition(), __eye_direction, getItemTypeLeftHand(), getItemTypeRightHand()));
                send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
                send(new RequestFallMessage(getShape(), getZVelocity()));
            } 
        }
        void ModelController::rightAttack() {
            if (Player::Model::attack()) {
                send(new AttackMessage(getPosition(), __eye_direction, getItemTypeLeftHand(),getItemTypeRightHand()));
                send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
                send(new RequestFallMessage(getShape(), getZVelocity()));
            }
        }
        void ModelController::teleport(const glm::vec3& position) {
            setPosition(position);
        }
        void ModelController::__move(const glm::vec3& delta) {
            Player::Model::move(delta); 
            __isChanged = true;
            send(new MyBase::SetCameraMessage(getEyePosition(), __eye_direction));
            send(new CheckHoverMessage(getEyePosition(), __eye_direction));
            send(new FocusMessage(getPosition()));
            send( new WorldMoveMessage(getPosition()));
        }
        void ModelController::__rotate(const glm::vec3& dir) {
            __direction = glm::normalize(dir);
            Player::Model::rotate(__direction);
        }
        void ModelController::__see(const glm::vec3& dir) {
            Player::Model::see(dir);
        }
        void ModelController::__look(const glm::vec3& position) {
            Player::Model::look(position);
        }
        void ModelController::seeRotate(const float& horizontal, const float& vertical) {
            __eye_direction = glm::rotate(__eye_direction, horizontal, glm::vec3(0, 0, 1));
            glm::vec3 axis = glm::cross(__eye_direction, glm::vec3(0,0,1));
            glm::vec3 eye_direction = glm::rotate(__eye_direction, vertical, axis);

            float angle = glm::angle(eye_direction, glm::vec3(0,0,-1));
            if (angle>M_PI/20 && angle < M_PI*0.95) {
                __eye_direction = eye_direction;
                Player::Model::see(__eye_direction);
            }
            send(new MyBase::SetCameraMessage(getEyePosition(), __eye_direction));
            send(new CheckHoverMessage(getEyePosition(), __eye_direction));
        }
        void ModelController::setDrawAble(const bool& drawable) {
            __isDrawable = drawable;
        }
        void ModelController::glDraw() const {
            if (__isDrawable) Player::Model::draw();
        }
        void ModelController::update() {}


        ResetCameraCommand::ResetCameraCommand(MyCraft::Player::ModelController* model): __model(model) {};
        ResetCameraCommand::~ResetCameraCommand() {};
        MyBase::MessageType ResetCameraCommand::getType() const {
            return MyBase::MessageType::ResetCamera;
        }
        void ResetCameraCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
            MyBase::ResetCameraMessage* package = (MyBase::ResetCameraMessage*)message;
            if (package->isFirstCamera) __model->setDrawAble(false);
            else __model->setDrawAble(true);
            __model->send(new MyBase::SetCameraMessage(__model->getEyePosition(), __model->getDirection()));
        }
    
        MoveCommand::MoveCommand(Player::ModelController* model): __model(model) {}
        MoveCommand::~MoveCommand() {}

        MyBase::MessageType MoveCommand::getType() const {
            return MyBase::MessageType::Move;
        }
        void MoveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   {
            MoveMessage* moveMessage = (MoveMessage*)message;
            if (__model->isCrouch()) {
                if (moveMessage->direction.z==0) 
                    __model->__move(moveMessage->direction);
            }
            else {
                __model->__move(moveMessage->direction);
                if (moveMessage->direction.z<0) {
                    auto model = __model->getShape();
                    mine.send(new RequestFallMessage(model, __model->getZVelocity()));
                }
            }
        }

        DiveCommand::DiveCommand(ModelController& model): __model(model) {}
        DiveCommand::~DiveCommand() {}
        MyBase::MessageType DiveCommand::getType() const {
            return MyBase::Dive;
        }
        void DiveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)  {
            __model.swim();
            mine.send(new DiveLightMessage());
        }        
        
        OnGroundCommand::OnGroundCommand(ModelController& model): __model(model) {}
        OnGroundCommand::~OnGroundCommand() {}
        MyBase::MessageType OnGroundCommand::getType() const {
            return MyBase::OnGround;
        }
        void OnGroundCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)  {
            __model.walk();
            mine.send(new OnGroundLightMessage());
        }
    }
}
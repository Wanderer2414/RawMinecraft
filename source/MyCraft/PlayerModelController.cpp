#include "PlayerModelController.h"
#include "Camera.h"
#include "ControlCenter.h"
#include "HealthBar.h"
#include "HealthModule.h"
#include "Item.h"
#include "Message.h"
#include "Global.h"
#include "ModelController.h"
#include "PlayerInventoryModule.h"
#include "PlayerModel.h"
#include "Sun.h"
#include "WorldRender.h"

namespace MyCraft {
    PlayerModelController::PlayerModelController(): __direction(0, -1, 0), __isChanged(false),
        __isLeftAttack(0), __isRightAttack(0), __eye_direction(0, -1, 0),
        __isCrouch(false), __isDrawable(true), HealthModule(100) {
        __animationClock.setDuration(30);
        __attack__cooldown.setDuration(250);
        __speed = 0.2;
        __diagonal = {0.6, 0.4, 1.9};
        __speedControl.setDuration(30);


        add(new PlayerMoveCommand(this));
        add(new FallCommand(this));
        add(new StopFallCommand(this));
        add(new JumpCommand(this));
        add (new ResetCameraCommand(this));
        add(new PrepareOpenInventoryCommand(this));
        add(new ReceiveItemCommand(this));
        add(new HoldItemCommand(this));
        add(new DiveCommand());
        add(new OnGroundCommand());
        add(new DamageCommand(*this));
}
    PlayerModelController::~PlayerModelController() {}
    bool PlayerModelController::isCrounch() const {
        return __isCrouch;
    }
    bool PlayerModelController::catchEvent(GLFWwindow* window) {
        __isChanged = ModelController::catchEvent(window) || __isChanged;
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
    bool PlayerModelController::__moveManage(GLFWwindow* window) {
        glm::vec3 dir(0);
        if (glfwGetKey(window, GLFW_KEY_A)) {
            dir.y -= __speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            dir.y += __speed;
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            dir.x += __speed;
        } else if (!__isCrouch && __speed!=0.2f) {
            __speed = 0.2;
            __animationClock.setDuration(30);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            dir.x -= __speed;
        }
        if (__speed!=0.3f && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) && !__isCrouch) {
            __speed = 0.3;
            __animationClock.setDuration(10);
        }
        if (glm::length(dir)) {
            dir = __toAbsoluteCoordinate(dir);
            dir.z = 0;
            dir = glm::normalize(dir)*__speed;
            rotate(dir);
            send(new RequestGotoMessage(getShape(), dir));
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            //Jump here
            send(new RequestJumpMessage(getShape(), 0.35));
        }
        //Auto fall
        send(new RequestFallMessage(getShape(), getZVelocity()));
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            if (!__isCrouch) {
                __animationClock.setDuration(40);
                __isCrouch = true;
            }
        } else __isCrouch = false;

        return __isChanged;
    }
    
    bool PlayerModelController::handle(GLFWwindow* window) {
        __isChanged = ModelController::handle(window) || __isChanged;
        if (__speedControl.get()) {
            __speedControl.restart();
            __isChanged = __moveManage(window) || __isChanged;
        }
        if (__animationClock.get()) {
            __animationClock.restart();
            if (__isCrouch) {
                __speed = 0.05;
                __isChanged = true;
            }
            if (__isRightAttack) {
                if (__attack__cooldown.get()) {
                    __isRightAttack = false;
                }
                __isChanged = true;
            }
            if (__isLeftAttack) {
                if (__attack__cooldown.get()) {
                    __isLeftAttack = false;
                }
                __isChanged = true;
            }
        }
        __isChanged = Player::Model::reset() || __isChanged;
        __isChanged = Player::Model::apply() || __isChanged;
        return __isChanged;
    }
    glm::vec3 PlayerModelController::getPosition() const {
        return Player::Model::getPosition();
    }
    glm::vec3 PlayerModelController::getDirection() const {
        return __eye_direction;
    }
    glm::vec3 PlayerModelController::__toAbsoluteCoordinate(const glm::vec3& delta) const {
        glm::vec3 d = delta.x*__eye_direction;
        d += delta.y*glm::normalize(glm::cross(__eye_direction, glm::vec3(0, 0, 1)));
        return d;
    }
    glm::mat4x3 PlayerModelController::getShape() const {
        float angle = glm::angle(__direction, glm::vec3(0, -1, 0));
        if (__direction.x < 0) angle = -angle;

        glm::mat4x3 ans;
        ans[0] = getPosition();
        ans[1] = {__diagonal.x, 0, 0};
        ans[2] = {0, __diagonal.y, 0};
        ans[3] = {0, 0, __diagonal.z};

        ans[1] = glm::rotate(ans[1], angle, glm::vec3(0, 0, 1));
        ans[2] = glm::rotate(ans[2], angle, glm::vec3(0, 0, 1));
        ans[0] -= ans[1]/2.f+ans[2]/2.f;
        if (__isCrouch) {
            ans[0] -= ans[2];
            ans[2] = ans[2]*1.8f;
        } 
        return ans;
    }

    void PlayerModelController::__damage() {
        send(new UpdateHealthBarMessage(getHealthPercent()));
    }
    void PlayerModelController::__dead() {
        
    }
    void PlayerModelController::__heal() {
        
    }
    void PlayerModelController::reset() {
        __isChanged = false;
    }
    void PlayerModelController::leftAttack() {
        if (__attack__cooldown.get()) {
            __attack__cooldown.restart();
            Player::Model::left_attack();
            __isRightAttack = false;
            __isLeftAttack = true;
            send(new PlaceMessage(getPosition(), __eye_direction, getItemTypeLeftHand(), getItemTypeRightHand()));
            send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
            send(new RequestFallMessage(getShape(), getZVelocity()));
        } 
    }
    void PlayerModelController::rightAttack() {
        if (__attack__cooldown.get()) {
            __attack__cooldown.restart();
            Player::Model::attack();
            __isRightAttack = false;
            __isRightAttack = true;
            send(new AttackMessage(getPosition(), __eye_direction, getItemTypeLeftHand(),getItemTypeRightHand()));
            send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
            send(new RequestFallMessage(getShape(), getZVelocity()));
        }
    }
    void PlayerModelController::teleport(const glm::vec3& position) {
        setPosition(position);
    }
    void PlayerModelController::move(const glm::vec3& delta) {
        Player::Model::move(delta); 
        __isChanged = true;
        if (isCrounch()) {
            send(new MyBase::SetCameraMessage(getPosition() + glm::vec3(0,0,1.4), __eye_direction));
            send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.4), __eye_direction));
        }
        else {
            send(new MyBase::SetCameraMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
            send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
        }
        send( new WorldMoveMessage(getPosition()));
    }
    void PlayerModelController::rotate(const glm::vec3& dir) {
        __direction = glm::normalize(dir);
        Player::Model::rotate(__direction);
    }
    void PlayerModelController::rotate(const float& angle) {
        __direction = glm::rotate(__direction, angle, glm::vec3(0,0, 1));
        Player::Model::rotate(__direction);
    }
    void PlayerModelController::see(const glm::vec3& dir) {
        Player::Model::see(dir);
    }
    void PlayerModelController::seeRotate(const float& horizontal, const float& vertical) {
        __eye_direction = glm::rotate(__eye_direction, horizontal, glm::vec3(0, 0, 1));
        glm::vec3 axis = glm::cross(__eye_direction, glm::vec3(0,0,1));
        glm::vec3 eye_direction = glm::rotate(__eye_direction, vertical, axis);

        float angle = glm::angle(eye_direction, glm::vec3(0,0,-1));
        if (angle>M_PI/20 && angle < M_PI*0.95) {
            __eye_direction = eye_direction;
            Player::Model::see(__eye_direction);
        }
        if (isCrounch()) {
            send(new MyBase::SetCameraMessage(getPosition()+glm::vec3(0,0,1.4), __eye_direction));
            send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
        }
        else {
            send(new MyBase::SetCameraMessage(getPosition()+glm::vec3(0,0,1.8), __eye_direction));
            send(new CheckHoverMessage(getPosition() + glm::vec3(0,0,1.8), __eye_direction));
        }
    }
    void PlayerModelController::setDrawAble(const bool& drawable) {
        __isDrawable = drawable;
    }
    void PlayerModelController::glDraw() const {
        if (__isDrawable) Player::Model::draw();
    }
    void PlayerModelController::update() {}

    ResetCameraCommand::ResetCameraCommand(MyCraft::PlayerModelController* model): __model(model) {};
    ResetCameraCommand::~ResetCameraCommand() {};
    MyBase::MessageType ResetCameraCommand::getType() const {
        return MyBase::MessageType::ResetCamera;
    }
    void ResetCameraCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        MyBase::ResetCameraMessage* package = (MyBase::ResetCameraMessage*)message;
        if (package->isFirstCamera) __model->setDrawAble(false);
        else __model->setDrawAble(true);
        if (__model->isCrounch()) __model->send(new MyBase::SetCameraMessage(__model->getPosition() + glm::vec3(0,0,1.4), __model->getDirection()));
        else __model->send(new MyBase::SetCameraMessage(__model->getPosition() + glm::vec3(0,0,1.8), __model->getDirection()));
    }


    MoveMessage::MoveMessage(const glm::vec3& d): direction(d) {}
    MoveMessage::~MoveMessage() {}
    MyBase::MessageType MoveMessage::getType() const {
        return MyBase::MessageType::Move;
    }

    FallMessage::FallMessage(const float& z): zVelocity(z) {}
    FallMessage::~FallMessage() {}
    MyBase::MessageType FallMessage::getType() const  {
        return MyBase::MessageType::Fall;
    }

    MyBase::MessageType StopFallMessage::getType() const {
        return MyBase::MessageType::StopFall;
    }
    StopFallMessage::StopFallMessage() {};
    StopFallMessage::~StopFallMessage() {};

    PlaceMessage::PlaceMessage(const glm::vec3& pos, const glm::vec3& dir, const ItemType& left, const ItemType& right): position(pos), direction(dir), rightItem(right), leftItem(left) {}
    PlaceMessage::~PlaceMessage() {}
    MyBase::MessageType PlaceMessage::getType() const {
        return MyBase::MessageType::Place;
    }
    
    AttackMessage::AttackMessage(const glm::vec3& pos, const glm::vec3& dir, const ItemType& left, const ItemType& right): position(pos), direction(dir), leftItem(left), rightItem(right) {}
    AttackMessage::~AttackMessage() {}
    MyBase::MessageType AttackMessage::getType() const{
        return MyBase::MessageType::Attack;
    }

    PlayerMoveCommand::PlayerMoveCommand(MyCraft::PlayerModelController* model): __model(model) {}
    PlayerMoveCommand::~PlayerMoveCommand() {}

    MyBase::MessageType PlayerMoveCommand::getType() const {
        return MyBase::MessageType::Move;
    }
    void PlayerMoveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   {
        MoveMessage* moveMessage = (MoveMessage*)message;
        if (__model->isCrounch()) {
            if (moveMessage->direction.z==0) 
                __model->move(moveMessage->direction);
        }
        else {
            __model->move(moveMessage->direction);
            if (!__model->isFall()) {
                auto model = __model->getShape();
                mine.send(new RequestFallMessage(model, __model->getZVelocity()));
            }
        }
    }

    DiveMessage::DiveMessage() {}
    DiveMessage::~DiveMessage() {}
    MyBase::MessageType DiveMessage::getType() const {
        return MyBase::Dive;;
    }
    DiveCommand::DiveCommand() {}
    DiveCommand::~DiveCommand() {}
    MyBase::MessageType DiveCommand::getType() const {
        return MyBase::Dive;
    }
    void DiveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)  {
        mine.send(new DiveLightMessage());
    }

    OnGroundMessage::OnGroundMessage() {}
    OnGroundMessage::~OnGroundMessage() {}
    MyBase::MessageType OnGroundMessage::getType() const {
        return MyBase::OnGround;
    }
    
    
    OnGroundCommand::OnGroundCommand() {}
    OnGroundCommand::~OnGroundCommand() {}
    MyBase::MessageType OnGroundCommand::getType() const {
        return MyBase::OnGround;
    }
    void OnGroundCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)  {
        mine.send(new OnGroundLightMessage());
    }
}
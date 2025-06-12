#ifndef Message_H
#define Message_H
#include "Global.h"

namespace MyCraft {
    class Message;
    class Network;
    class World;
    enum MessageType: unsigned char {
        RequestGotoMessage,
        RequestFallMessage,
        MoveMessage,
        FallMessage,
        StopFallMessage,
    };
    class Port {
    public:
        Port(Network* network = 0);
        ~Port();

        virtual void match(Network* network);

        void send(Message* Message);
        void send(Port& port, Message* Message);
        virtual std::vector<MessageType> getTypes() const = 0;
        friend class Network;
    protected:
        virtual void receive(Port& source, Message* Message) = 0;
    private:
        Network* __network;
    };

    class Network {
    public:
        Network();
        ~Network();
        virtual void match(Port* port);
        virtual void receive(Port& source, Message* Message);
        virtual void receive(Port& source, Port& destination, Message* Message);
    private:
        std::map<MessageType, std::vector<Port*>>   __ports;
        void send(Port& source, Port& destination, Message* Message);
    };
    
    class Message {
    public:
        virtual MessageType getType() const = 0;
    private:
    };

    class RequestGoto: public Message {
    public:
        RequestGoto(const glm::mat4x3& rectangleBox, const glm::vec2& direction);
        ~RequestGoto();
        MessageType     getType() const override;
        glm::vec2       direction;
        glm::mat4x3     rectangleBox;
    private:
    };
    class RequestFall: public Message {
    public:
        RequestFall(const glm::mat4x3& rectangleBox, const float& zVelocity);
        ~RequestFall();
        MessageType     getType() const override;
        glm::mat4x3 rectangleBox;
        float zVelocity;
    };
    class Move: public Message {
    public:
        Move(const glm::vec3& direction);
        ~Move();
        MessageType     getType() const override;
        glm::vec3       direction;
    };
    class Fall: public Message {
    public:
        Fall(const float& zVelocity);
        ~Fall();
        MessageType     getType() const override;

        float zVelocity;
    };
    class StopFall: public Message {
    public:
        MessageType     getType() const override;
    };
}
#endif



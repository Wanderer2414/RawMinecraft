#ifndef Message_H
#define Message_H
#include "Global.h"

namespace MyCraft {
    class Message;
    class Network;
    class World;
    class Command;
    #define MessageTypeSize 7
    enum MessageType: unsigned char {
        RequestGotoMessage,
        RequestFallMessage,
        MoveMessage,
        FallMessage,
        StopFallMessage,
        MoveCamera,
        RotateCamera
    };
    class Port {
    public:
        Port(Network* network = 0);
        ~Port();

        virtual void match(Network* network);

        void add(Command* command);
        void erase(const MessageType& type);
        void send(Message* Message);
        void send(Port& port, Message* Message);
        std::vector<MessageType> getTypes() const;
        friend class Network;
    protected:
        virtual void receive(Port& source, Message* Message);
    private:
        std::map<MessageType, Command*> __commands;
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
        std::vector<std::vector<Port*>>             __ports;
        void send(Port& source, Port& destination, Message* Message);
    };
    
    class Message {
    public:
        virtual MessageType getType() const = 0;
    private:
    };
    class Command {
    public:
        virtual MessageType getType()      const = 0;
        virtual void execute(Port& mine, Port& source, Message* message) = 0;
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



#ifndef Message_H
#define Message_H
#include "Global.h"

namespace MyCraft {
    class Message;
    class Network;
    class World;
    class Command;
    #define MessageTypeSize 11
    enum MessageType: unsigned char {
        RequestGoto, RequestFall,
        Move, Fall,  StopFall,
        SetCamera, ResetCamera,
        LeftAttack, RightAttack,
        Placeblock,
        CheckHover
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
        virtual ~Message() = 0;
    private:
    };
    class Command {
    public:
        virtual MessageType getType()      const = 0;
        virtual void execute(Port& mine, Port& source, Message* message) = 0;
        virtual ~Command() = 0;
    };



}
#endif



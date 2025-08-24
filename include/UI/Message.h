#ifndef Message_H
#define Message_H
#include "Global.h"

namespace MyBase {
    class Message;
    class Network;
    class World;
    class Command;
    #define MessageTypeSize 34
    enum MessageType: unsigned char {
        RequestGoto, RequestFall, RequestJump, RequestRotate,
        Move, Fall,  StopFall, Jump, Rotate,
        SetCamera, ResetCamera,
        Attack, Place,
        AcceptPlace, AcceptDestroy,
        CheckHover,
        WorldMove, Teleport,
        PrepareOpenInventory, OpenInventoryBlock,
        DropItem, ReceiveItem, AddItem,
        HoldItem,
        DiveView, Dive, OngroundView, OnGround,
        Damage, Health, UpdateHealth,
        Focus, CreatePatrolPath, CreateFollowPath,
    };
    class Port {
    public:
        Port(Network* network = 0);
        virtual ~Port();

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
        static void match(Port* port);
        static void unmatch(Port* port);
        virtual void receive(Port& source, Message* Message);
        virtual void receive(Port& source, Port& destination, Message* Message);
        static void close();
    private:
        Network();
        ~Network();
        static Network& get();
        std::vector<std::vector<Port*>>             __ports;
        void send(Port& source, Port& destination, Message* Message);
        static Network* network;
    };
    
    class Message {
    public:
        virtual MessageType getType() const = 0;
        virtual ~Message() = 0;
    private:
    };
    
    class Command {
    public:
        Command();
        virtual MessageType getType()      const = 0;
        virtual void execute(Port& mine, Port& source, Message* message) = 0;
        virtual ~Command() = 0;
    };



}
#endif



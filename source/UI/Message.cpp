#include "Message.h"

namespace MyCraft {
    Port::Port(Network* network): __network(network) {}
    Port::~Port() {
        for (auto& command:__commands) delete command.second;
        __commands.clear();
    }

    void Port::match(Network* network) {
        __network = network;
    }

    void Port::send(Message* Message) {
        __network->receive(*this, Message);
    }
    void Port::send(Port& port, Message* Message) {
        __network->receive(*this, port, Message);
    }
    std::vector<MessageType> Port::getTypes() const {
        std::vector<MessageType> types(__commands.size());
        int i = 0;
        for (const auto& command: __commands) types[i++] = command.first;
        return types;
    }
    void Port::add(Command* command) {
        auto& __command = __commands[command->getType()];
        if (__command) delete __command;
        __command = command;
    }
    void Port::erase(const MessageType& type) {
        if (__commands.find(type)!=__commands.end()) delete __commands[type];
        __commands.erase(type);
    }
    void Port::receive(Port& source, Message* Message) {
        if (__commands.find(Message->getType())!=__commands.end()) {
            __commands[Message->getType()]->execute(*this, source, Message);
        }
        delete Message;
    }


    Network::Network(): __ports(MessageTypeSize) {}
    Network::~Network() {}
    void Network::match(Port* port) {
        port->match(this);
        const auto& types = port->getTypes();
        for (const auto& type: types)
            __ports[type].push_back(port);
    }
    void Network::receive(Port& source, Message* Message) {
        auto& ports = __ports[Message->getType()];
        for (auto& port:ports) send(source, *port, Message);
    }
    void Network::receive(Port& source, Port& destination, Message* Message) {
        destination.receive(source, Message);
    }
    void Network::send(Port& source, Port& destination, Message* Message) {
        destination.receive(source, Message);
    }
    

    RequestGoto::RequestGoto(const glm::mat4x3& p, const glm::vec2& d): rectangleBox(p), direction(d) {

    }
    RequestGoto::~RequestGoto() {

    }

    RequestFall::RequestFall(const glm::mat4x3& rec, const float& z): rectangleBox(rec), zVelocity(z) {
    }
    RequestFall::~RequestFall() {
    }
    MessageType RequestFall::getType() const {
        return MessageType::RequestFallMessage;
    }
    MessageType RequestGoto::getType() const {
        return MessageType::RequestGotoMessage;
    }
    Move::Move(const glm::vec3& d): direction(d) {

    }
    Move::~Move() {

    }
    MessageType Move::getType() const {
        return MessageType::MoveMessage;
    }
    Fall::Fall(const float& z): zVelocity(z) {

    }
    Fall::~Fall() {

    }
    MessageType Fall::getType() const  {
        return MessageType::FallMessage;
    }

    MessageType StopFall::getType() const {
        return MessageType::StopFallMessage;
    }
}



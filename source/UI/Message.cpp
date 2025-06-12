#include "Message.h"

namespace MyCraft {
    Port::Port(Network* network): __network(network) {

    }
    Port::~Port() {

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

    Network::Network() {}
    Network::~Network() {}
    void Network::match(Port* port) {
        port->match(this);
        const auto& types = port->getTypes();
        for (const auto& type: types)
            __ports[type].push_back(port);
    }
    void Network::receive(Port& source, Message* Message) {
        if (__ports.find(Message->getType()) != __ports.end()){
            auto& ports = __ports[Message->getType()];
            for (auto& port:ports) send(source, *port, Message);
        }
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



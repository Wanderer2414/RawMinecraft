#include "Message.h"

namespace MyBase {
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
        delete Message;
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
    }


    Network::Network(): __ports(MessageTypeSize) {}
    Network::~Network() {}
    Network* Network::network = 0;
    Network& Network::get() {
        if (!network) network = new Network();
        return *network;
    }
    void Network::match(Port* port) {
        port->match(&get());
        const auto& types = port->getTypes();
        for (const auto& type: types)
            get().__ports[type].push_back(port);
    }
    void Network::unmatch(Port *port) {
        const auto& types = port->getTypes();
        for (const auto& type: types) {
            for (int i = get().__ports[type].size()-1; i>=0; i--)
                if (get().__ports[type][i] == port) {
                    get().__ports[type].erase(get().__ports[type].begin()+i);
                    break;
                }
        }
    }
    void Network::receive(Port& source, Message* Message) {
        auto& ports = __ports[Message->getType()];
        for (auto& port:ports) receive(source, *port, Message);
        delete Message;
    }
    void Network::receive(Port& source, Port& destination, Message* Message) {
        destination.receive(source, Message);
    }
    void Network::send(Port& source, Port& destination, Message* Message) {
        destination.receive(source, Message);
    }
    void Network::close() {
        if (!network) delete network;
        network = 0;
    }
    Command::Command() {}
    Command::~Command() {};
    Message::~Message() {};
    
}



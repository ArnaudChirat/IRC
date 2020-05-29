#include "Observer.class.hpp"
#include "IRCMessage.class.hpp"
#include "MessageMediator.class.hpp"
#include "IRCServer.class.hpp"

Observer::Observer(void) {}

Observer::~Observer(void) {}

void Observer::subscribe(SocketClient* socket){
    this->_subscribers.insert(socket);
}

void Observer::unsubscribe(SocketClient* socket){
    this->_subscribers.erase(socket);
}

void Observer::notify(Client * client, std::string const & command){
    User * user = NULL;
    std::string notification;
    if ((user = dynamic_cast<User*>(client)))
        notification = IRCMessage(Parameters(*user), command).to_string();
    for (auto it = this->_subscribers.begin(); it != this->_subscribers.end(); ++it){
        IRCServer::_message_mediator->sendReply(notification, *it);
    }
}

// void Observer::notify(Channel * client, std::string const & command){
//     // IRCMessage notification(channel, command); // build IRC Message from client with command
//     // for (auto it = this->_subscribers.begin(); it != this->_subscribers.end(); ++it){
//     //     IRCServer::_message_mediator->sendReply(notification.to_string(), *it);
//     // }
// }
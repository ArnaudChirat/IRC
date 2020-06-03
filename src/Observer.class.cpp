#include "Observer.class.hpp"
#include "IRCMessage.class.hpp"
#include "MessageMediator.class.hpp"
#include "IRCServer.class.hpp"
#include <iostream>
Observer::Observer(void) : _originOfMsg(NULL) {}

Observer::~Observer(void) {}

void Observer::subscribe(SocketClient* socket){
    this->_subscribers.insert(socket);
}

void Observer::unsubscribe(SocketClient* socket){
    this->_subscribers.erase(socket);
}

bool Observer::notify(Client * client, std::string const & command){
    User * user = NULL;
    std::string notification;
    if ((user = dynamic_cast<User*>(client))){
        ServerClient * server = IRCServer::getServerFromUser(user->getName());
        if (!server)
            notification = IRCMessage(Parameters(*user).paramIRCServer(*(IRCServer::getInstance())), command).to_string();
        else
            notification = IRCMessage(Parameters(*user).paramServer(*server), command).to_string();
    }
    for (auto it = this->_subscribers.begin(); it != this->_subscribers.end(); ++it){
        if (*it != this->_originOfMsg->getSocketClient())
            IRCServer::_message_mediator->sendReply(notification, *it);
    }
    return true;
}

// void Observer::notify(Channel * client, std::string const & command){
//     // IRCMessage notification(channel, command); // build IRC Message from client with command
//     // for (auto it = this->_subscribers.begin(); it != this->_subscribers.end(); ++it){
//     //     IRCServer::_message_mediator->sendReply(notification.to_string(), *it);
//     // }
// }

Client * Observer::getOriginOfMsg(void) const{
    return this->_originOfMsg;
}

void Observer::setOriginOfMsg(Client* client){
    this->_originOfMsg = client;
}
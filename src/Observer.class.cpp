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

void    Observer::sendNotification(std::string const & notification) const{
    for (auto it = this->_subscribers.begin(); it != this->_subscribers.end(); ++it){
        if (*it != this->_originOfMsg)
            IRCServer::_message_mediator->sendReply(notification, *it);
    }
}

bool Observer::notify(Client * client, std::string const & command) const {
    if (client){
        User * user = NULL;
        ServerClient * server = NULL;
        std::string notification;
        if ((user = dynamic_cast<User*>(client))){
            server = IRCServer::getServerFromUser(user->getName());
            if (!server)
                notification = IRCMessage(Parameters(*user).paramIRCServer(*(IRCServer::getInstance())), command).to_string();
            else
                notification = IRCMessage(Parameters(*user).paramServer(*server), command).to_string();
        }
        else if ((server = dynamic_cast<ServerClient*>(client))){
            notification = IRCMessage(Parameters(*server), command).to_string();
        }
        this->sendNotification(notification);

    }
    return true;
}

bool Observer::notify(Channel * channel, User * user, std::string const & command) const {
    if (channel){
        if (user){
            std::string notification = IRCMessage(Parameters(*user).paramChannel(*channel), command).to_string();
            this->sendNotification(notification);
        }
    }
    else {
        throw std::logic_error("Massive error in observer notify from Channel");
    }
    return true;
}

SocketClient * Observer::getOriginOfMsg(void) const{
    return this->_originOfMsg;
}

void Observer::setOriginOfMsg(SocketClient* client){
    this->_originOfMsg = client;
}
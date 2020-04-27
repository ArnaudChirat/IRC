#ifndef MESSAGE_OPERATION_HPP
#define MESSAGE_OPERATION_HPP
#include <string>

class MessageStrategyInterface
{
public:
    virtual void execute(std::string &message) const = 0;
};

class ConnectionMessage : public MessageStrategyInterface
{
    void execute(std::string &message) const;
};

class ChannelOperationMessage : public MessageStrategyInterface
{
    void execute(std::string &message) const;
};

class SendingMessage : public MessageStrategyInterface
{
    void execute(std::string &message) const;
};

class ServerQueryMessage : public MessageStrategyInterface
{
    void execute(std::string &message) const;
};

class ServiceQueryMessage : public MessageStrategyInterface
{
    void execute(std::string &message) const;
};

class MiscellaneousMessage : public MessageStrategyInterface
{
    void execute(std::string &message) const;
};

class ContextMessage
{
public:
    ContextMessage(MessageStrategyInterface *strategy);
    ContextMessage &setStratgey(MessageStrategyInterface *strategy);
    void execute(std::string &message);
    void parseMessage(std::string &message);

private:
    MessageStrategyInterface *_strategy;
};

#endif
#pragma once

#include <iterator>

#include "vectornav/Message.h"

class Driver 
{

public:
    using Message =
        vectornav::Message<
            comms::option::app::ReadIterator<const std::uint8_t *>,                                  // Polymorphic read
            comms::option::app::WriteIterator<std::back_insert_iterator<std::vector<std::uint8_t>>>, // Polymorphic write
            comms::option::app::LengthInfoInterface,                                                 // Polymorphic length calculation
            comms::option::app::IdInfoInterface,                                                     // Polymorphic message ID retrieval
            comms::option::app::NameInterface,                                                       // Polymorphic message name retrieval
            comms::option::app::Handler<Driver>                                               // Polymorphic dispatch
            >;

    using vn_msg = vectornav::message::vectornav_msg<Message>;

    void handle(vn_msg &msg);
    void handle(Message &msg);

protected:
    virtual bool startImpl() override final;
    virtual std::size_t processInputImpl(const std::uint8_t *buf, std::size_t bufLen) override final;

private:
    enum CommsStage
    {
        CommsStage_vectornav_msg,
        CommsStage_NumOfValues
    };

    void sendMessage(const Message& msg);
    void doNextStage();
    void sendvectornav_msg();

    using Frame = vectornav::frame::Frame<Message>;
    Frame m_frame;
    CommsStage m_currentStage = CommsStage_NumOfValues;
};

// what I want to do is to test out the writing
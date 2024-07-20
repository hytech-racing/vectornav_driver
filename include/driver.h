#pragma once

#include <iterator>

#include "vectornav/Message.h"
#include "vectornav/frame/Frame.h"
#include "comms/units.h"
#include "comms/process.h"

class Driver
{

public:
    using InMessage =
        vectornav::Message<
            comms::option::app::ReadIterator<const std::uint8_t *>, // Polymorphic read
            comms::option::app::Handler<Driver>                     // Polymorphic dispatch
            >;
    using OutMessage =
        vectornav::Message<
            comms::option::IdInfoInterface,
            comms::option::WriteIterator<std::back_insert_iterator<std::vector<std::uint8_t>>>,
            comms::option::LengthInfoInterface>;
    using vn_msg = vectornav::message::Vectornav_msg<InMessage>;

    void handle(vn_msg &msg);
    void handle(InMessage &msg)
    {
        static_cast<void>(msg); // ignore
    }
    void sendMessage(const OutMessage &msg, const std::function<void(const std::vector<uint8_t> &)> &send_function);

protected:
private:
    using AllInMessages =
        std::tuple<
            vn_msg>;

    using Frame = vectornav::frame::Frame<InMessage, AllInMessages>;
    Frame _msg_frame;
};

// what I want to do is to test out the writing
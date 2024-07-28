#pragma once

#include <iterator>
#include <iostream>
#include "vectornav/Message.h"
#include "vectornav/frame/Frame.h"
#include "comms/units.h"
#include "comms/process.h"
#include <cstdint>
#include <optional>

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
    using AllInMessages =
        std::tuple<
            vn_msg>;

    using Frame = vectornav::frame::Frame<InMessage, AllInMessages>;

    void handle(vn_msg &msg);
    void handle(InMessage &msg)
    {
        std::cout <<"not used"<<std::endl;
        static_cast<void>(msg); // ignore
    }

    bool recv(std::vector<std::uint8_t>& data_in);

    std::optional<Frame> handleRecv(const std::function<std::vector<uint8_t>()>& recv_function);
    void sendMessage(const OutMessage &msg, const std::function<void(const std::vector<uint8_t> &)> &send_function);
    Frame & get_frame() {return _msg_frame;}
protected:
private:
    bool _vn_msg_recvd;
    Frame _msg_frame;
    vn_msg _current_vn_msg;
};

// what I want to do is to test out the writing
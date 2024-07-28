#include "driver.h"
#include <iostream>
#include <bitset>


void printBits(const std::vector<uint8_t>& bytes) {
    for (const auto& byte : bytes) {
        std::bitset<8> bits(byte); // Convert byte to bits
        std::cout << bits << " ";
    }
    std::cout << std::endl;
}

bool Driver::recv(std::vector<std::uint8_t>& data_in)
{
    if (!data_in.empty())
    {
        std::cout << "recvd "<< data_in.size() <<std::endl;

        // printBits(data_in);
        _vn_msg_recvd = false;
        auto consumed = comms::processAllWithDispatch(&data_in[0], data_in.size(), _msg_frame, *this);

        std::cout <<"consumed " << consumed <<std::endl;
        data_in.erase(data_in.begin(), data_in.begin() + consumed);
        return _vn_msg_recvd;
    }
    return false;
}

void Driver::handle(vn_msg &msg)
{
    _vn_msg_recvd = true;
    _current_vn_msg = msg;
    std::cout << "test groups gp out " << msg.field_groups().getBitValue_general_purpose() << std::endl;
}

std::optional<Driver::Frame> Driver::handleRecv(const std::function<std::vector<uint8_t>()> & recv_function)
{
    auto data_in = recv_function();
    if(recv(data_in)){
        // return _current_vn_msg;
    }
    return std::nullopt;
}

void Driver::sendMessage(const OutMessage &msg, const std::function<void(const std::vector<uint8_t> &)> &send_function)
{
    std::vector<uint8_t> out_buf;
    out_buf.reserve(_msg_frame.length(msg));
    auto iter = std::back_inserter(out_buf);

    auto es = _msg_frame.write(msg, iter, out_buf.max_size());
    if (es == comms::ErrorStatus::UpdateRequired)
    {
        auto *updateIter = &out_buf[0];
        es = _msg_frame.update(msg, updateIter, out_buf.size());
        std::cout <<"update required"<<std::endl;
    }

    if (es != comms::ErrorStatus::Success)
    {
        std::cout << "es=" << (int)es << std::endl;
        assert(!"Write operation failed unexpectedly");
        return;
    }
    static_cast<void>(es);
    assert(es == comms::ErrorStatus::Success); // do not expect any error
    send_function(out_buf);
}

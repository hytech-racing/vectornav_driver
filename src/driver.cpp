#include "driver.h"
#include <iostream>


void Driver::handle(vn_msg &msg)
{
    std::cout << "test groups gp out " << msg.field_groups().getBitValue_general_purpose() << std::endl;
}

void Driver::sendMessage(const OutMessage& msg, const std::function<void(const std::vector<uint8_t>&)>& send_function)
{
    std::vector<uint8_t> out_buf;
    out_buf.reserve(_msg_frame.length(msg));
    auto iter = std::back_inserter(out_buf);

    auto es = _msg_frame.write(msg, iter, out_buf.max_size());
    if (es == comms::ErrorStatus::UpdateRequired) {
        auto* updateIter = &out_buf[0];
        es = _msg_frame.update(updateIter, out_buf.size());
    }
    static_cast<void>(es);
    assert(es == comms::ErrorStatus::Success); // do not expect any error
    send_function(out_buf);
}

#include "vectornav/Message.h"
#include "vectornav/message/Vectornav_msg.h"
#include "driver.h"
// #include <boost/interprocess/shared_memory_object.hpp>
// #include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

const int port = 12345;
const std::string server_ip = "127.0.0.1";

Driver vn_driver;

int main() {
    try {
        boost::asio::io_context io_context;
        udp::resolver resolver(io_context);
        udp::resolver::query query(udp::v4(), server_ip, std::to_string(port));
        udp::endpoint receiver_endpoint = *resolver.resolve(query).begin();

        udp::socket socket(io_context);
        socket.open(udp::v4());

        auto send_lambda = [&socket, &receiver_endpoint](const std::vector<uint8_t>& buf) {
            // Send data
            socket.send_to(boost::asio::buffer(buf), receiver_endpoint);

            // Receive response
        };

        using send_msg = vectornav::message::Vectornav_msg<Driver::OutMessage>;
        send_msg msg_to_send;

        auto& test_groups = msg_to_send.field_groups();
        // using test_groups = typename std::decay<decltype(msg_to_send)>::type;
        test_groups.setBitValue_general_purpose(true);

        vn_driver.sendMessage(msg_to_send, send_lambda);
        // vn_driver.sendMessage()
        std::cout << "message sent!" <<std::endl;


    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

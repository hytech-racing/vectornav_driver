#include "vectornav/Message.h"
#include "vectornav/message/Vectornav_msg.h"
#include "driver.h"
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
            auto ret = socket.send_to(boost::asio::buffer(buf), receiver_endpoint);
            std::cout << "running lambda, ret: "<< ret <<std::endl;

            // Receive response
        };

        using send_msg = vectornav::message::Vectornav_msg<Driver::OutMessage>;
        send_msg msg_to_send;

        msg_to_send.field_groups().setBitValue_general_purpose(true);
        std::cout <<msg_to_send.field_bg1_cond().doesExist() <<std::endl;
        // test_groups.setBitValue_general_purpose(true);
        
        // auto& test_gf1 = msg_to_send.field_gf1_condition();
        // auto& test_cond = msg_to_send.field_gf1_condition();

        // msg_to_send.field_bg1().field().setBitValue_TimeStartup(true);
        // msg_to_send.
        // msg_to_send..value().setBitValue_TimeStartup(true);
        // msg_to_send.field_timestartup_condition().value().setValue(10000);
        vn_driver.sendMessage(msg_to_send, send_lambda);
        
        std::cout << "message sent!" <<std::endl;


    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}


// #include <boost/asio.hpp>
// #include <iostream>

// using boost::asio::ip::udp;

// const int port = 12345;
// const std::string server_ip = "127.0.0.1";

// int main() {
//     try {
//         boost::asio::io_context io_context;
//         udp::resolver resolver(io_context);
//         udp::resolver::query query(udp::v4(), server_ip, std::to_string(port));
//         udp::endpoint receiver_endpoint = *resolver.resolve(query).begin();

//         udp::socket socket(io_context);
//         socket.open(udp::v4());

//         std::string message = "Hello from client";
//         socket.send_to(boost::asio::buffer(message), receiver_endpoint);

//         char reply[1024];
//         udp::endpoint sender_endpoint;
//         size_t reply_length = socket.receive_from(boost::asio::buffer(reply), sender_endpoint);

//         std::cout << "Reply from server: " << std::string(reply, reply_length) << std::endl;
//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }

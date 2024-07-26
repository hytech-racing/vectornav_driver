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
        // msg_to_send.Field_groups().setBitValue_general_purpose(true);
        // msg_to_send.field_bg1_cond().field().setBitValue_AngularRate(true);
        // msg_to_send.field_bg1_cond().field().setBitValue_TimeStartup(true);
        
        // msg_to_send.field_timestartup_condition().field().setValue(10000);
        // msg_to_send.field_angularrate_condition().field().field_ang_rad_s1().setValue(3.0);
        // msg_to_send.field_angularrate_condition().field().field_ang_rad_s2().setValue(3.0);
        // msg_to_send.field_angularrate_condition().field().field_ang_rad_s3().setValue(3.0);
        
        // msg_to_send.doRefresh(); // required to handle the updating of the state of the message to ensure that the 
        // std::cout << msg_to_send.field_bg1_cond().doesExist() <<std::endl;
        // std::cout << msg_to_send.field_timestartup_condition().doesExist() <<std::endl;
        vn_driver.sendMessage(msg_to_send, send_lambda);
        std::cout << "message sent!" <<std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

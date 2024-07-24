#include "vectornav/Message.h"
#include "driver.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/array.hpp>


using boost::asio::ip::udp;

const int port = 12345;
const std::string server_ip = "127.0.0.1";

Driver driver_impl;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a UDP socket and bind it to the port.
        udp::socket socket(io_context, udp::endpoint(udp::v4(), port));
        std::cout << "Server is listening on port " << port << std::endl;

        for (;;) {
        
            udp::endpoint client_endpoint;
            
            boost::array<std::uint8_t, 512> m_inputBuf;
            std::vector<std::uint8_t> m_inData;

            // Receive data from the client.
            size_t length = socket.receive_from(boost::asio::buffer(m_inputBuf), client_endpoint);
            // std::cout << "Received: " << std::string(data, length) << std::endl;
            auto dataBegIter = m_inputBuf.begin();
            auto dataEndIter = dataBegIter + length;
            m_inData.insert(m_inData.end(), dataBegIter, dataEndIter);

            driver_impl.recv(m_inData);
            // Send a response back to the client.
            std::string response = "Message received";
            // socket.send_to(boost::asio::buffer(response), client_endpoint);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

// #include <boost/asio.hpp>
// #include <iostream>

// using boost::asio::ip::udp;

// const int port = 12345;

// int main() {
//     try {
//         boost::asio::io_context io_context;

//         // Create a UDP socket and bind it to the port.
//         udp::socket socket(io_context, udp::endpoint(udp::v4(), port));
//         std::cout << "Server is listening on port " << port << std::endl;

//         for (;;) {
//             char data[1024];
//             udp::endpoint client_endpoint;

//             // Receive data from the client.
//             size_t length = socket.receive_from(boost::asio::buffer(data), client_endpoint);
//             std::cout << "Received: " << std::string(data, length) << std::endl;

//             // Send a response back to the client.
//             std::string response = "Message received";
//             socket.send_to(boost::asio::buffer(response), client_endpoint);
//         }
//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }

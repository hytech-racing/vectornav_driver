#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "vectornav/Message.h"
#include "driver.h"

Driver driver_impl;

using SerialPort = boost::asio::serial_port;

void readData(SerialPort& m_serial, boost::array<std::uint8_t, 512>& m_inputBuf, std::vector<std::uint8_t>& m_inData);

int main()
{
    boost::asio::io_service io;
    SerialPort m_serial(io);
    boost::system::error_code ec;
    std::string dev = "/dev/ttyUSB0";
    m_serial.open(dev, ec);
    if (ec) {
        std::cerr << "ERROR: Failed to open " << dev << std::endl;
        return 1;
    }

    m_serial.set_option(SerialPort::baud_rate(115200));
    m_serial.set_option(SerialPort::character_size(8));
    m_serial.set_option(SerialPort::parity(SerialPort::parity::none));
    m_serial.set_option(SerialPort::stop_bits(SerialPort::stop_bits::one));
    m_serial.set_option(SerialPort::flow_control(SerialPort::flow_control::none));

    boost::array<std::uint8_t, 512> m_inputBuf;
    std::vector<std::uint8_t> m_inData;

    readData(m_serial, m_inputBuf, m_inData);
    io.run();

    return 0;
}

void readData(SerialPort& m_serial, boost::array<std::uint8_t, 512>& m_inputBuf, std::vector<std::uint8_t>& m_inData)
{
    m_serial.async_read_some(
        boost::asio::buffer(m_inputBuf),
        [&](const boost::system::error_code& ec, std::size_t bytesCount)
        {
            if (ec) {
                if (ec != boost::asio::error::operation_aborted) {
                    std::cerr << "ERROR: " << ec.message() << std::endl;
                }
                return;
            }

            auto dataBegIter = m_inputBuf.begin();
            auto dataEndIter = dataBegIter + bytesCount;
            m_inData.insert(m_inData.end(), dataBegIter, dataEndIter);
            driver_impl.recv(m_inData);

            // Initiate another asynchronous read
            readData(m_serial, m_inputBuf, m_inData);
        }
    );
}

#pragma once

#include <boost/uuid/uuid_io.hpp>

#include "Connection.hpp" 

using namespace IEngine::Net;

// Async Write Wrapper
void Connection::WriteToSocket() {

	if (m_writing) {
		m_moreToWrite = true;
		return;
	}
	m_writing = true;
	m_dbstream.Swap();

	async_write(m_socket, m_dbstream.WriteBuffer(), [this](boost::system::error_code ec, std::size_t n) {
		m_writing = false;
		if (ec) { 
			std::cout << "async_write encountered with error: \n" << ec << std::endl;
		}
		else if (m_moreToWrite) {
			WriteToSocket();
			m_moreToWrite = false;
			return;
		}
	});
}

// Async Read Wrapper
void Connection::ReadFromSocket() {
	async_read_until(m_socket, m_inputStream.ActiveIBuffer(), '\n',
	[this](boost::system::error_code ec, std::size_t) {
		if (ec) {
			m_reading = false;
			std::cout << "async_read_until encountered an error: \n" << ec << std::endl;
		}
		else {
			m_reading = true;
			std::string line;
			std::getline(m_inputStream.ActiveIStream(), line);
			//m_IEgineInterface.HandleLine(line);
			
			// Gross hardcoded echo
			std::stringstream ss;
			ss << m_connectionID << ": " << line << std::endl;
			std::cout << ss.str();
			WriteToBuffer(ss.str());
			ReadFromSocket();
		}
	});
}

void Connection::WriteToBuffer(const std::string& msg) {
	m_dbstream.ActiveStream() << msg;
	WriteToSocket();
}

std::ostream& Connection::operator<<(const std::string& msg) {
	WriteToBuffer(msg);
	m_moreToWrite = true;
	return m_dbstream.ActiveStream();
};

std::ostream& Connection::ostream() {
	WriteToSocket();
	m_moreToWrite = true;
	return m_dbstream.ActiveStream();
}

void Connection::Start() {
	std::cout << "Socket Communication Starting..." << std::endl;
	WriteToBuffer("Connection Established!\n\r");
	ReadFromSocket();
}

bool Connection::Active() {
	if (m_writing || m_reading) {
		return true;
	}
	return false;
}

void Connection::Close() {
	std::cout << "Socket Communication Stopping..." << std::endl;
	// TODO
}

SocketType &Connection::Socket() { return m_socket; };
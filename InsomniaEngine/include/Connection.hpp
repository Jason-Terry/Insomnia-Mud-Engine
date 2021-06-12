#pragma once

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <iostream>

#include "DoubleBufferedStream.hpp"
#include "InputStream.hpp"

namespace IEngine {
	namespace Net {
		typedef boost::asio::ip::tcp::socket SocketType;
		class Connection {
		public:

			explicit Connection(boost::asio::io_service &io_service) : 
				m_socket(io_service),
				m_dbstream(),
				m_inputStream(),
				m_writing(false),
				m_moreToWrite(false),
				m_reading(true),
				m_connectionID(boost::uuids::random_generator()()),
				m_connectionIDStr()
			{
				std::cout << "New Connection Created." << std::endl;
			};

			void WriteToBuffer(const std::string& msg);

			std::ostream& operator<<(const std::string& msg);
			std::ostream& ostream();

			void Start();
			bool Active();
			void Close();

			SocketType &Socket();

		private:
			void WriteToSocket();
			void ReadFromSocket();
			SocketType m_socket;
			DoubleBufferStream m_dbstream;
			InputStream m_inputStream;
			boost::uuids::uuid m_connectionID;
			std::string m_connectionIDStr;
			bool m_writing;
			bool m_reading;
			bool m_moreToWrite;
		};
	}
}
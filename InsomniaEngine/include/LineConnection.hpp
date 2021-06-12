#pragma once


#include "Connection.hpp";

namespace IEngine {
	namespace Net {
		class LineOrientedConnection : public Connection {
		public: 
			LineOrientedConnection(boost::asio::io_service& io_service) :
				Connection(io_service),
				m_inputStream(&m_inputBuffer) {
			}

		private:
			void ReadLine();
			boost::asio::streambuf m_inputBuffer;
			std::istream m_inputStream;
		};
	}
}
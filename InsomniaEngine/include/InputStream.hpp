#pragma once

#include <boost/asio.hpp>

namespace IEngine {
	namespace Net {
		class InputStream {

		public:
			InputStream() :
				m_inputStream(&m_inputBuffer) {

			}

			boost::asio::streambuf& ActiveIBuffer() {
				return m_inputBuffer;
			}

			std::istream& ActiveIStream() {
				return m_inputStream;
			}


		private:
			boost::asio::streambuf m_inputBuffer;
			std::istream m_inputStream;
		};
	}
}
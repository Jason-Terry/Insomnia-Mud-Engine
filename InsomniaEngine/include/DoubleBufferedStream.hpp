#pragma once

#include <boost/asio.hpp>

namespace IEngine {
	namespace Net {
		class DoubleBufferStream {

		public:
			DoubleBufferStream() :
				m_outputStream1(&m_outputBuffer1),
				m_outputStream2(&m_outputBuffer2),
				m_activeStream(&m_outputStream1),
				m_activeBuffer(&m_outputBuffer1),
				m_writeStream(&m_outputStream2),
				m_writebuffer(&m_outputBuffer2) {

			}

			boost::asio::streambuf& ActiveBuffer() {
				return *m_activeBuffer;
			}

			boost::asio::streambuf& WriteBuffer() {
				return *m_writebuffer;
			}

			std::ostream& ActiveStream() {
				return *m_activeStream;
			}

			std::ostream& WriteStream() {
				return *m_writeStream;
			}

			void Swap() {
				std::swap(m_activeBuffer, m_writebuffer);
				std::swap(m_activeStream, m_writeStream);
			}

		private:
			boost::asio::streambuf *m_activeBuffer, *m_writebuffer;
			std::ostream *m_activeStream, *m_writeStream;
			boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
			std::ostream m_outputStream1, m_outputStream2;
		};
	}
}
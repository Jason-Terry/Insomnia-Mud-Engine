#pragma once

#include <boost/asio.hpp>
#include <iostream>

#include "Connection.hpp"

namespace IEngine {
	namespace Net {
	
		class Server {
		public:

			Server(int port) : 
				m_run(false),
				m_signal_set(m_io_service, SIGINT, SIGTERM),
				m_acceptor(m_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port)) {
				std::cout << "Creating server instance with port: " << port << std::endl;
				m_signal_set.async_wait([this](boost::system::error_code ec, int signal) {
					std::cout << "Server received signal(" << signal << ")" << " attempting clean shutdown." << std::endl;
					m_run = false;
					m_acceptor.cancel();
				});
			}

			void Run() {
				Accept();
				std::thread thrd_ctx([&]() {m_io_service.run(); }); // Will run until we have no async tasks.
				std::cout << "Server now running." << std::endl;
				m_run = true;
				while (m_run) {
					// wait
				}
			}

		protected:
			void CleanConnections() {
				const size_t old_size = m_connections.size();
				std::erase_if(m_connections, [](auto& c) { return !c.Active(); });
				std::cout << "Connections Removed: " << old_size - m_connections.size() << std::endl;
				std::cout << "Current Connection Count: " << m_connections.size() << std::endl;
			}

		private:

			void Accept() {
				m_connections.emplace_back(m_io_service);
				auto &connection = m_connections.back();
				std::cout << "Ready to accept a incoming connection..." << std::endl;
				m_acceptor.async_accept(connection.Socket(), [this, &connection](boost::system::error_code ec) {
					if (!ec) {
						std::cout << "Connection established." << std::endl;
						connection.Start();
						connection.WriteToBuffer("Welcome\n\r");
						this->CleanConnections(); // TODO -- Would like connections to self manage
						Accept();
					}
				});
			}

			boost::asio::io_service m_io_service;
			boost::asio::signal_set m_signal_set;
			boost::asio::ip::tcp::acceptor m_acceptor;
			bool m_run;

			std::list<Connection> m_connections;
		};
	}
}
#ifndef Robot_Network_TcpClient
#define Robot_Network_TcpClient

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/**
 * Design goal: Hide boost::asio code entirely.
 * 
 * Modeled after the Boost::Asio chat client example code: 
 * http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/example/chat/
 * chat_client.cpp
 */
namespace Network {
class TcpClient
{
	public:
		/**
		 * CTOR. 
		 * Searches for the port supplying the service and connects. 
		 * Optionally supply the host of the robot server. 
		 */
		TcpClient(std::string host = "localhost");

		/**
		 * DTOR. Close connection.
		 */
		~TcpClient();

		/**
		 * Write a message to the server. 
		 * Probably not thread-safe. (TODO: add mutexes?)
		 */
		void write(std::string msg);

		/**
		 * Read the robot status from the server. 
		 * Probably not thread-safe. (TODO: add mutexes?)
		 * TODO: Return status object? (motor states, blink states, etc?)
		 * 		An advanced client might request state & image frame(s) together
		 */ 
		std::string read();

		// TODO: Can this be restarted? 
		void close();

	private:

		/**
		 * The TCP Socket. 
		 */
		tcp::socket socket;

};
}

#endif

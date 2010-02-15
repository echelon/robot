#include "TcpClient.hpp"

#include <boost/bind.hpp>

namespace Network {

// XXX: Much of the code is adapted from Boost::Asio tutorial code
TcpClient::TcpClient(std::string host)
{
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host.c_str(), "robotserv");

	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

	// create & connect
	tcp::socket socket(io_service);
	boost::system::error_code error = boost::asio::error::host_not_found;
	while(error && endpoint_iterator != end) {
		socket.close();
		socket.connect(*endpoint_iterator++, error);
    }

    if(error) {
		throw boost::system::system_error(error);
	}


	// XXX: VERSUS...

	socket_.async_connect(endpoint,
		boost::bind(&TcpClient::handleConnect, this,
				boost::asio::placeholders::error, 
				++endpoint_iterator)
	);




}



}

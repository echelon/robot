#include <stdio.h>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>


#include "controller/KeyboardThread.hpp"

// THIS IS FROM THE BOOST ASIO TUTORIAL

using boost::asio::ip::tcp;

int main(int argc, char** argv)
{
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(argv[1], "daytime");

	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

	// create & connect
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
		throw boost::system::system_error(error);

	printf("Connected..\n");

	// Read data into buffer
	for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

  		if (error == boost::asio::error::eof)
      	  break; // Connection closed cleanly by peer.
      	else if (error)
      	  throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }

	// Write data to the socket if there's anything to write
	while(1) {
		boost::system::error_code error;

		size_t len = socket.write_some(buf);
	}


	printf("Done\n");
	return 0;
}

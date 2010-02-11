#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <stdio.h>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}


int main()
{

    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));


for (;;)
    {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

	printf("Make message...\n");

  std::string message = make_daytime_string();

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message),
          boost::asio::transfer_all(), ignored_error);

	printf("Done transfer...\n");

    }



	return 0;
}

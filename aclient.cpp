//g++ aclient.cpp -o aclient -pthread

#include <iostream>
#include <iomanip>
#include <thread>

#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>

using namespace asio; using namespace asio::ip;

char readbuf[128] {"CCC"};

struct r_handler
{	r_handler* rh;
	tcp::socket* sct;
	
	void operator()( const error_code& error, std::size_t tbytes )
	{
	 	if ( !error ) { 

		readbuf[tbytes] = 0;
		std::cout << readbuf << std::endl;

		sct->async_read_some( asio::buffer( readbuf, 128 ), *rh );

		}

		else std::cout << error.message() << std::endl;

	}
} rh0;

int main()
{	

    tcp::endpoint endpoint { address::from_string( "127.0.0.1" ), 31313 };
    io_context iOv;
    tcp::socket socket0 { iOv };
    error_code ec;

	bool msg {false};
	
try_:	
	socket0.connect( endpoint, ec ); 

	if ( ec ) 
	{ if ( !msg ) { std::cout << "Server offline <-" << "\n"; msg = true; }
	  sleep(1); goto try_; }

	std::cout << "Connection established. Welcome to chat ->" << "\n\n";

	//r_handler* rh = new r_handler;
	rh0.rh = &rh0; rh0.sct = &socket0;

	socket0.async_read_some( asio::buffer( readbuf, 128 ), rh0 );

	do { sleep(1); 

	     iOv.poll(); }

	while ( true );

	socket0.write_some( asio::buffer( "SC0", 3 ));

    	socket0.close(); //delete rh;

	std::cout << "\nDisconnected <-\n";

}


#include "HTTPUtils.h"

#include "Utils/AvroUtils.h"
#include "Utils/GPUdbExceptions.h"

#include <ostream>
#include <sstream>

#include <boost/bind.hpp>


namespace gpudb
{


// ================== Helper Class 'client' Member Functions ===================


client::client( boost::asio::io_service& io_service,
                const std::string& ipaddr, const std::string& port,
                const std::string& endpoint, const std::string& json_data )
    : resolver_ (io_service),
      socket_   (io_service),
      is_json ( true ),
      is_binary ( false )
{
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream( &request_ );
    request_stream << "POST " << endpoint << " HTTP/1.0\r\n"
                   << "Host: " << ipaddr << ":" << port << "\r\n"
                   << "Content-Type: application/json; charset=UTF-8\r\n"
                   << "Content-Length: " << json_data.length() << "\r\n"
                   << "Accept: application/json\r\n"
        // << "Authorization: Basic "
                   << "Connection: close\r\n\r\n"
                   << json_data << "\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    boost::asio::ip::tcp::resolver::query query( ipaddr, port );
    resolver_.async_resolve(query,
                            boost::bind(&client::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::iterator));
}  // end client constructor with json data



client::client( boost::asio::io_service& io_service,
                const std::string& ipaddr, const std::string& port,
                const std::string& endpoint, const std::vector<uint8_t>& binary_data )
    : resolver_ (io_service),
      socket_   (io_service),
      is_json ( false ),
      is_binary ( true )
{
    size_t data_size = binary_data.size();

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream( &request_ );
    request_stream << "POST " << endpoint << " HTTP/1.0\r\n"
                   << "Host: " << ipaddr << ":" << port << "\r\n"
                   << "Content-Type: application/octet-stream\r\n"
                   << "Content-Length: " << data_size << "\r\n"
        // << "Authorization: Basic "
                   << "Connection: close\r\n\r\n";
    // Write the binary data one character at a time to the payload
    for ( size_t i = 0; i < data_size; ++i )
        request_stream << binary_data[ i ];
    request_stream << "\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    boost::asio::ip::tcp::resolver::query query( ipaddr, port );
    resolver_.async_resolve( query,
                             boost::bind( &client::handle_resolve, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::iterator ) );
}  // end client constructor with binary data


void client::handle_resolve( const boost::system::error_code& err,
                             boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        // Attempt a connection to each endpoint in the list until we
        // successfully establish a connection.
        boost::asio::async_connect( socket_, endpoint_iterator,
                                    boost::bind( &client::handle_connect, this,
                                                 boost::asio::placeholders::error ) );
    }
    else
    {
        throw gpudb::NetworkException( err.message() );
    }
}  // end handle_resolve



void client::handle_connect(const boost::system::error_code& err)
{
    if (!err)
    {
        // The connection was successful. Send the request.
        boost::asio::async_write( socket_, request_,
                                  boost::bind( &client::handle_write_request, this,
                                               boost::asio::placeholders::error) );
    }
    else
    {
        throw gpudb::NetworkException( err.message() );
    }
}  // end handle_connect



void client::handle_write_request(const boost::system::error_code& err)
{
    if (!err)
    {
        // Read the response status line. The response_ streambuf will
        // automatically grow to accommodate the entire line. The growth may be
        // limited by passing a maximum size to the streambuf constructor.
        boost::asio::async_read_until( socket_, response_, "\r\n",
                                       boost::bind( &client::handle_read_status_line, this,
                                                    boost::asio::placeholders::error ) );
    }
    else
    {
        throw gpudb::NetworkException( err.message() );
    }
}  // end handle_write_request



void client::handle_read_status_line(const boost::system::error_code& err)
{
    if (!err)
    {
        // Check that response is OK.
        std::istream response_stream( &response_ );

        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            throw gpudb::NetworkException( "Invalid response\n" );
        }
        // Don't worry about bad status codes; gpudb response will have the error
        // message will be handled elsewhere (in GPUdb.cpp)

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until( socket_, response_, "\r\n\r\n",
                                       boost::bind( &client::handle_read_headers, this,
                                                    boost::asio::placeholders::error ) );
    }
    else
    {
        throw gpudb::NetworkException(  err.message() );
    }
}  // end handle_read_status_line



void client::handle_read_headers(const boost::system::error_code& err)
{
    if (!err)
    {
        // Process the response headers.
        std::istream response_stream(&response_);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r");

        // Start reading remaining data until EOF.
        boost::asio::async_read( socket_, response_,
                                 boost::asio::transfer_at_least(1),
                                 boost::bind( &client::handle_read_content, this,
                                              boost::asio::placeholders::error ) );
    }
    else
    {
        throw gpudb::NetworkException(  err.message() );
    }
}  // end handle_read_headers



void client::handle_read_content(const boost::system::error_code& err)
{
    if (!err)
    {
        // Continue reading remaining data until EOF.
        boost::asio::async_read( socket_, response_,
                                 boost::asio::transfer_at_least(1),
                                 boost::bind( &client::handle_read_content, this,
                                              boost::asio::placeholders::error ) );
    }
    else if (err != boost::asio::error::eof)
    {
        throw gpudb::NetworkException( err.message() );
    }
}  // end handle_read_content




// Retrieve the json response; if query was binary, return false
bool client::get_response( std::string& response )
{
    if ( is_binary )
        return false;

    // Convert the streambuf to std::string
    boost::asio::streambuf::const_buffers_type buf = response_.data();
    std::string str( boost::asio::buffers_begin( buf ),
                     boost::asio::buffers_begin( buf ) + response_.size() );

    response = str;
    return true;
}  // end get_response json



// Retrieve the binary response; if query was json, return false
bool client::get_response( std::vector<uint8_t>& response )
{
    if ( is_json )
        return false;

    // Convert the streambuf to an istream
    boost::asio::streambuf::const_buffers_type buf = response_.data();
    std::istream is( &response_ );

    // Copy the data from istream to the vector
    response.assign( std::istreambuf_iterator<char>( is ),
                     std::istreambuf_iterator<char>() );
    return true;
}  // end get_response binary



// ========================= HTTPUtils Member Functions =======================




// Make an HTTP call to GPUdb at gpudb_ip::gpudb_port with json encoding
// static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::string& json_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_ip,
                                             const std::string& gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    try
    {
        // Create the boost asynchronous network IO service
        boost::asio::io_service io_service;

        // The constructor takes care of the whole network messaging
        // via handler/callback functions
        client c( io_service, gpudb_ip, gpudb_port, endpoint, json_data );

        // Need to actually run the service to initiate the call
        io_service.run();

        // Retrieve the message
        std::string json_response;
        c.get_response( json_response );

        // Convert the GPUdb response to an object
        gpudb::gpudb_response gresponse;
        if ( gpudb::AvroUtils::convert_to_object( json_response, gresponse ) == false )
            throw gpudb::QueryException( "Error: Unable to parse GPUdb response!\n" );

        return gresponse;
    }
    catch (const std::exception& e)
    {  // had a problem
        throw;
    }

    return gpudb::gpudb_response(); // shouldn't get here when throwing
}  // end call_gpudb with IP address and port for JSON data




// Make an HTTP call to GPUdb at gpudb_ip::gpudb_port with binary encoding
//static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& avro_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_ip,
                                             const std::string&  gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    try
    {
        // Create the boost asynchronous network IO service
        boost::asio::io_service io_service;

        // The constructor takes care of the whole network messaging
        // via handler/callback functions
        client c( io_service, gpudb_ip, gpudb_port, endpoint, avro_data );

        // Need to actually run the service to initiate the call
        io_service.run();

        // Retrieve the message
        std::vector<uint8_t> binary_response;
        c.get_response( binary_response );

        // Convert the GPUdb response to an object
        gpudb::gpudb_response gresponse;
        if ( gpudb::AvroUtils::convert_to_object( binary_response, gresponse ) == false )
            throw gpudb::QueryException( "Unable to parse GPUdb response!\n" );

        return gresponse;
    }
    catch (const std::exception& e)
    {  // had a problem
        throw;
    }

    return gpudb::gpudb_response(); // shouldn't get here when throwing
}  // end call_gpudb with IP address and port for AVRO encoded data



//  ------------------------ Convenience wrappers ---------------------------


// Make an HTTP call to GPUdb at 127.0.0.1::9191 with binary encoding
// with derived endpoint
//static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& avro_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    return HTTPUtils::call_gpudb( avro_data, get_endpoint( avro_type ), "9191",
                                  username, password, timeout_secs);
}

// Defaults to a local GPUdb at 127.0.0.1 with binary encoding and derived endpoint
//static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& avro_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    return HTTPUtils::call_gpudb( avro_data, get_endpoint( avro_type ), gpudb_port,
                                  username, password, timeout_secs);
}


// Defaults to a local GPUdb at 127.0.0.1 with the given endpoint with binary encoding 
//static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& avro_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    return HTTPUtils::call_gpudb( avro_data, endpoint, "127.0.0.1", gpudb_port,
                                  username, password, timeout_secs);
}









} // end namespace gpudb



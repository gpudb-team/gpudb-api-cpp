
#include "HTTPUtils.h"

#include "Utils/AvroUtils.h"
#include "Utils/GPUdbExceptions.h"

#include <ostream>
#include <sstream>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>



namespace gpudb
{

// ========================= HTTPUtils Member Functions =======================


// Protected:
// ----------

/// Binary version of making a call to GPUdb using Poco::Net
// static
void HTTPUtils::poco_query( const std::string& ipaddr, const std::string& port,
                            const std::string& endpoint,
                            const std::string& req_json_data,
                            std::string &output, int timeout_secs )
{
    try
    {
        // Convert the port to a number
        std::istringstream port_iss( port );
        unsigned short port_num;
        port_iss >> port_num;

        // Create the client session
        Poco::Net::HTTPClientSession s( ipaddr, port_num );
        s.setTimeout( Poco::Timespan( timeout_secs, 0 ) );
        // Create the request packet
        Poco::Net::HTTPRequest http_request(Poco::Net::HTTPRequest::HTTP_POST, endpoint);
        // http_request.setContentType("application/octet-stream"); // binary
        http_request.setContentType("application/json"); // json

        // Write to stream (send the packet)
        std::ostream& os = s.sendRequest( http_request );
        os.write( (const char*)&req_json_data[0], req_json_data.size() );

        // Receive the response
        Poco::Net::HTTPResponse response;
        std::istream& rs = s.receiveResponse( response );

        //convert istream to string
        output = "";
        output.assign( std::istreambuf_iterator<char>(rs),
                       std::istreambuf_iterator<char>() );
    }
    catch (const std::exception& e)
    {
        throw gpudb::NetworkException( e.what() );
    }
} // end poco_query json format



/// Binary version of making a call to GPUdb using Poco::Net
// static
void HTTPUtils::poco_query( const std::string& ipaddr, const std::string& port,
                 const std::string& endpoint,
                 const std::vector<uint8_t>& req_binary_data,
                 std::vector<uint8_t> &output,
                 int timeout_secs )
{
    try
    {
        // Convert the port to a number
        std::istringstream port_iss( port );
        unsigned short port_num;
        port_iss >> port_num;

        // Create the client session
        Poco::Net::HTTPClientSession s( ipaddr, port_num );
        s.setTimeout( Poco::Timespan( timeout_secs, 0 ) );
        // Create the request packet
        Poco::Net::HTTPRequest http_request(Poco::Net::HTTPRequest::HTTP_POST, endpoint);
        http_request.setContentType("application/octet-stream"); // binary
        // http_request.setContentType("application/json"); // json

        // Write to stream (send the packet)
        std::ostream& os = s.sendRequest( http_request );
        os.write( (const char*)&req_binary_data[0], req_binary_data.size() );

        // Receive the response
        Poco::Net::HTTPResponse response;
        std::istream& rs = s.receiveResponse( response );

        //convert istream to binary/char vector
        output.assign( std::istreambuf_iterator<char>( rs ),
                       std::istreambuf_iterator<char>() );
    }
    catch (const std::exception& e)
    {
        throw gpudb::NetworkException( e.what() );
    }
} // end poco_query binary format






// Public:
// -------


// Ping GPUdb
std::string HTTPUtils::ping( const std::string& gpudb_ip,
                             const std::string& gpudb_port )
{
    try
    {
        // // Create the boost asynchronous network IO service
        // boost::asio::io_service io_service;

        // // The constructor takes care of the whole network messaging
        // // via handler/callback functions
        // client c( io_service, gpudb_ip, gpudb_port );

        // // Need to actually run the service to initiate the call
        // io_service.run();

        // // Retrieve the message
        // std::string json_response;
        // c.get_response( json_response );

        // Make the ping endpoint call
        std::string ping_endpoint = "/";
        std::string ping_payload = "";
        std::string ping_response;
        poco_query( gpudb_ip, gpudb_port, ping_endpoint, ping_payload,
                    ping_response );


        return ping_response; // Return the ping response
    }
    catch (const std::exception& e)
    {  // had a problem
        throw;
    }

    return ""; // shouldn't get here when throwing
}  // end ping


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
        // Make the call and retrieve the response
        std::string json_response;
        poco_query( gpudb_ip, gpudb_port, endpoint, json_data, json_response );
        // std::cout << "json response: " << json_response << std::endl;


        // Convert the GPUdb response to an object
        gpudb::gpudb_response gresponse;
        if ( gpudb::AvroUtils::convert_to_object( json_response, gresponse ) == false )
        {
            std::cout << "avro utils conversion ERROR encountered; response size: " << json_response.size() << std::endl; // debug ------
            std::cout << "last 100 bytes: " << json_response.substr( json_response.size() - 100, 100 ) << std::endl; // debug ------------------
            throw gpudb::QueryException( "Error: Unable to parse GPUdb response!\n" );
        }

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
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& binary_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_ip,
                                             const std::string& gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    try
    {
        // Make the call and retrieve the response
        std::vector<uint8_t> binary_response;
        poco_query( gpudb_ip, gpudb_port, endpoint, binary_data, binary_response );

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
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& binary_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    return HTTPUtils::call_gpudb( binary_data, get_endpoint( avro_type ), "9191",
                                  username, password, timeout_secs);
}

// Defaults to a local GPUdb at 127.0.0.1 with binary encoding and derived endpoint
//static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& binary_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    return HTTPUtils::call_gpudb( binary_data, get_endpoint( avro_type ), gpudb_port,
                                  username, password, timeout_secs);
}


// Defaults to a local GPUdb at 127.0.0.1 with the given endpoint with binary encoding 
//static
gpudb::gpudb_response HTTPUtils::call_gpudb( const std::vector<uint8_t>& binary_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_port,
                                             const std::string& username,
                                             const std::string& password,
                                             int timeout_secs )
{
    return HTTPUtils::call_gpudb( binary_data, endpoint, "127.0.0.1", gpudb_port,
                                  username, password, timeout_secs);
}








} // end namespace gpudb



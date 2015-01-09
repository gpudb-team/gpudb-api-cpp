#ifndef __HTTP_UTILS__
#define __HTTP_UTILS__

#include <string>
#include <vector>
#include "AvroTypes.h"

#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>


#include "obj_defs/gpudbresponse.h"

namespace gpudb
{


// A class to handle the network calls
class client
{
public:
    // Only the constructor is public
    client( boost::asio::io_service& io_service,
            const std::string& ipaddr, const std::string& port,
            const std::string& endpoint,
            const std::string& json_data );

    client( boost::asio::io_service& io_service,
            const std::string& ipaddr, const std::string& port,
            const std::string& endpoint,
            const std::vector<uint8_t>& binary_data  );

    // Retrieve the json response; if query was binary, return false
    bool get_response( std::string& response );

    // Retrieve the binary response; if query was json, return false
    bool get_response( std::vector<uint8_t>& response );

private:
    // Handles the resolver which is called within the constructor
    void handle_resolve( const boost::system::error_code& err,
                         boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    // Handles the connect call called within the resolver handler
    void handle_connect(const boost::system::error_code& err);

    // Handles the write request call called within the connection handler
    void handle_write_request(const boost::system::error_code& err);

    // Handles the read status call called within the write request handler
    void handle_read_status_line(const boost::system::error_code& err);

    // Handles the header reader call called within the status reader handler
    void handle_read_headers(const boost::system::error_code& err);

    // Handles the content reader call called within the header reader handler
    void handle_read_content(const boost::system::error_code& err);

    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
    bool is_json;
    bool is_binary;
};  // end class client


class HTTPUtils
{
    HTTPUtils();

public:
  
    // Make an HTTP call to GPUdb at gpudb_ip::gpudb_port with binary encoding
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& avro_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_ip,
                                             const std::string& gpudb_port,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

    // Make an HTTP call to GPUdb at gpudb_ip::gpudb_port with json encoding
    static gpudb::gpudb_response call_gpudb( const std::string& json_data,
                                             const std::string& endpoint,
                                             const std::string& gpudb_ip,
                                             const std::string& gpudb_port,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

    // Convenience wrappers

    // Make an HTTP call to GPUdb at 127.0.0.1::gpudb_port with binary encoding
    // with the given endpoint
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& avro_data,
                                             const std::string& endpoint,
                                             const std::string&  gpudb_port,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

    // Make an HTTP call to GPUdb at 127.0.0.1::gpudb_port with binary encoding
    // with derived endpoint
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& avro_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& gpudb_port,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

    // Make an HTTP call to GPUdb at 127.0.0.1::9191 with binary encoding
    // with derived endpoint
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& avro_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

}; // end class HTTPUtils


} // end namespace gpudb

#endif // __HTTP_UTILS__

#ifndef __HTTP_UTILS__
#define __HTTP_UTILS__

#include <string>
#include <vector>
#include "AvroTypes.h"

#include "obj_defs/gpudbresponse.h"


namespace gpudb
{



class HTTPUtils
{
    HTTPUtils();

public:
  
    // Ping GPUdb
    static std::string ping( const std::string& gpudb_ip,
                             const std::string& gpudb_port );

    // Make an HTTP call to GPUdb at gpudb_ip::gpudb_port with binary encoding
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& binary_data,
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
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& binary_data,
                                             const std::string& endpoint,
                                             const std::string&  gpudb_port,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

    // Make an HTTP call to GPUdb at 127.0.0.1::gpudb_port with binary encoding
    // with derived endpoint
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& binary_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& gpudb_port,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );

    // Make an HTTP call to GPUdb at 127.0.0.1::9191 with binary encoding
    // with derived endpoint
    static gpudb::gpudb_response call_gpudb( const std::vector<uint8_t>& binary_data,
                                             gpudb::avro_t avro_type,
                                             const std::string& username = "",
                                             const std::string& password = "",
                                             int timeout_secs = 60 );


protected:
    // Make a query to GPUdb using Poco::Net (json formatted data)
    static void poco_query( const std::string& ipaddr, const std::string& port,
                            const std::string& endpoint,
                            const std::string& req_json_data,
                            std::string &output, int timeout_secs = 60 );


    // Make a query to GPUdb using Poco::Net (binary formatted data)
    static void poco_query( const std::string& ipaddr, const std::string& port,
                            const std::string& endpoint,
                            const std::vector<uint8_t>& req_binary_data,
                            std::vector<uint8_t> &output,
                            int timeout_secs = 60 );


}; // end class HTTPUtils


} // end namespace gpudb

#endif // __HTTP_UTILS__

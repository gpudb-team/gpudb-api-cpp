/* ***************************************
 * GPUdb C++ API Example: Register Type
 *
 * GIS Federal, Inc.
 * ***************************************
 */


#include <stdio.h>

#include "GPUdb.h"


int main(int argc, char* argv[])
{
    // Replace with the IP address and port of your GPUdb
    GPUdb gpudb( "127.0.0.1", 9191, "JSON" );

    // Check that the GPUdb handler was created successfully
    if ( gpudb.status() == gpudb::ERROR )
    {
        std::cerr << "Error in creating GPUdb handler: " << gpudb.error_message() << std::endl;
        std::cerr << "Quitting program!\n";
        return 0;
    }

    // Register a data type
    gpudb::register_type_response register_type_resp;

    std::string point_type = "{\"type\":\"record\",\"name\":\"point\",\"fields\":[{\"name\":\"x\",\"type\":\"double\"},{\"name\":\"y\",\"type\":\"double\"},{\"name\":\"OBJECT_ID\",\"type\":\"string\"}]}";

    if ( gpudb.register_type( point_type, "", "", "POINT", register_type_resp ) == true )
        std::cout << "Registered point type ID: " << register_type_resp.type_id << std::endl;
    else
        std::cout << "Error in registering point type: " << gpudb.error_message() << std::endl;

    

    return 0;
}  // end main



// Expected Output:

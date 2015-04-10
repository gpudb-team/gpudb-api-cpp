/* **********************************
* GPUdb C++ API Example: Status
*
* GIS Federal, Inc.
* **********************************
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
 
    // Clear response container
    gpudb::clear_response clear_resp;
 
    // Sample of how error status code and message retrieval works
    if ( gpudb.clear( "point_set", "", clear_resp ) == false )
        std::cout << "\nTesting an error case--clearing a set before creating it (expecting an error message here): "
                  << gpudb.error_message() << std::endl << std::endl;
 
    // Register a data type
    gpudb::register_type_response register_type_resp;
 
    std::string point_type = "{\"type\":\"record\",\"name\":\"point\",\"fields\":[{\"name\":\"x\",\"type\":\"double\"},{\"name\":\"y\",\"type\":\"double\"},{\"name\":\"OBJECT_ID\",\"type\":\"string\"}]}";
 
    std::cout << "Registering point type: " << gpudb.register_type( point_type, "", "", "POINT", register_type_resp ) << std::endl;
 
    // Create a new set (using the previous response information)
    gpudb::new_set_response new_set_resp;
 
    // Create a top level set using the newly registered data type ID
    std::cout << "Creating a set (with type ID " << register_type_resp.type_id << "): "
              << gpudb.new_set( register_type_resp.type_id,
                                "point_set", "", new_set_resp )
              << std::endl;
 
    // Check the status of the newly created sets
    gpudb::status_response status_resp;
 
    if ( gpudb.status( "point_set", status_resp ) == true )
    {
        for (size_t i = 0; i < status_resp.set_ids.size(); ++i )
            std::cout << "Set " << status_resp.set_ids[ i ]
                      << " with semantic type " << status_resp.semantic_types[ i ]
                      << " size: " << status_resp.full_sizes[ i ] << std::endl;
    }
    else
        std::cout << "Error in checking set status: " << gpudb.error_message();
    
 
    return 0;
}  // end main
 
 
 
// Expected Output:
//
// Testing an error case--clearing a set before creating it (expecting an error message here): Unable to clear unknown set id: point_set
//
// Registering point type: 1
// Creating a set (with type ID 4885851200021372076): 1
// Set point_set with semantic type POINT size: 0

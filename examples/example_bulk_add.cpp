/* **********************************
 * GPUdb C++ API Example: Bulk Add
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

    // Register a data type
    gpudb::register_type_response register_type_resp;

    std::string point_type = "{\"type\":\"record\",\"name\":\"point\",\"fields\":[{\"name\":\"x\",\"type\":\"double\"},{\"name\":\"y\",\"type\":\"double\"},{\"name\":\"OBJECT_ID\",\"type\":\"string\"}]}";

    std::cout << "Registering point type: " << gpudb.register_type( point_type, "", "", "POINT", register_type_resp ) << std::endl;

    // Create a new set (using the previous response information)
    gpudb::new_set_response new_set_resp;
    std::string set_id = "bulk_add_example_set";

    // Create a top level set using the newly registered data type ID
    std::cout << "Creating a set (with type ID " << register_type_resp.type_id << "): "
              << gpudb.new_set( register_type_resp.type_id,
                                set_id, "", new_set_resp )
              << std::endl;


    // Add multiple objects all at once
    gpudb::bulk_add_response bulk_add_resp;

    std::vector<std::string> obj_list;
    char buf[100];

    // Create a list of 100 objects
    for ( int i = 0; i < 100; ++i )
    {
        sprintf( buf, "{\"x\":%d,\"y\":%d,\"OBJECT_ID\":\"\"}", i-10, i-20 );
        obj_list.push_back( buf );
    }
    if ( gpudb.bulk_add( set_id, obj_list, point_type, gpudb::NONE, bulk_add_resp ) == true )
    {
        std::cout << "Added 100 objects in bulk successfully\n";
        for ( size_t i = 0; i < 100; ++i )
            std::cout << i << "th objects ID: " << bulk_add_resp.OBJECT_IDs[ i ] << std::endl;
    }
    else
        std::cout << "Error in adding object; error message: " << gpudb.error_message() << std::endl;



    return 0;
}  // end main



// Expected Output:

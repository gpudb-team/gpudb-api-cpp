/* **********************************
 * GPUdb C++ API Example: Get Set
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
    std::string set_id = "point_set";

    // Create a top level set using the newly registered data type ID
    std::cout << "Creating a set (with type ID " << register_type_resp.type_id << "): "
              << gpudb.new_set( register_type_resp.type_id,
                                set_id, "", new_set_resp )
              << std::endl;

    // Add multiple objects all at once
    gpudb::bulk_add_response bulk_add_resp;

    std::vector<std::string> obj_list;
    char buf[100];

    for ( int i = 0; i < 100; ++i )
    {
        sprintf( buf, "{\"x\":%d,\"y\":%d,\"OBJECT_ID\":\"\"}", i-10, i-20 );
        obj_list.push_back( buf );
    }
    std::cout << "Add 100 objects to the child set in bulk: "
              << gpudb.bulk_add( set_id, obj_list, point_type, gpudb::NONE, bulk_add_resp )
              << std::endl;

    // Check the status of the newly created set
    gpudb::status_response status_resp;

    std::cout << "Check status of the set: " << gpudb.status( set_id, status_resp ) << std::endl;

    // Get the objects back from the set
    gpudb::get_set_response get_set_resp;

    // Since the set in concern is a child-type set, no semantic type needs
    // to be provided
    bool get_set_ret_val = gpudb.get_set( set_id, 0, 10, "", get_set_resp);
    std::cout << "Get set data: " << get_set_ret_val << std::endl;

    if ( get_set_ret_val == true )
    {
        std::cout << "Set objects:\n";
        // We can use the 0th index of the status response 'sizes' vector
        // since only one set's objects were requested
        for (size_t i = 0; i < get_set_resp.list_str.size(); ++i )
            std::cout << "\t" << get_set_resp.list_str[ i ] << "\n";
    }
    

    return 0;
}  // end main



// Expected Output:

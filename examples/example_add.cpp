/* **********************************
 * GPUdb C++ API Example: Add
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
    std::string set_id = "add_example_set";

    // Create a top level set using the newly registered data type ID
    std::cout << "Creating a set (with type ID " << register_type_resp.type_id << "): "
              << gpudb.new_set( register_type_resp.type_id,
                                set_id, "", new_set_resp )
              << std::endl;


    // Add some objects to the set
    gpudb::add_object_response add_obj_resp;

    // Need to format the object using JSON formatting
    std::string obj1 = "{\"x\":123.45,\"y\":23.34,\"OBJECT_ID\":\"abcd\"}";
    std::string obj2 = "{\"x\":100.10,\"y\":33.33,\"OBJECT_ID\":\"efgh\"}";
    std::string obj3 = "{\"x\":-78.91,\"y\":42.42,\"OBJECT_ID\":\"blah\"}";

    // We can reuse the response structure
    // and we're not using any special parameters (gpudb::NONE)
    if ( gpudb.add_object( set_id, obj1, point_type, gpudb::NONE, add_obj_resp ) == true )
        std::cout << "Added object successfully; object ID: " << add_obj_resp.OBJECT_ID << std::endl;
    else
        std::cout << "Error in adding object; error message: " << gpudb.error_message() << std::endl;

    if ( gpudb.add_object( set_id, obj2, point_type, gpudb::NONE, add_obj_resp ) == true )
        std::cout << "Added object successfully; object ID: " << add_obj_resp.OBJECT_ID << std::endl;
    else
        std::cout << "Error in adding object; error message: " << gpudb.error_message() << std::endl;

    if ( gpudb.add_object( set_id, obj3, point_type, gpudb::NONE, add_obj_resp ) == true )
        std::cout << "Added object successfully; object ID: " << add_obj_resp.OBJECT_ID << std::endl;
    else
        std::cout << "Error in adding object; error message: " << gpudb.error_message() << std::endl;



    return 0;
}  // end main



// Expected Output:

/* **********************************
 * GPUdb C++ API Test Harness
 *
 * @author Meem Mahmud
 * GIS Federal, Inc.
 * **********************************
 */


#include <stdio.h>

#include "GPUdb.h"


int main(int argc, char* argv[])
{
    try
    {
        // Various legal and illegal (error-prone) ways to create a GPUdb object
        // GPUdb gpudb; // valid; use "127.0.0.1:9191" for ip:port and binary encoding by default
        // GPUdb gpudb( "127x0.0.1", 9191, "BINARY" ); // invalid address
        // GPUdb gpudb( "127x0.0.1", 9190, "BINARY" ); // invalid address and port
        // GPUdb gpudb( "127x0.0.1", 9191, "BINARY", true ); // invalid address, using exceptions
        // GPUdb gpudb( "127.0.0.1", 8080, "JSON" ); // invalid port, json encoding
        // GPUdb gpudb( "127.0.0.1", 8080, "BINARY" ); // invalid port, binary encoding
        // GPUdb gpudb( "127.0.0.1", 9191, "BINARY", true ); // valid; enable exception throwing
        // GPUdb gpudb( "127.0.0.1", 9191, "JSON", true ); // valid; enable exception throwing
        GPUdb gpudb( "127.0.0.1", 9191, "JSON" ); // valid; use JSON encoding
        // GPUdb gpudb( "127.0.0.1", 9191, "BINARY" ); // valid; use BINARY encoding

        // Check that the GPUdb handler was created successfully
        if ( gpudb.status() == gpudb::ERROR )
        {
            std::cerr << "Error in creating GPUdb handler: " << gpudb.error_message() << std::endl;
            std::cerr << "Quitting program!\n";
            return 0;
        }

        // Clear any pre-existing sets with the names to be used here
        gpudb::clear_response clear_resp;

        // Sample of how error status code and message retrieval works
        if ( gpudb.clear( "non_existing_set_name", "", clear_resp ) == false )
            std::cout << "\nTesting an error case (expecting an error here): " << gpudb.error_message() << std::endl << std::endl;

        // Now clear any lingering sets from previous runs of this code
        gpudb.clear( "point_set_parent", "", clear_resp );
        gpudb.clear( "point_set_child", "", clear_resp );
        gpudb.clear( "bbox_result", "", clear_resp );
        gpudb.clear( "bbox_result2", "", clear_resp );

        // Register type
        gpudb::register_type_response register_type_resp;

        std::string point_type = "{\"type\":\"record\",\"name\":\"point\",\"fields\":[{\"name\":\"x\",\"type\":\"double\"},{\"name\":\"y\",\"type\":\"double\"},{\"name\":\"OBJECT_ID\",\"type\":\"string\"}]}";

        std::cout << "Registering point type: " << gpudb.register_type( point_type, "", "", "POINT", register_type_resp ) << std::endl;

        // Create new sets (using the previous response information)
        gpudb::register_parent_set_response register_parent_set_resp;  // for parent set
        gpudb::new_set_response new_set_resp;  // for child set

        // Register/create the set as a parent and allow duplicate children
        std::cout << "Registering a parent set: " << gpudb.register_parent_set( "point_set_parent", true, register_parent_set_resp )
                  << std::endl;

        // Create a child set using the parent from above and
        // also the newly registered data type ID
        std::cout << "Creating a child set (with type ID " << register_type_resp.type_id << "): "
                  << gpudb.new_set( register_type_resp.type_id,
                                    "point_set_child", "point_set_parent", new_set_resp )
                  << std::endl;


        // Add some objects to the child set
        gpudb::add_object_response add_obj_resp;

        // Need to format the object using JSON formatting
        std::string obj = "{\"x\":123.45,\"y\":23.34,\"OBJECT_ID\":\"abcd\"}";
        std::cout << "Adding an object: "
                  << gpudb.add_object( "point_set_child", obj, point_type, gpudb::NONE, add_obj_resp )
                  << std::endl;
        obj = "{\"x\":100.10,\"y\":33.33,\"OBJECT_ID\":\"efgh\"}";
        std::cout << "Adding an object: "
                  << gpudb.add_object( "point_set_child", obj, point_type, gpudb::NONE, add_obj_resp )
                  << std::endl;
        obj = "{\"x\":-78.91,\"y\":42.42,\"OBJECT_ID\":\"blah\"}";
        std::cout << "Adding an object: "
                  << gpudb.add_object( "point_set_child", obj, point_type, gpudb::NONE, add_obj_resp )
                  << std::endl;

        // Check the status of the newly created sets
        // (Note that the response is being reused as we're not doing
        // anything meaningful with the returned values yet)
        gpudb::status_response status_resp;

        std::cout << "Check status of parent set: " << gpudb.status( "point_set_parent", status_resp ) << std::endl;
        std::cout << "Check status of child set: " << gpudb.status( "point_set_child", status_resp ) << std::endl;


        // Get the objects from the child set
        gpudb::get_set_response get_set_resp;

        bool get_set_ret_val = gpudb.get_set( "point_set_child", 0, 10, "", get_set_resp);
        std::cout << "Get child set data: " << get_set_ret_val << std::endl;

        if ( get_set_ret_val == true )
        {
            std::cout << "Child set objects:\n";
            std::cout << get_set_resp.list.size() << std::endl;
            // We know that point_set_child is a single child set;
            // so we can use the 0th index of the status response 'sizes' vector
            for (int i = 0; i < status_resp.sizes[0]; ++i )
                std::cout << "\t" << get_set_resp.object_ids[ i ] << "\n";
        }

        // Perform a bounding box filter on the child set
        gpudb::bounding_box_response bbox_resp;

        std::cout << "Bounding box filter: "
                  << gpudb.bounding_box( "point_set_child", "bbox_result", -80, 110, 30, 50, "x", "y", bbox_resp )
                  << "; resulting count (expecting 2): ";
        std::cout << bbox_resp.count << std::endl;


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
                  << gpudb.bulk_add( "point_set_child", obj_list, point_type, gpudb::NONE, bulk_add_resp )
                  << std::endl;


        // Perform another bounding box filter on the child set
        std::cout << "Bounding box filter after the bulk add: "
                  << gpudb.bounding_box( "point_set_child", "bbox_result2", 0, 10, -10, 0, "x", "y", bbox_resp )
                  << "; resulting count (expecting 11): ";
        std::cout << bbox_resp.count << std::endl;


        // // Commented out so that the data can be viewed on Gadmin or Gaiademo
        // // Clear the sets created in this program; note the reuse of the response
        // // structure since it's not really used in any meaningful way
        // std::cout << "Clearing the parent set: " << gpudb.clear( "point_set_parent", "", clear_resp )
        //           << std::endl;
        // std::cout << "Clearing the child set: " << gpudb.clear( "point_set_child", "", clear_resp )
        //           << std::endl;
        // std::cout << "Clearing the bounding box result set: " << gpudb.clear( "bbox_result", "", clear_resp )
        //           << std::endl;
        // std::cout << "Clearing the 2nd bounding box result set: " << gpudb.clear( "bbox_result2", "", clear_resp )
        //           << std::endl;
    }
    catch ( const std::exception &e )
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught.\n";
    }

    return 0;
}  // end main

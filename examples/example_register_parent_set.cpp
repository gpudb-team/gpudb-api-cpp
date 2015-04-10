/* ********************************************
 * GPUdb C++ API Example: Regiser Parent Set
 *
 * GIS Federal, Inc.
 * ********************************************
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

    // Create a new parent set
    gpudb::register_parent_set_response register_parent_set_resp;

    if ( gpudb.register_parent_set( "parent_set", true, register_parent_set_resp ) == true )
        std::cout << "New parent set created with ID:  " << register_parent_set_resp.set_id << std::endl;
    else
        std::cout << "Error in creating parent set: " << gpudb.error_message();


    return 0;
}  // end main



// Expected Output:

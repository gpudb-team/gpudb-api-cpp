/* **********************************
 * GPUdb C++ API
 * Header file for class GPUdb
 *
 * @author Meem Mahmud
 * GIS Federal, Inc.
 * **********************************
 */

#ifndef _GPUDB_H_
#define _GPUDB_H_


#include <stdio.h>
#include <string>

#include "Utils/HTTPUtils.h"
#include "Utils/AvroUtils.h"

#include "obj_defs/addobjectrequest.h"
#include "obj_defs/addobjectresponse.h"
#include "obj_defs/boundingboxrequest.h"
#include "obj_defs/boundingboxresponse.h"
#include "obj_defs/bulkaddrequest.h"
#include "obj_defs/bulkaddresponse.h"
#include "obj_defs/clearrequest.h"
#include "obj_defs/clearresponse.h"
#include "obj_defs/getsetrequest.h"
#include "obj_defs/getsetresponse.h"
#include "obj_defs/newsetrequest.h"
#include "obj_defs/newsetresponse.h"
#include "obj_defs/registerparentsetrequest.h"
#include "obj_defs/registerparentsetresponse.h"
#include "obj_defs/registertyperequest.h"
#include "obj_defs/registertyperesponse.h"
#include "obj_defs/statusrequest.h"
#include "obj_defs/statusresponse.h"




namespace gpudb
{
    enum status_code
    {
        OK,
        ERROR
    };


    /// /add and /bulkadd parameters
    enum add_parameter
    {
        NONE, /// No parameter provided
        /// Enables updating/replacing objects with existing primary keys
        UPDATE_ON_EXISTING_PK
    };  // end enum add_parameter


}  // end namespace gpudb







class GPUdb
{
private:

    std::string g_ip;  // The IP address of the GPUdb
    std::string g_port;  // The port at which GPUdb listens
    std::string g_encoding; // The encoding of this GPUdb handler instance
    std::string g_username; // Username for the GPUdb
    std::string g_password; // Password for the GPUdb
    std::string g_error_message; // Holds the last query's error message, if any
    gpudb::status_code g_status; // Indicates if the last query was successful or a failure
    bool g_throw_exceptions; // Make exception throwing optional; suppressed by default (not ideal C++ practice)

public:
    // Create a connection with a local running GPUdb by default
    GPUdb() : g_ip( "127.0.0.1" ), g_port( "9191" ), g_encoding( "BINARY" ),
              g_username ( "" ), g_password ( "" ),
              g_error_message( "" ), g_status( gpudb::OK ), g_throw_exceptions( false ) {}

    // Create a connection with a GPUdb server at the specified location
    // Optional parameters:
    //     username -- username for the GPUdb
    //     password -- password for the GPUdb
    //     throw_exceptions -- for enabling exception throwing (disabled by default)
    GPUdb( std::string ip, int port, std::string encoding,
           std::string username = "", std::string password = "",
           bool throw_exceptions = false );


    // Make an HTTP request to GPUdb with the given endpoint and data
    // and extract the response data
    // Returns success or failure
    // Also stores error message internally for the user's future perusal
    template <class Treq, class Tresp>
    bool query( const Treq& request_data, const std::string& endpoint, Tresp& response );

    // Returns the current status of this GPUdb handler instance
    gpudb::status_code status();

    // Returns the current error message, if any (empty string otherwise)
    std::string error_message();

    // Add an object to an existing set in GPUdb
    bool add_object( const std::string &set_id,
                     const std::string &object_data_json,
                     const std::string &type_definition,
                     const gpudb::add_parameter& param,
                     gpudb::add_object_response &response );

    // Add an object to an existing set in GPUdb
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool add_object( const gpudb::add_object_request &request,
                     gpudb::add_object_response &response );

    // Add multiple objects to an existing set in GPUdb
    bool bulk_add( const std::string& set_id,
                   const std::vector<std::string>& object_json_list,
                   const std::string& type_definition,
                   const gpudb::add_parameter& param,
                   gpudb::bulk_add_response& response );


    // Add multiple objects to an existing set in GPUdb
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool bulk_add( const gpudb::bulk_add_request& request,
                   gpudb::bulk_add_response& response );


    // Do a bounding box filter on a given set
    bool bounding_box( const std::string& set_id,
                       const std::string& result_set_id,
                       const double& min_x,
                       const double& max_x,
                       const double& min_y,
                       const double& max_y,
                       const std::string& x_attr_name,
                       const std::string& y_attr_name,
                       gpudb::bounding_box_response& response );

    // Do a bounding box filter on a given set
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool bounding_box( const gpudb::bounding_box_request& request,
                       gpudb::bounding_box_response& response );

    // Clear an existing set in GPUdb
    bool clear( const std::string &set_id,
                const std::string &authorization,
                gpudb::clear_response &response );

    // Clear an existing set in GPUdb
    bool clear( const gpudb::clear_request &request,
                gpudb::clear_response &response );

    // Get the data from an existing set in GPUdb
    bool get_set( const std::string& set_id,
                  const int64_t& start,
                  const int64_t& end,
                  const std::string& semantic_type,
                  gpudb::get_set_response& response );

    // Get the data from an existing set in GPUdb
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool get_set( const gpudb::get_set_request& request,
                  gpudb::get_set_response& response );

    // Create a new set in GPUdb
    bool new_set( const std::string &set_id,
                  const std::string &parent_set_id,
                  const std::string &type_id,
                  gpudb::new_set_response &response );

    // Create a new set in GPUdb
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool new_set( const gpudb::new_set_request &request,
                  gpudb::new_set_response &response );

    // Register a set as a parent set
    bool register_parent_set( const std::string &set_id,
                              bool allow_duplicate_children,
                              gpudb::register_parent_set_response &response );

    // Register a set as a parent set
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool register_parent_set( const gpudb::register_parent_set_request &request,
                              gpudb::register_parent_set_response &response );

    // Register a data type definition in GPUdb
    bool register_type( const std::string &type_definition,
                        const std::string &annotation,
                        const std::string &label,
                        const std::string &semantic_type,
                        gpudb::register_type_response &response );

    // Register a data type definition in GPUdb
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool register_type( const gpudb::register_type_request &request,
                        gpudb::register_type_response &response );

    // Return the status of a set in GPUdb
    bool status( std::string set_id, gpudb::status_response &response );

    // Return the status of a set in GPUdb
    // Note: all parameter checking is done in GPUdb internally and
    //        not here; so use this version at your own risk
    bool status( const gpudb::status_request &request, gpudb::status_response &response );

};  // end class GPUdb


#endif

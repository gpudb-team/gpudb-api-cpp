/* **********************************
 * GPUdb C++ API
 * Class GPUdb Implementation
 *
 * @author Meem Mahmud
 * GIS Federal, Inc.
 * **********************************
 */

#include <stdio.h>
#include <exception>
#include <sstream>
#include <map>

#include "GPUdb.h"
#include "Utils/GPUdbExceptions.h"






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructors and other function(s)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Create a connection with a GPUdb server at the specified location
// Optional parameters:
//     username -- username for the GPUdb
//     password -- password for the GPUdb
//     throw_exceptions -- for enabling exception throwing (disabled by default)
GPUdb::GPUdb( std::string ip, int port, std::string encoding,
              std::string username, std::string password,
              bool throw_exceptions )
{
    // Set the username and password
    g_username = username;
    g_password = password;

    // Set status and error messages to OK by default
    g_error_message = "";
    g_status = gpudb::OK;

    // Perhaps not ideal C++, but lets the user decide if they want exceptions
    g_throw_exceptions = throw_exceptions;

    // Set the IP address and the port (need to conver to string)
    g_ip = ip;
    std::stringstream ss;
    ss << port;
    g_port = ss.str();


    // TODO: Support SNAPPY encoding
    // if ( encoding == "BINARY" || encoding == "JSON" || encoding == "SNAPPY" )
    if ( encoding == "BINARY" || encoding == "JSON" )
        g_encoding = encoding;
    else
    {
        g_error_message = "Invalid encoding provided: " + encoding;
        g_status = gpudb::ERROR;
        if ( g_throw_exceptions )
            throw gpudb::InvalidEncodingException();
    }

    // TODO: Support HTTP and HTTPS


    // Check that a connection can be established at the given IP address & port
    // by making an HTTP call to GPUdb
    try
    {
        gpudb::HTTPUtils::call_gpudb( "{\"\"}", "/serverstatus",
                                      g_ip, g_port, g_username, g_password );
    }
    catch ( const std::exception &e )
    {   // Set the error message and status, throw if needed
        std::stringstream err_ss;
        err_ss << "Could not connect to GPUdb (" << e.what() << ")";
        g_error_message = err_ss.str();
        g_status = gpudb::ERROR;
        if ( g_throw_exceptions )
            throw;
    }

}   // end constructor



// Returns the current status of this GPUdb handler instance
gpudb::status_code GPUdb::status()
{
    return g_status;
} // end status


// Returns the current error message, if any (empty string otherwise)
std::string GPUdb::error_message()
{
    return g_error_message;
}  // end error_message





// Make an HTTP request to GPUdb with the given endpoint and data
// and extract the response data
// Also stores any error message internally for the user's future use
// Returns success or failure or throws exceptions if enabled in the constructor
template <class Treq, class Tresp>
bool GPUdb::query( const Treq& request_data,
                   const std::string& endpoint,
                   Tresp& response )
{
    gpudb::gpudb_response gresponse;

    // For binary encoding, convert the object before the HTTP call
    if ( g_encoding == "BINARY" )
    {
        std::vector<uint8_t> avro_data;

        // Convert the data to Avro format
        gpudb::AvroUtils::convert_to_bytes<Treq>( request_data, avro_data );

        // Make an HTTP call to GPUdb
        gresponse = gpudb::HTTPUtils::call_gpudb( avro_data, endpoint, g_ip, g_port, g_username, g_password );

        // Upon success, convert the returned data to human readable data
        if ( gresponse.status == "OK" )
        {
            if( gpudb::AvroUtils::convert_to_object( gresponse.data, response ) == false )
            {
                // Indicate that there was a decoding error
                gresponse.status = "ERROR";
                gresponse.message = "Problem decoding Avro object for " + response.schema_name();
            }
        }
    }  // end binary encoding
    else if ( g_encoding == "JSON" )
    {   // for JSON encoding, convert the object to a JSON formatted string
        // then make the HTTP call
        std::string json_data;
        
        // Convert the data to JSON formatted string
        gpudb::AvroUtils::convert_to_json_by_schema_str<Treq>( request_data,
                                                               request_data.schema_str(),
                                                               json_data );
        // Make an HTTP call to GPUdb
        gresponse =  gpudb::HTTPUtils::call_gpudb( json_data, endpoint, g_ip, g_port, g_username, g_password );

        // Upon success, convert the returned data to human readable data
        if ( gresponse.status == "OK" )
        {
            if ( gpudb::AvroUtils::convert_to_object( gresponse.data_str, response ) == false )
            {
                // Indicate that there was a decoding error
                gresponse.status = "ERROR";
                gresponse.message = "Problem decoding Avro object for " + response.schema_name();
            }
        }
    }  // end json encoding
    else // unacceptable encoding
    {
        // Indicate error (but should not reach this point)
        gresponse.status = "ERROR";
        gresponse.message = "Bad encoding: neither BINARY nor JSON!";
    }

    if ( gresponse.status == "ERROR" )
    { // in case of an error, store the error message and status
        g_status = gpudb::ERROR;
        g_error_message = gresponse.message;

        // Perhaps not ideal C++, but lets the user decide if they want exceptions
        if ( g_throw_exceptions )
            throw gpudb::QueryException( g_error_message );

        return false;
    }
    else // successfully made the GPUdb query
    {   // reset the error status and message
        g_status = gpudb::OK;
        g_error_message = "";

        return true;
    }
}  // end query




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constructors and other function(s) (end)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GPUdb Endpoint API Wrapper Functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// Ping GPUdb
bool GPUdb::ping( std::string &response )
{
    // Ping GPUdb
    response = gpudb::HTTPUtils::ping( g_ip, g_port );

    // Response should not be empty
    if ( response == "" )
        return false;
    return true;

}  // end ping


// Add an object to an existing set in GPUdb
// In: set_id -- name of the set to which the object will be added (required)
//     object_data_json -- The object data in JSON format
//     type_definition -- Defines the data type (may be needed to convert object
//                        to binary format)
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::add_object_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::add_object( const std::string &set_id,
                        const std::string &object_data_json,
                        const std::string &type_definition,
                        const gpudb::add_parameter& param,
                        gpudb::add_object_response &response )
{
    gpudb::add_object_request request;

    // Set the request parameters
    request.set_id = set_id;

    if ( g_encoding == "BINARY" || g_encoding == "SNAPPY" )
    {
        request.object_data_str = "";
        request.object_encoding = "BINARY";
        if ( false == gpudb::AvroUtils::convert_json_to_binary_by_schema_str( object_data_json,
                                                                       type_definition,
                                                                       request.object_data ) )
            return false;
    }
    else
    {
        request.object_data_str = object_data_json;
        request.object_encoding = "JSON";
    }

    switch ( param ) // parse parameter options
    {
        case gpudb::UPDATE_ON_EXISTING_PK:
            // Add this parameter to be true
            request.params.insert( std::pair<std::string, std::string >( "update_on_existing_pk", "true" ) );
            break;
        case gpudb::NONE:
        default:
            // nothing to do; params will be empty
            break;
    }  // end switch on param

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/add", response );
}   // end add_object



// Add an object to an existing set in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::add_object_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::add_object( const gpudb::add_object_request &request,
                        gpudb::add_object_response &response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/add", response );
}   // end add_object with request struct



// Add multiple objects to an existing set in GPUdb
// In: set_id -- name of the set to which the objects will be added (required)
//     object_json_list -- A list of the object data in JSON format
//     type_definition -- Defines the data type (may be needed to convert object
//                        to binary format)
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::bulk_add_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::bulk_add( const std::string& set_id,
                      const std::vector<std::string>& object_json_list,
                      const std::string& type_definition,
                      const gpudb::add_parameter& param,
                      gpudb::bulk_add_response& response )
{
    gpudb::bulk_add_request request;

    // Set the request parameters
    request.set_id = set_id;

    // Get the number of objects to add
    size_t num_objs = object_json_list.size();

    if ( g_encoding == "BINARY" || g_encoding == "SNAPPY" )
    {
        request.list_encoding = "BINARY";

        for ( size_t i = 0; i < num_objs; ++i )
        {
            std::vector<uint8_t> bin_obj_data;
            request.list_str.push_back( "" ); // used for JSON encoding only
            if ( false == gpudb::AvroUtils::convert_json_to_binary_by_schema_str( object_json_list[ i ],
                                                                           type_definition,
                                                                           bin_obj_data ) )
                return false;
            request.list.push_back( bin_obj_data );
        }
    }
    else
    {
        // GPUdb derives the number of objects to add from
        // the size of the binary container; so push empty things
        for ( size_t i = 0; i < num_objs; ++i )
        {
            std::vector<uint8_t> empty_bin_data;
            request.list.push_back( empty_bin_data );
        }
        request.list_str = object_json_list;
        request.list_encoding = "JSON";
    }

    switch ( param ) // parse parameter options
    {
        case gpudb::UPDATE_ON_EXISTING_PK:
            // Add this parameter to be true
            request.params.insert( std::pair<std::string, std::string >( "update_on_existing_pk", "true" ) );
            break;
        case gpudb::NONE:
        default:
            // nothing to do; params will be empty
            break;
    }  // end switch on param


    // Make an HTTP call to GPUdb and return the response
    return query( request, "/bulkadd", response );
}   // end bulk_add


// Add multiple objects to an existing set in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::bulk_add_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::bulk_add( const gpudb::bulk_add_request& request,
                      gpudb::bulk_add_response& response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/bulkadd", response );
}   // end bulk_add with request struct




// Do a bounding box filter on a given set
// In: set_id -- name of the set on which the filter will be performed (required)
//     result_set_id -- name of the resultant set
//     min_x -- minimum x attribute value for the bounding box
//     max_x -- maximum x attribute value for the bounding box
//     min_y -- minimum y attribute value for the bounding box
//     max_y -- maximum y attribute value for the bounding box
//     x_attr_name -- name of the attribute that will be treated as the 'x' (1st) attribute
//     y_attr_name -- name of the attribute that will be treated as the 'y' (2nd) attribute
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::bounding_box_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::bounding_box( const std::string& set_id,
                          const std::string& result_set_id,
                          const double& min_x,
                          const double& max_x,
                          const double& min_y,
                          const double& max_y,
                          const std::string& x_attr_name,
                          const std::string& y_attr_name,
                          gpudb::bounding_box_response& response )
{
    gpudb::bounding_box_request request;

    // Set the request parameters
    request.set_id = set_id;
    request.result_set_id = result_set_id;
    request.min_x = min_x;
    request.max_x = max_x;
    request.min_y = min_y;
    request.max_y = max_y;
    request.x_attr_name = x_attr_name;
    request.y_attr_name = y_attr_name;
    request.user_auth_string = "";

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/boundingbox", response );
}   // end bounding_box


// Do a bounding box filter on a given set
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::bounding_box_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::bounding_box( const gpudb::bounding_box_request& request,
                          gpudb::bounding_box_response& response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/boundingbox", response );
}   // end bounding_box with request struct



// Clear an existing set in GPUdb
// In: set_id -- name of the set to be deleted/cleared (required)
//     authorization -- deprecated parameter, user should pass an empty string
//                      (any non-empty string will be disregarded)
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::clear_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::clear( const std::string &set_id,
                   const std::string &authorization,
                   gpudb::clear_response &response )
{
    gpudb::clear_request  request;

    // Set the request parameters
    request.set_id = set_id;
    request.authorization = authorization;

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/clear", response );
}   // end clear


// Clear an existing set in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::clear_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::clear( const gpudb::clear_request &request,
                   gpudb::clear_response &response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/clear", response );
}   // end clear with request struct



// Get the data from an existing set in GPUdb
// In: set_id -- name of the set from which data is to be retrieved (required)
//     start -- first index of the range of objects to retrieve
//     end -- last index of the range of objects to retrieve
//     semantic_type -- The type of objects to retrieve (relevant only for
//                      parent sets that have child sets of different types)
//                      Can be left empty.
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::get_set_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::get_set( const std::string& set_id,
                     const int64_t& start,
                     const int64_t& end,
                     const std::string& semantic_type,
                     gpudb::get_set_response& response )
{
    gpudb::get_set_request  request;

    // Set the request parameters
    request.set_id = set_id;
    request.start = start;
    request.end = end;
    request.semantic_type = semantic_type;
    request.user_auth_string = "";

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/getset", response );
}   // end get_set


// Get the data from an existing set in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::get_set_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::get_set( const gpudb::get_set_request& request,
                     gpudb::get_set_response& response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/getset", response );
}   // end get_set with request struct



// Create a new set in GPUdb
// In: type_id -- A string identifying the data type of the new set (required)
//     set_id -- name of the set to be created (required)
//     parent_set_id -- Name of an optional parent set (can be an empty string)
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::new_set_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::new_set( const std::string &type_id,
                     const std::string &set_id,
                     const std::string &parent_set_id,
                     gpudb::new_set_response &response )
{
    gpudb::new_set_request  request;

    // Set the request parameters
    request.type_id = type_id;
    request.set_id = set_id;
    request.parent_set_id = parent_set_id;

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/newset", response );
}   // end new_set


// Create a new set in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean value indicating failure/success 
//      Sets gpudb::new_set_response structure member variables to
//      values returned by GPUdb (only upon success)
bool GPUdb::new_set( const gpudb::new_set_request &request,
                     gpudb::new_set_response &response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/newset", response );
}   // end new_set with request struct




// Register a set as a parent set
// In: set_id -- Name of an existing GPUdb set to be registered
//               to be a parent set; required parameter
//     allow_duplicate_children -- Allow/disallow duplicate children
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean failure/success 
//      Sets gpudb::register_parent_set_response structure to
//      values returned by GPUdb upon success
bool GPUdb::register_parent_set( const std::string &set_id,
                                 bool allow_duplicate_children,
                                 gpudb::register_parent_set_response &response )
{
    gpudb::register_parent_set_request request;

    // Set the request parameters
    request.set_id = set_id;
    request.allow_duplicate_children = allow_duplicate_children;

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/registerparentset", response );
}   // end register_parent_set


// Register a set as a parent set
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean failure/success 
//      Sets gpudb::register_parent_set_response structure to
//      values returned by GPUdb upon success
bool GPUdb::register_parent_set( const gpudb::register_parent_set_request &request,
                                 gpudb::register_parent_set_response &response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/registerparentset", response );
}   // end register_parent_set with request struct



// Register a data type definition in GPUdb
// In: type definition -- required
//     annotation -- optional (i.e. can be an empty string)
//     label -- optional (i.e. can be an empty string)
//     semantic_type -- required
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean failure/success 
//      Sets gpudb::register_type_response structure to
//      values returned by GPUdb upon success
bool GPUdb::register_type( const std::string &type_definition,
                           const std::string &annotation,
                           const std::string &label,
                           const std::string &semantic_type,
                           gpudb::register_type_response &response )
{
    gpudb::register_type_request  request;

    // Set the request parameters
    request.type_definition = type_definition;
    request.annotation = annotation;
    request.label = label;
    request.semantic_type = semantic_type;

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/registertype", response );
}   // end register_type


// Register a data type definition in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean failure/success 
//      Sets gpudb::register_type_response structure to
//      values returned by GPUdb upon success
bool GPUdb::register_type( const gpudb::register_type_request &request,
                           gpudb::register_type_response &response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/registertype", response );
}   // end register_type with request struct



// Retrieve the status of a set in GPUdb
// In: set_id -- Name of the set whose status is to be retrieved; required
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean failure/success 
//      Sets gpudb::status_response structure to
//      values returned by GPUdb upon success
bool GPUdb::status( std::string set_id, gpudb::status_response &response )
{
    gpudb::status_request  request;

    request.set_id = set_id;

    // Make an HTTP call to GPUdb and return the response
    return query( request, "/status", response );
}   // end status


// Retrieve the status of a set in GPUdb
// In: requst -- Contains all parameters for the query
// (Note: all parameter checking is done in GPUdb internally and
//        not here.)
// Out: Returns boolean failure/success 
//      Sets gpudb::status_response structure to
//      values returned by GPUdb upon success
bool GPUdb::status( const gpudb::status_request &request,
                    gpudb::status_response &response )
{
    // Make an HTTP call to GPUdb and return the response
    return query( request, "/status", response );
}   // end status with request struct

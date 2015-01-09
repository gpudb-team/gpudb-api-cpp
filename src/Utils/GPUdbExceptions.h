#ifndef __GPUDB_EXCEPIONS__
#define __GPUDB_EXCEPIONS__


namespace gpudb
{

class InvalidEncodingException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid encoding provided!\n";
    }
};  // end class InvalidEncodingException


class InvalidIPAddrException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid IP address provided!\n";
    }
};  // end class InvalidIPAddrException


class QueryException : public std::runtime_error
{
public:

    QueryException( const std::string& error ) : std::runtime_error ( error ) {}

};  // end class QueryException


class NetworkException : public std::runtime_error
{
public:

    NetworkException( const std::string& error ) : std::runtime_error ( error ) {}

};  // end class NetworkException


} // end namespace gpudb


#endif // __GPUDB_EXCEPIONS__

#ifndef _AVRO_UTILS_H_
#define _AVRO_UTILS_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <map>

#include <avro/Compiler.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <avro/Specific.hh>
#include <avro/Stream.hh>
#include <avro/Generic.hh> // for encode(e, GenericDatum)

#include "AvroTypes.h"


namespace gpudb
{

class AvroMemoryOutputStream;

// --------------------------------------------------------------------------
/// @class AvroUtils A collection of common avro operations on gpudb avro type.
// --------------------------------------------------------------------------
class AvroUtils
{
private:

    typedef std::map<std::string, avro::ValidSchema> map_string_to_schema;
    static map_string_to_schema string_to_schema;

public:

    /// Init the static schema string to avro::ValidSchema.
    static void initialize();

    /// Clear the static schema string to avro::ValidSchema.
    static void shutdown();

    /// Compile the given schema string into an avro::ValidSchema.
    /// Throws if the schema cannot be converted.
    static avro::ValidSchema compile_schema(const std::string& schema_str);

    /// Get a previously compiled schema or compile it an add it to a map.
    /// Throws if the schema cannot be converted.
    static avro::ValidSchema get_or_compile_schema(const std::string& schema_str);

    // ----------------------------------------------------------------------

    /// Generic function to initialize an avro object.
    /// Specialize in cpp file and declare at bottom of this header for types that require it.
    template <class avro_T>
    static void init_avro_message(avro_T& /* obj */) { }

    // ----------------------------------------------------------------------
    // Json string <--> binary encoding.

    /// Convert the avro object json stream into the binary encoding.
    /// The body of this autogenerated function is in ../obj_defs/obj_defs.cpp
    static bool convert_json_to_binary(std::istream& json_stream,
                                       gpudb::avro_t avro_type,
                                       std::vector<uint8_t>& bytes);
    /// Convert the binary encoded avro object into the json string encoding.
    /// The body of this autogenerated function is in ../obj_defs/obj_defs.cpp
    static bool convert_binary_to_json(const std::vector<uint8_t>& binary,
                                       gpudb::avro_t avro_type,
                                       std::string& json_str);

    /// Take a JSON-encoded string, decode it to an avro object, then re-encode to binary.
    template <class T>
    static bool convert_json_to_binary(const std::string& json_str, std::vector<uint8_t>& bytes);
    /// Take a binary-encoded vector, decode it to an avro object, then re-encode to json.
    template <class T>
    static bool convert_binary_to_json(const std::vector<uint8_t>& binary, std::string& json_str);

    /// Convert the binary encoded data and convert to json using an avro::jsonEncoder(schema).
    static bool convert_binary_to_json(const std::vector<uint8_t>& binary,
                                       const avro::ValidSchema& schema,
                                       const avro::EncoderPtr& jsonEncoder, // expensive to create
                                       std::string& json_str);

    /// Convert the json encoded string to an avro object, then encode to binary.
    static bool convert_json_to_binary_by_schema_str(const std::string& json_str,
                                                     const std::string& type_schema_str,
                                                     std::vector<uint8_t>& avro_object);

    // ----------------------------------------------------------------------
    // Binary/Json encoded bytes to avro object.

    /// Convert binary encoded vector into an avro object.
    template <class T>
    static inline bool convert_to_object(const std::vector<uint8_t>& bytes, T& avro_object)
    {
        return convert_to_object(bytes.data(), bytes.size(), avro_object);
    }

    template <class T>
    static inline bool convert_to_object(const std::string& str, T& avro_object)
    {
        size_t len = str.size();
        if ( len < get_min_binary_encoded_size<T>(avro_object) )
            return false;

        try
        {
            // Note: This is relatively efficient and the input data is not copied.
            std::auto_ptr<avro::InputStream> in( avro::memoryInputStream( (const uint8_t*)str.data(), len ) );

            // Create a schema based on the avro object, create a JSON deocder and decode
            // the data into the object
            avro::ValidSchema type_schema = AvroUtils::get_or_compile_schema( avro_object.schema_str() );
            avro::DecoderPtr d = avro::jsonDecoder( type_schema );
            d->init(*in);
            avro::decode(*d, avro_object);
        }
        catch (const std::exception &e)
        {
            return false;
        }

        return true;
        // return convert_to_object((const uint8_t*)str.data(), str.size(), avro_object);
     }

    /// Convert binary encoded bytes into an avro object.
    template <class T>
    static bool convert_to_object(const uint8_t* bytes, size_t len, T& avro_object);

    /// Convert the json encoded string to an avro object.
    template <class T>
    static bool convert_json_to_object_by_schema_str(const std::string& json_str,
                                                     const std::string& type_schema_str,
                                                     T& avro_object);

    /// Convert the json encoded string to an avro object.
    template <class T>
    static bool convert_json_to_object_by_schema(const std::string& json_str,
                                                 const avro::ValidSchema& schema,
                                                 T& avro_object);

    // ------------------------------------------------------------------------
    // Avro object to binary encoding

    /// Convert an avro object into binary vector.
    /// If buffer_size=0, get_estimated_binary_encoded_size() is used to specify the
    /// buffer for a speed improvement.
    template <class T>
    static bool convert_to_bytes(const T& avro_object,
                                 std::vector<uint8_t>& bytes,
                                 size_t buffer_size = 0);

    /// Convert an avro object into a previously unused binary memory stream.
    /// If buffer_size=0, get_estimated_binary_encoded_size() is used to specify the
    /// buffer for a speed improvement.
    template <class T>
    static bool convert_to_byte_stream(const T& avro_object,
                                       AvroMemoryOutputStream& memStream,
                                       size_t buffer_size = 0);

    // ------------------------------------------------------------------------
    // Object to json string

    /// Convert an avro object into a json string, given a schema string.
    template <class T>
    static bool convert_to_json_by_schema_str(const T& avro_object,
                                              const std::string& type_schema_str,
                                              std::string& json_str);

    /// convert an avro object into a string, given an avro::ValidSchema.
    template <class T>
    static bool convert_to_json_by_schema(const T& avro_object,
                                          const avro::ValidSchema& type_schema,
                                          std::string& json_str);

    /// convert an avro object into a string, given an avro::EncoderPtr.
    template <class T>
    static bool convert_to_json_by_encoder(const T& avro_object,
                                           avro::EncoderPtr encoder,
                                           std::string& json_str);

    // ----------------------------------------------------------------------

    /// Get an estimate of the binary encoded size of an avro object or a 4096 default size.
    /// Specializations autogenerated in ../obj_defs/obj_defs.h/cpp
    template<typename T>
    static inline size_t get_estimated_binary_encoded_size(const T& ) { return 4096; }

    static const size_t AVRO_BINARY_VECT_PAD = sizeof(int64_t); ///< Max number of bytes of avro binary encoded vector's size.

    // Below are the binary encoded sizes of objects in avro.
    static inline size_t get_binary_encoded_size(bool )                         { return sizeof(int8_t);  } // encoded as int8_t
    static inline size_t get_binary_encoded_size(float )                        { return sizeof(int32_t); } // encoded as int32_t
    static inline size_t get_binary_encoded_size(double )                       { return sizeof(int64_t); } // encoded as int64_t
    static inline size_t get_binary_encoded_size(int32_t )                      { return sizeof(int32_t); }
    static inline size_t get_binary_encoded_size(int64_t )                      { return sizeof(int64_t); }
    static inline size_t get_binary_encoded_size(const std::string& s)          { return AVRO_BINARY_VECT_PAD + s.size(); }
    static inline size_t get_binary_encoded_size(const std::vector<uint8_t>& v) { return AVRO_BINARY_VECT_PAD + v.size(); }

    template<typename T>
    static inline size_t get_binary_encoded_size(const std::vector<T>& v) { return AVRO_BINARY_VECT_PAD + v.size()*sizeof(T); }
    template<typename K, typename V>
    static inline size_t get_binary_encoded_size(const std::map<K, V>& m) { return AVRO_BINARY_VECT_PAD + m.size()*(sizeof(K)+sizeof(V)); }

    template<typename T>
    static inline size_t get_binary_encoded_size(const std::vector<std::vector<T> >& v)
    {
        size_t ct = 0;
        for (size_t i=0; i<v.size(); i++)
        {
            ct += (AVRO_BINARY_VECT_PAD + v[i].size() * sizeof(T));
        }
        return AVRO_BINARY_VECT_PAD + ct;
    }

    template<typename T>
    static inline size_t get_binary_encoded_size(const std::vector<std::vector<std::vector<T> > >& v)
    {
        size_t ct = 0;
        size_t vs = v.size();
        size_t vis;
        for (size_t i=0; i<vs; i++)
        {
            ct += AVRO_BINARY_VECT_PAD;
            vis = v[i].size();
            for (size_t j=0; j<vis; j++)
            {
                ct += (AVRO_BINARY_VECT_PAD + v[i][j].size()*sizeof(T));
            }
        }
        return AVRO_BINARY_VECT_PAD + ct;
    }

    static inline size_t get_binary_encoded_size(const std::vector<std::string>& v)
    {
        size_t bytes = 0;
        for (size_t i = 0; i < v.size(); ++i)
            bytes += v[i].size();

        return AVRO_BINARY_VECT_PAD + v.size()*AVRO_BINARY_VECT_PAD + bytes;
    }


    template<typename T>
    static inline size_t get_binary_encoded_size(const std::map<std::string, std::vector<T> >& v)
    {
        if (v.empty())
            return AVRO_BINARY_VECT_PAD;
        else
            return AVRO_BINARY_VECT_PAD+v.size()*(v.begin()->first.size() + v.begin()->second.size() + 2*AVRO_BINARY_VECT_PAD);
    }


    /// Get an estimate of the min binary encoded size of an avro object or 0 if unknown.
    /// Specialize in cpp file and declare at bottom of this header for types
    /// that are known to be larger than the default.
    template<typename T>
    static inline size_t get_min_binary_encoded_size(const T& ) { return 0; }

    // Below are the binary encoded sizes of objects in avro.
    static inline size_t get_min_binary_encoded_size(bool )                 { return sizeof(int8_t);  } // encoded as int8_t
    static inline size_t get_min_binary_encoded_size(float )                { return sizeof(int32_t); } // encoded as int32_t
    static inline size_t get_min_binary_encoded_size(double )               { return sizeof(int64_t); } // encoded as int64_t
    static inline size_t get_min_binary_encoded_size(int32_t )              { return sizeof(int8_t);  } // zig-zag
    static inline size_t get_min_binary_encoded_size(int64_t )              { return sizeof(int8_t);  } // zig-zag
    static inline size_t get_min_binary_encoded_size(const std::string& )   { return sizeof(int8_t);  } // zig-zag size

    template<typename T, typename V>
    static inline size_t get_min_binary_encoded_size(const std::vector<V>& ) { return sizeof(int8_t);  } // zig-zag size
    template<typename T, typename K, typename V>
    static inline size_t get_min_binary_encoded_size(const std::map<K, V>& ) { return sizeof(int8_t);  } // zig-zag size

};

// --------------------------------------------------------------------------
/// @class AvroMemoryOutputStream Exposed avro::MemoryOutputStream from
///                               avro-cpp-1.7.4/impl/Stream.cc to get at data.
// --------------------------------------------------------------------------
class AvroMemoryOutputStream : public avro::OutputStream
{
    friend class AvroUtils; // they can carefully change chunkSize_

    size_t chunkSize_;
    std::vector<uint8_t*> data_;
    size_t available_;
    size_t byteCount_;

public:

    static const size_t DEFAULT_CHUNK_SIZE = 4096;

    AvroMemoryOutputStream(size_t chunkSize = DEFAULT_CHUNK_SIZE) : chunkSize_(chunkSize),
        available_(0), byteCount_(0) { }
    virtual ~AvroMemoryOutputStream() {
        for (std::vector<uint8_t*>::const_iterator it = data_.begin();
            it != data_.end(); ++it) {
            delete[] *it;
        }
    }

    inline bool next(uint8_t** data, size_t* len) {
        if (available_ == 0) {
            data_.push_back(new uint8_t[chunkSize_]);
            available_ = chunkSize_;
        }
        *data = &data_.back()[chunkSize_ - available_];
        *len = available_;
        byteCount_ += available_;
        available_ = 0;
        return true;
    }

    inline void backup(size_t len) {
        available_ += len;
        byteCount_ -= len;
    }

    inline uint64_t byteCount() const {
        return byteCount_;
    }

    inline void flush() { }

    // ----------------------------------------------------------------------
    // Everything above is unmodified, below are added functions.

    /// Copy the data into the buffer, returns buffer size.
    inline size_t getBuffer(std::vector<uint8_t>& bytes) const
    {
        bytes.resize(byteCount_);
        return getBuffer(&bytes[0]);
    }

    /// Copy the data into the buffer, returns buffer size.
    inline size_t getBuffer(std::string& bytes) const
    {
        bytes.resize(byteCount_);
        return getBuffer((uint8_t*)&bytes[0]);
    }

    /// Copy the data into the buffer which MUST be at least byteCount() large,
    /// returns the buffer size.
    size_t getBuffer(uint8_t* bytes) const
    {
        size_t chunks = data_.size();
        size_t start_idx = 0;

        for (size_t i = 0; i < chunks; ++i)
        {
            if (start_idx >= byteCount_)
                break;

            if (start_idx + chunkSize_ <= byteCount_)
                memcpy(bytes+start_idx, data_[i], chunkSize_);
            else
            {
                // at last block that is not completely full
                memcpy(bytes+start_idx, data_[i], byteCount_ - start_idx);
                break;
            }

            start_idx += chunkSize_;
        }

        return byteCount_;
    }
};

// --------------------------------------------------------------------------
// AvroUtils template function definitions
// --------------------------------------------------------------------------

template <class T>
bool AvroUtils::convert_json_to_binary(const std::string& json_str, std::vector<uint8_t>& bytes)
{
    const std::string& schema_str = T::schema_str();

    avro::ValidSchema schema = AvroUtils::get_or_compile_schema(schema_str);

    T obj;
    AvroUtils::convert_json_to_object_by_schema<T>(json_str, schema, obj);
    return AvroUtils::convert_to_bytes<T>(obj, bytes);
}

template <class T>
bool AvroUtils::convert_binary_to_json(const std::vector<uint8_t>& binary, std::string& str)
{
    T obj;
    AvroUtils::convert_to_object<T>(binary, obj);

    const std::string& schema_str = T::schema_str();

    avro::ValidSchema schema = AvroUtils::get_or_compile_schema(schema_str);

    return AvroUtils::convert_to_json_by_schema<T>(obj, schema, str);
}

template <class T>
bool AvroUtils::convert_to_object(const uint8_t* bytes, size_t len, T& avro_object)
{
    if (len < get_min_binary_encoded_size<T>(avro_object))
    {
        return false;
    }

    try
    {
        // Note: This is relatively efficient and the input data is not copied.
        std::auto_ptr<avro::InputStream> in(avro::memoryInputStream(bytes, len));
        avro::DecoderPtr d = avro::binaryDecoder();
        d->init(*in);
        avro::decode(*d, avro_object);
    }
    catch (const std::exception &e)
    {
        return false;
    }

    return true;
}

template <class T>
bool AvroUtils::convert_json_to_object_by_schema_str(const std::string& json_str,
                                                     const std::string& type_schema_str,
                                                     T& avro_object)
{
    avro::ValidSchema type_schema = AvroUtils::get_or_compile_schema(type_schema_str);
    return convert_json_to_object_by_schema(json_str, type_schema, avro_object);
}

template <class T>
bool AvroUtils::convert_json_to_object_by_schema(const std::string& json_str,
                                                 const avro::ValidSchema& schema,
                                                 T& avro_object)
{
    // set up decoder
    std::istringstream iss (json_str, std::istringstream::in);
    std::auto_ptr<avro::InputStream> in = avro::istreamInputStream(iss);
    avro::DecoderPtr d = avro::jsonDecoder(schema);

    // decode into the avro object
    d->init(*in);
    avro::decode(*d, avro_object);

    return true;
}

template <class T>
bool AvroUtils::convert_to_byte_stream(const T& avro_object,
                                       AvroMemoryOutputStream& memStream,
                                       size_t _buffer_size)
{
    size_t buffer_size = _buffer_size > 0 ? _buffer_size : get_estimated_binary_encoded_size<T>(avro_object);

    // we can ONLY change the chunk size if the stream hasn't been used yet.
    if (memStream.byteCount_ == 0)
        memStream.chunkSize_ = buffer_size;

    avro::EncoderPtr e = avro::binaryEncoder();
    e->init(memStream);
    avro::encode(*e, avro_object);
    //need to run flush to reclaim unused space and not transfer the entire 4096/1024 bytes
    e->flush();

    return true;
}

template <class T>
bool AvroUtils::convert_to_bytes(const T& avro_object, std::vector<uint8_t>& bytes, size_t _buffer_size)
{
    // Note that the chunk size will be adjusted in convert_to_byte_stream().
    AvroMemoryOutputStream out(AvroMemoryOutputStream::DEFAULT_CHUNK_SIZE);

    if (AvroUtils::convert_to_byte_stream(avro_object, out, _buffer_size))
    {
        out.getBuffer(bytes);
        return true;
    }

    return false;
}

template <class T>
bool AvroUtils::convert_to_json_by_schema_str(const T& avro_object,
                                              const std::string& type_schema_str,
                                              std::string& json_str)
{
    avro::ValidSchema type_schema = get_or_compile_schema(type_schema_str);
    return convert_to_json_by_schema(avro_object, type_schema, json_str);
}

template <class T>
bool AvroUtils::convert_to_json_by_schema(const T& avro_object,
                                          const avro::ValidSchema& type_schema,
                                          std::string& json_str)
{
    // set up encoder
    avro::EncoderPtr e = avro::jsonEncoder(type_schema);
    return convert_to_json_by_encoder(avro_object, e, json_str);
}

template <class T>
bool AvroUtils::convert_to_json_by_encoder(const T& avro_object,
                                           avro::EncoderPtr encoder,
                                           std::string& json_str)
{
    std::ostringstream oss (std::ostringstream::out);

    std::auto_ptr<avro::OutputStream> out = avro::ostreamOutputStream(oss);
    encoder->init(*out);
    avro::encode(*encoder, avro_object);
    encoder->flush();

    json_str = oss.str();
    return true;
}

// --------------------------------------------------------------------------
// Specializations of init_avro_message(), definitions in cpp file.

class filter_by_string_response;

template<> void AvroUtils::init_avro_message<gpudb::filter_by_string_response>(gpudb::filter_by_string_response& );


} // end namespace gpudb

// --------------------------------------------------------------------------
// Specializations of min/get_estimated_binary_encoded_size(), definitions in avro .h files.

#include "../obj_defs/obj_defs.h" // forward declarations




#endif

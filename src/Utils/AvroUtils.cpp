#include "AvroUtils.h"

#include "../obj_defs/actorobject.h"
#include "../obj_defs/actorlist.h"
#include "../obj_defs/gpudbresponse.h"
#include "../obj_defs/filterbystringresponse.h"


namespace gpudb
{

AvroUtils::map_string_to_schema AvroUtils::string_to_schema;


//static
void AvroUtils::initialize()
{
  static bool is_initialized = false;
  if (is_initialized) return;
  is_initialized = true;

  // Add the common schemas we probably need
  string_to_schema[gpudb::actor_object::schema_str()]  = compile_schema(gpudb::actor_object::schema_str());
  string_to_schema[gpudb::actor_list::schema_str()]    = compile_schema(gpudb::actor_list::schema_str());
  string_to_schema[gpudb::gpudb_response::schema_str()] = compile_schema(gpudb::gpudb_response::schema_str());
}

//static
void AvroUtils::shutdown()
{
    AvroUtils::string_to_schema.clear();
}

avro::ValidSchema AvroUtils::compile_schema(const std::string& schema_str)
{
  std::istringstream iss_req(schema_str, std::istringstream::in);
  avro::ValidSchema type_schema;
  avro::compileJsonSchema(iss_req, type_schema);
  return type_schema;
}

//static
avro::ValidSchema AvroUtils::get_or_compile_schema(const std::string& schema_str)
{
  map_string_to_schema::const_iterator it = string_to_schema.find(schema_str);
  if (it != string_to_schema.end())
    return it->second;

  avro::ValidSchema schema = compile_schema(schema_str);
  string_to_schema[schema_str] = schema;
  return schema;
}

//static
bool AvroUtils::convert_binary_to_json(const std::vector<uint8_t>& binary,
                                       const avro::ValidSchema& schema,
                                       const avro::EncoderPtr& jsonEncoder,
                                       std::string& json_str)
{
    avro::GenericDatum datum(schema);
    avro::DecoderPtr d = avro::binaryDecoder(); // cheap to create
    std::auto_ptr<avro::InputStream> in = avro::memoryInputStream(binary.data(), binary.size());
    d->init(*in);
    avro::decode(*d, datum);

    AvroUtils::convert_to_json_by_encoder(datum, jsonEncoder, json_str);
    return true;
}


// Convert the json encoded string to an avro object, then encode to binary.
//static
bool AvroUtils::convert_json_to_binary_by_schema_str( const std::string& json_str,
                                                      const std::string& type_schema_str,
                                                      std::vector<uint8_t>& bytes)
{
    try
    {
        // create the schema
        avro::ValidSchema type_schema = AvroUtils::get_or_compile_schema( type_schema_str );
        // return convert_json_to_object_by_schema(json_str, type_schema, avro_object);

        // create a generic datum given the schema
        avro::GenericDatum datum( type_schema );

        // set up decoder
        std::istringstream iss (json_str, std::istringstream::in);
        std::auto_ptr<avro::InputStream> in = avro::istreamInputStream( iss );
        avro::DecoderPtr d = avro::jsonDecoder( type_schema );

        // decode into the avro object
        d->init( *in );
        avro::decode( *d, datum );

        // Note that the chunk size will be adjusted in convert_to_byte_stream().
        AvroMemoryOutputStream out(AvroMemoryOutputStream::DEFAULT_CHUNK_SIZE);
        avro::EncoderPtr e = avro::binaryEncoder();
        e->init( out );
        avro::encode( *e, datum);
        e->flush(); // reclaim unused space

        // Copy the binary data to the output vector
        out.getBuffer( bytes );

        return true;
    }
    catch (const std::exception &excep )
    {
        return false;
    }
} // end convert_json_to_binary_by_schema_str

// --------------------------------------------------------------------------
// Specializations of init_avro_message(), declarations at bottom of .h file.

template<>
void AvroUtils::init_avro_message<gpudb::filter_by_string_response>(gpudb::filter_by_string_response& obj)
{
    obj.count = 0;
}

} // end namespace gpudb

/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef ADDOBJECTREQUEST_TEMP_H__H_
#define ADDOBJECTREQUEST_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct add_object_request {
    std::string set_id;
    std::vector<uint8_t> object_data;
    std::string object_data_str;
    std::string object_encoding;
    std::map<std::string, std::string > params;

    inline add_object_request() {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(set_id)+
            AvroUtils::get_min_binary_encoded_size(object_data)+
            AvroUtils::get_min_binary_encoded_size(object_data_str)+
            AvroUtils::get_min_binary_encoded_size(object_encoding)+
            AvroUtils::get_min_binary_encoded_size(params); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(set_id)+
            AvroUtils::get_binary_encoded_size(object_data)+
            AvroUtils::get_binary_encoded_size(object_data_str)+
            AvroUtils::get_binary_encoded_size(object_encoding)+
            AvroUtils::get_binary_encoded_size(params)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("add_object_request");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"add_object_request\",\"fields\":[{\"name\":\"set_id\",\"type\":\"string\"},{\"name\":\"object_data\",\"type\":\"bytes\"},{\"name\":\"object_data_str\",\"type\":\"string\"},{\"name\":\"object_encoding\",\"type\":\"string\"},{\"name\":\"params\",\"type\":{\"type\":\"map\",\"values\":\"string\"}}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::add_object_request> {
    static void encode(Encoder& e, const gpudb::add_object_request& v) {
        avro::encode(e, v.set_id);
        avro::encode(e, v.object_data);
        avro::encode(e, v.object_data_str);
        avro::encode(e, v.object_encoding);
        avro::encode(e, v.params);
    }
    static void decode(Decoder& d, gpudb::add_object_request& v) {
        avro::decode(d, v.set_id);
        avro::decode(d, v.object_data);
        avro::decode(d, v.object_data_str);
        avro::decode(d, v.object_encoding);
        avro::decode(d, v.params);
    }
};

}
#endif


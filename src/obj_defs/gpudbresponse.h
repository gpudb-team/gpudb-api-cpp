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


#ifndef GAIARESPONSE_TEMP_H__H_
#define GAIARESPONSE_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct gpudb_response {
    std::string status;
    std::string message;
    std::string data_type;
    std::vector<uint8_t> data;
    std::string data_str;

    inline gpudb_response() {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(status)+
            AvroUtils::get_min_binary_encoded_size(message)+
            AvroUtils::get_min_binary_encoded_size(data_type)+
            AvroUtils::get_min_binary_encoded_size(data)+
            AvroUtils::get_min_binary_encoded_size(data_str); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(status)+
            AvroUtils::get_binary_encoded_size(message)+
            AvroUtils::get_binary_encoded_size(data_type)+
            AvroUtils::get_binary_encoded_size(data)+
            AvroUtils::get_binary_encoded_size(data_str)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("gpudb_response");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"gpudb_response\",\"fields\":[{\"name\":\"status\",\"type\":\"string\"},{\"name\":\"message\",\"type\":\"string\"},{\"name\":\"data_type\",\"type\":\"string\"},{\"name\":\"data\",\"type\":\"bytes\"},{\"name\":\"data_str\",\"type\":\"string\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::gpudb_response> {
    static void encode(Encoder& e, const gpudb::gpudb_response& v) {
        avro::encode(e, v.status);
        avro::encode(e, v.message);
        avro::encode(e, v.data_type);
        avro::encode(e, v.data);
        avro::encode(e, v.data_str);
    }
    static void decode(Decoder& d, gpudb::gpudb_response& v) {
        avro::decode(d, v.status);
        avro::decode(d, v.message);
        avro::decode(d, v.data_type);
        avro::decode(d, v.data);
        avro::decode(d, v.data_str);
    }
};

}
#endif


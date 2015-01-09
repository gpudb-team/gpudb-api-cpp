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


#ifndef GETSETREQUEST_TEMP_H__H_
#define GETSETREQUEST_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct get_set_request {
    int64_t start;
    int64_t end;
    std::string set_id;
    std::string semantic_type;
    std::string user_auth_string;

    inline get_set_request()  : start(0),end(0) {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(start)+
            AvroUtils::get_min_binary_encoded_size(end)+
            AvroUtils::get_min_binary_encoded_size(set_id)+
            AvroUtils::get_min_binary_encoded_size(semantic_type)+
            AvroUtils::get_min_binary_encoded_size(user_auth_string); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(start)+
            AvroUtils::get_binary_encoded_size(end)+
            AvroUtils::get_binary_encoded_size(set_id)+
            AvroUtils::get_binary_encoded_size(semantic_type)+
            AvroUtils::get_binary_encoded_size(user_auth_string)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("get_set_request");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"get_set_request\",\"fields\":[{\"name\":\"start\",\"type\":\"long\"},{\"name\":\"end\",\"type\":\"long\"},{\"name\":\"set_id\",\"type\":\"string\"},{\"name\":\"semantic_type\",\"type\":\"string\"},{\"name\":\"user_auth_string\",\"type\":\"string\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::get_set_request> {
    static void encode(Encoder& e, const gpudb::get_set_request& v) {
        avro::encode(e, v.start);
        avro::encode(e, v.end);
        avro::encode(e, v.set_id);
        avro::encode(e, v.semantic_type);
        avro::encode(e, v.user_auth_string);
    }
    static void decode(Decoder& d, gpudb::get_set_request& v) {
        avro::decode(d, v.start);
        avro::decode(d, v.end);
        avro::decode(d, v.set_id);
        avro::decode(d, v.semantic_type);
        avro::decode(d, v.user_auth_string);
    }
};

}
#endif


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


#ifndef BOUNDINGBOXREQUEST_TEMP_H__H_
#define BOUNDINGBOXREQUEST_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct bounding_box_request {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
    std::string x_attr_name;
    std::string y_attr_name;
    std::string set_id;
    std::string result_set_id;
    std::string user_auth_string;

    inline bounding_box_request()  : min_x(0),max_x(0),min_y(0),max_y(0) {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(min_x)+
            AvroUtils::get_min_binary_encoded_size(max_x)+
            AvroUtils::get_min_binary_encoded_size(min_y)+
            AvroUtils::get_min_binary_encoded_size(max_y)+
            AvroUtils::get_min_binary_encoded_size(x_attr_name)+
            AvroUtils::get_min_binary_encoded_size(y_attr_name)+
            AvroUtils::get_min_binary_encoded_size(set_id)+
            AvroUtils::get_min_binary_encoded_size(result_set_id)+
            AvroUtils::get_min_binary_encoded_size(user_auth_string); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(min_x)+
            AvroUtils::get_binary_encoded_size(max_x)+
            AvroUtils::get_binary_encoded_size(min_y)+
            AvroUtils::get_binary_encoded_size(max_y)+
            AvroUtils::get_binary_encoded_size(x_attr_name)+
            AvroUtils::get_binary_encoded_size(y_attr_name)+
            AvroUtils::get_binary_encoded_size(set_id)+
            AvroUtils::get_binary_encoded_size(result_set_id)+
            AvroUtils::get_binary_encoded_size(user_auth_string)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("bounding_box_request");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"bounding_box_request\",\"fields\":[{\"name\":\"min_x\",\"type\":\"double\"},{\"name\":\"max_x\",\"type\":\"double\"},{\"name\":\"min_y\",\"type\":\"double\"},{\"name\":\"max_y\",\"type\":\"double\"},{\"name\":\"x_attr_name\",\"type\":\"string\"},{\"name\":\"y_attr_name\",\"type\":\"string\"},{\"name\":\"set_id\",\"type\":\"string\"},{\"name\":\"result_set_id\",\"type\":\"string\"},{\"name\":\"user_auth_string\",\"type\":\"string\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::bounding_box_request> {
    static void encode(Encoder& e, const gpudb::bounding_box_request& v) {
        avro::encode(e, v.min_x);
        avro::encode(e, v.max_x);
        avro::encode(e, v.min_y);
        avro::encode(e, v.max_y);
        avro::encode(e, v.x_attr_name);
        avro::encode(e, v.y_attr_name);
        avro::encode(e, v.set_id);
        avro::encode(e, v.result_set_id);
        avro::encode(e, v.user_auth_string);
    }
    static void decode(Decoder& d, gpudb::bounding_box_request& v) {
        avro::decode(d, v.min_x);
        avro::decode(d, v.max_x);
        avro::decode(d, v.min_y);
        avro::decode(d, v.max_y);
        avro::decode(d, v.x_attr_name);
        avro::decode(d, v.y_attr_name);
        avro::decode(d, v.set_id);
        avro::decode(d, v.result_set_id);
        avro::decode(d, v.user_auth_string);
    }
};

}
#endif


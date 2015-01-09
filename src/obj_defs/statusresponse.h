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


#ifndef STATUSRESPONSE_TEMP_H__H_
#define STATUSRESPONSE_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct status_response {
    std::string set_id;
    std::vector<std::string > set_ids;
    std::vector<bool > is_parent;
    std::vector<std::string > semantic_types;
    std::vector<std::string > labels;
    std::vector<int64_t > sizes;
    std::vector<int64_t > full_sizes;
    std::map<std::string, std::vector<std::string > > child_map;
    int64_t total_size;
    int64_t total_full_size;

    inline status_response()  : total_size(0),total_full_size(0) {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(set_id)+
            AvroUtils::get_min_binary_encoded_size(set_ids)+
            AvroUtils::get_min_binary_encoded_size(is_parent)+
            AvroUtils::get_min_binary_encoded_size(semantic_types)+
            AvroUtils::get_min_binary_encoded_size(labels)+
            AvroUtils::get_min_binary_encoded_size(sizes)+
            AvroUtils::get_min_binary_encoded_size(full_sizes)+
            AvroUtils::get_min_binary_encoded_size(child_map)+
            AvroUtils::get_min_binary_encoded_size(total_size)+
            AvroUtils::get_min_binary_encoded_size(total_full_size); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(set_id)+
            AvroUtils::get_binary_encoded_size(set_ids)+
            AvroUtils::get_binary_encoded_size(is_parent)+
            AvroUtils::get_binary_encoded_size(semantic_types)+
            AvroUtils::get_binary_encoded_size(labels)+
            AvroUtils::get_binary_encoded_size(sizes)+
            AvroUtils::get_binary_encoded_size(full_sizes)+
            AvroUtils::get_binary_encoded_size(child_map)+
            AvroUtils::get_binary_encoded_size(total_size)+
            AvroUtils::get_binary_encoded_size(total_full_size)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("status_response");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"status_response\",\"fields\":[{\"name\":\"set_id\",\"type\":\"string\"},{\"name\":\"set_ids\",\"type\":{\"type\":\"array\",\"items\":\"string\"}},{\"name\":\"is_parent\",\"type\":{\"type\":\"array\",\"items\":\"boolean\"}},{\"name\":\"semantic_types\",\"type\":{\"type\":\"array\",\"items\":\"string\"}},{\"name\":\"labels\",\"type\":{\"type\":\"array\",\"items\":\"string\"}},{\"name\":\"sizes\",\"type\":{\"type\":\"array\",\"items\":\"long\"}},{\"name\":\"full_sizes\",\"type\":{\"type\":\"array\",\"items\":\"long\"}},{\"name\":\"child_map\",\"type\":{\"type\":\"map\",\"values\":{\"type\":\"array\",\"items\":\"string\"}}},{\"name\":\"total_size\",\"type\":\"long\"},{\"name\":\"total_full_size\",\"type\":\"long\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::status_response> {
    static void encode(Encoder& e, const gpudb::status_response& v) {
        avro::encode(e, v.set_id);
        avro::encode(e, v.set_ids);
        avro::encode(e, v.is_parent);
        avro::encode(e, v.semantic_types);
        avro::encode(e, v.labels);
        avro::encode(e, v.sizes);
        avro::encode(e, v.full_sizes);
        avro::encode(e, v.child_map);
        avro::encode(e, v.total_size);
        avro::encode(e, v.total_full_size);
    }
    static void decode(Decoder& d, gpudb::status_response& v) {
        avro::decode(d, v.set_id);
        avro::decode(d, v.set_ids);
        avro::decode(d, v.is_parent);
        avro::decode(d, v.semantic_types);
        avro::decode(d, v.labels);
        avro::decode(d, v.sizes);
        avro::decode(d, v.full_sizes);
        avro::decode(d, v.child_map);
        avro::decode(d, v.total_size);
        avro::decode(d, v.total_full_size);
    }
};

}
#endif


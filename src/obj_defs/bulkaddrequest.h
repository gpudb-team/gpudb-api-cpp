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


#ifndef BULKADDREQUEST_TEMP_H__H_
#define BULKADDREQUEST_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct bulk_add_request {
    std::string set_id;
    std::vector<std::vector<uint8_t> > list;
    std::vector<std::string > list_str;
    std::string list_encoding;

    inline bulk_add_request() {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(set_id)+
            AvroUtils::get_min_binary_encoded_size(list)+
            AvroUtils::get_min_binary_encoded_size(list_str)+
            AvroUtils::get_min_binary_encoded_size(list_encoding); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(set_id)+
            AvroUtils::get_binary_encoded_size(list)+
            AvroUtils::get_binary_encoded_size(list_str)+
            AvroUtils::get_binary_encoded_size(list_encoding)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("bulk_add_request");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"bulk_add_request\",\"fields\":[{\"name\":\"set_id\",\"type\":\"string\"},{\"name\":\"list\",\"type\":{\"type\":\"array\",\"items\":\"bytes\"}},{\"name\":\"list_str\",\"type\":{\"type\":\"array\",\"items\":\"string\"}},{\"name\":\"list_encoding\",\"type\":\"string\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::bulk_add_request> {
    static void encode(Encoder& e, const gpudb::bulk_add_request& v) {
        avro::encode(e, v.set_id);
        avro::encode(e, v.list);
        avro::encode(e, v.list_str);
        avro::encode(e, v.list_encoding);
    }
    static void decode(Decoder& d, gpudb::bulk_add_request& v) {
        avro::decode(d, v.set_id);
        avro::decode(d, v.list);
        avro::decode(d, v.list_str);
        avro::decode(d, v.list_encoding);
    }
};

}
#endif


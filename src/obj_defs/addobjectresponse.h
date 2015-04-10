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


#ifndef ADDOBJECTRESPONSE_TEMP_H__H_
#define ADDOBJECTRESPONSE_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct add_object_response {
    std::string OBJECT_ID;
    int32_t count_inserted;
    int32_t count_updated;

    inline add_object_response()  : count_inserted(0),count_updated(0) {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(OBJECT_ID)+
            AvroUtils::get_min_binary_encoded_size(count_inserted)+
            AvroUtils::get_min_binary_encoded_size(count_updated); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(OBJECT_ID)+
            AvroUtils::get_binary_encoded_size(count_inserted)+
            AvroUtils::get_binary_encoded_size(count_updated)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("add_object_response");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"add_object_response\",\"fields\":[{\"name\":\"OBJECT_ID\",\"type\":\"string\"},{\"name\":\"count_inserted\",\"type\":\"int\"},{\"name\":\"count_updated\",\"type\":\"int\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::add_object_response> {
    static void encode(Encoder& e, const gpudb::add_object_response& v) {
        avro::encode(e, v.OBJECT_ID);
        avro::encode(e, v.count_inserted);
        avro::encode(e, v.count_updated);
    }
    static void decode(Decoder& d, gpudb::add_object_response& v) {
        avro::decode(d, v.OBJECT_ID);
        avro::decode(d, v.count_inserted);
        avro::decode(d, v.count_updated);
    }
};

}
#endif


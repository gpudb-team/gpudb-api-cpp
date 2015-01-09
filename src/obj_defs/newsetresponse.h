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


#ifndef NEWSETRESPONSE_TEMP_H__H_
#define NEWSETRESPONSE_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct new_set_response {
    std::string type_id;
    bool sorted;

    inline new_set_response()  : sorted(false) {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(type_id)+
            AvroUtils::get_min_binary_encoded_size(sorted); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(type_id)+
            AvroUtils::get_binary_encoded_size(sorted)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("new_set_response");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"new_set_response\",\"fields\":[{\"name\":\"type_id\",\"type\":\"string\"},{\"name\":\"sorted\",\"type\":\"boolean\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::new_set_response> {
    static void encode(Encoder& e, const gpudb::new_set_response& v) {
        avro::encode(e, v.type_id);
        avro::encode(e, v.sorted);
    }
    static void decode(Decoder& d, gpudb::new_set_response& v) {
        avro::decode(d, v.type_id);
        avro::decode(d, v.sorted);
    }
};

}
#endif


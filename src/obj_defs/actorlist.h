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


#ifndef ACTORLIST_TEMP_H__H_
#define ACTORLIST_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct actor_list {
    std::vector<std::vector<uint8_t> > list;

    inline actor_list() {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(list); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(list)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("actor_list");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"actor_list\",\"fields\":[{\"name\":\"list\",\"type\":{\"type\":\"array\",\"items\":\"bytes\"}}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::actor_list> {
    static void encode(Encoder& e, const gpudb::actor_list& v) {
        avro::encode(e, v.list);
    }
    static void decode(Decoder& d, gpudb::actor_list& v) {
        avro::decode(d, v.list);
    }
};

}
#endif


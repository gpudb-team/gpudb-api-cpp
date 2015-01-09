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


#ifndef ACTOROBJECT_TEMP_H__H_
#define ACTOROBJECT_TEMP_H__H_


#include "../Utils/AvroUtils.h"
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace gpudb {
struct actor_object {
    int32_t process_rank;
    int32_t thread_id;
    std::string actor_type;
    std::string actor_id;
    std::vector<std::string > actor_values;
    std::string machine;
    std::string address_add;
    std::string address_get;
    std::string address_get_sms;

    inline actor_object()  : process_rank(0),thread_id(0) {}

    size_t min_binary_encoded_size() const { return
            AvroUtils::get_min_binary_encoded_size(process_rank)+
            AvroUtils::get_min_binary_encoded_size(thread_id)+
            AvroUtils::get_min_binary_encoded_size(actor_type)+
            AvroUtils::get_min_binary_encoded_size(actor_id)+
            AvroUtils::get_min_binary_encoded_size(actor_values)+
            AvroUtils::get_min_binary_encoded_size(machine)+
            AvroUtils::get_min_binary_encoded_size(address_add)+
            AvroUtils::get_min_binary_encoded_size(address_get)+
            AvroUtils::get_min_binary_encoded_size(address_get_sms); }

    size_t estimated_binary_encoded_size() const { return std::max((size_t)4096,
            AvroUtils::get_binary_encoded_size(process_rank)+
            AvroUtils::get_binary_encoded_size(thread_id)+
            AvroUtils::get_binary_encoded_size(actor_type)+
            AvroUtils::get_binary_encoded_size(actor_id)+
            AvroUtils::get_binary_encoded_size(actor_values)+
            AvroUtils::get_binary_encoded_size(machine)+
            AvroUtils::get_binary_encoded_size(address_add)+
            AvroUtils::get_binary_encoded_size(address_get)+
            AvroUtils::get_binary_encoded_size(address_get_sms)+
            1024); }

    static const std::string& schema_name(void)
    {
        static const std::string str("actor_object");
        return str;
    }
    static const std::string& schema_str(void)
    {
        static const std::string str("{\"type\":\"record\",\"name\":\"actor_object\",\"fields\":[{\"name\":\"process_rank\",\"type\":\"int\"},{\"name\":\"thread_id\",\"type\":\"int\"},{\"name\":\"actor_type\",\"type\":\"string\"},{\"name\":\"actor_id\",\"type\":\"string\"},{\"name\":\"actor_values\",\"type\":{\"type\":\"array\",\"items\":\"string\"}},{\"name\":\"machine\",\"type\":\"string\"},{\"name\":\"address_add\",\"type\":\"string\"},{\"name\":\"address_get\",\"type\":\"string\"},{\"name\":\"address_get_sms\",\"type\":\"string\"}]}");
        return str;
    }

};

}
namespace avro {
template<> struct codec_traits<gpudb::actor_object> {
    static void encode(Encoder& e, const gpudb::actor_object& v) {
        avro::encode(e, v.process_rank);
        avro::encode(e, v.thread_id);
        avro::encode(e, v.actor_type);
        avro::encode(e, v.actor_id);
        avro::encode(e, v.actor_values);
        avro::encode(e, v.machine);
        avro::encode(e, v.address_add);
        avro::encode(e, v.address_get);
        avro::encode(e, v.address_get_sms);
    }
    static void decode(Decoder& d, gpudb::actor_object& v) {
        avro::decode(d, v.process_rank);
        avro::decode(d, v.thread_id);
        avro::decode(d, v.actor_type);
        avro::decode(d, v.actor_id);
        avro::decode(d, v.actor_values);
        avro::decode(d, v.machine);
        avro::decode(d, v.address_add);
        avro::decode(d, v.address_get);
        avro::decode(d, v.address_get_sms);
    }
};

}
#endif


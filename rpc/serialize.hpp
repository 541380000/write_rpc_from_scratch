//
// Created by User on 2022/2/24.
//

#ifndef RPC_SERIALIZE_HPP
#define RPC_SERIALIZE_HPP
#define DEFAULT_INIT_SERIALIZE_BUFFER_SIZE 100   // bytes

#include <vector>
#include <iostream>
#include <memory>
#include "rpc_header.hpp"
using namespace std;
using byte=char;

struct SerBuffer{
    vector<byte> buffer;
};

/**
 * @brief create new SerBuffer with init size
 * @param buffer_size: init buffer size. enlarging it will avoid memory copy of vector, but use more memory.
 * @return unique_ptr to created buffer
 */
unique_ptr<SerBuffer> creatSerBuffer(const uint32_t buffer_size=DEFAULT_INIT_SERIALIZE_BUFFER_SIZE){
    auto ptr = make_unique<SerBuffer>();
    ptr->buffer.reserve(buffer_size);
    return move(ptr);
}


/**
 * @brief copy nBytes of data to SerBuffer
 * @param buffer: unique ptr to SerBuffer
 * @param data: data to copy
 * @param nBytes: how many bytes to copy
 */
void serializeData(const unique_ptr<SerBuffer>& buffer, const byte* data, const uint32_t nBytes){
    copy(data, data+nBytes, back_inserter(buffer->buffer));
}

/**
 *
 * @param buffer
 * @param nBytes: it can be positive(to move forward) and negative(to move backward)
 */
void serializeBufferSkip(const unique_ptr<SerBuffer>& buffer, int32_t nBytes){
    if (nBytes < 0)
        while(nBytes < 0) buffer->buffer.pop_back(), nBytes ++;
    else if (nBytes > 0)
        while(nBytes > 0) buffer->buffer.emplace_back('0'), nBytes --;
}

void resetSerBuffer(const unique_ptr<SerBuffer>& buffer){
    buffer->buffer.clear();
}

void fillNull(const unique_ptr<SerBuffer>& buffer){
    uint32_t sentinel = 0xFFFFFFFF;
    serializeData(buffer, (byte*)&sentinel, 4);
}

void serializeRPCHeader(const unique_ptr<SerBuffer>& buffer, const RPCHeader* const header){
    serializeData(buffer, (char*)header, sizeof(header));
}


#endif //RPC_SERIALIZE_HPP

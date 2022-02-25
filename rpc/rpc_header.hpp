//
// Created by User on 2022/2/24.
//

#ifndef RPC_RPC_HEADER_HPP
#define RPC_RPC_HEADER_HPP

#include <ostream>

struct RPCHeader{
    uint32_t id;    // id of procedure
    friend std::ostream &operator<<(std::ostream &os, const RPCHeader &header) {
        os << "id: " << header.id << " size: " << header.size;
        return os;
    }

    uint32_t size;  // size of payload(argument)
};

#endif //RPC_RPC_HEADER_HPP

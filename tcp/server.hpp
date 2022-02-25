//
// Created by User on 2022/2/24.
//

#ifndef RPC_SERVER_HPP
#define RPC_SERVER_HPP

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include "../rpc/serialize.hpp"
#include "../rpc/rpc_header.hpp"
#include "../business/SimplePerson.hpp"
#include "tcp_util.hpp"


void handle_rpc(int connfd)
{

    char buff[BUFFER_SIZE];
    RPCHeader header{};
    for (;;) {
        bzero(buff, BUFFER_SIZE);
        if (read_n_bytes(connfd, buff, sizeof(RPCHeader))){
            printf("Error reading nbytes header\n");
            exit(-1);
        }

        memcpy(&header, buff, sizeof(RPCHeader));
        printf("Get header: id: %d\t\tpayload: %d\n", header.id, header.size);

        bzero(buff, BUFFER_SIZE);
        if (read_n_bytes(connfd, buff, header.size)) {
            printf("Error reading nbytes payload\n");
            exit(-1);
        }

        auto buffer = make_unique<SerBuffer>();
        buffer->buffer.assign(buff, buff + header.size);
        cout << "Get header: " << header << endl;
        switch (header.id) {
            // get merry of two person
            case 1:{
                auto p1 = deserialize_simple_person(buffer);
                auto p2 = deserialize_simple_person(buffer, header.size / 2);

                auto res = string(p1->name) + string(" -Merry- ") + string(p2->name);
                auto resp_buf = make_unique<SerBuffer>();
                SimplePerson p{};
                memcpy(p.name, res.c_str(), res.length());
                p.age = max(p1->age, p2->age);
                p.weight = min(p1->weight, p2->weight);
                RPCHeader resp_header{100+header.id, sizeof(p.weight) + sizeof(p.name) + sizeof(p.age)};
                serializeRPCHeader(resp_buf, &resp_header);
                serialize_simple_person(&p, resp_buf);
                // send it back
                if (send(connfd, resp_buf->buffer.data(), resp_buf->buffer.size(), 0) == -1){
                    return ;
                }
                break;
            }
            // kill one person
            case 2:{
                auto p1 = deserialize_simple_person(buffer);
                SimplePerson p{"killed", p1->age, p1->weight};
                RPCHeader resp_header{100+header.id, sizeof(p.weight) + sizeof(p.name) + sizeof(p.age)};
                auto resp_buf = make_unique<SerBuffer>();
                serializeRPCHeader(resp_buf, &resp_header);
                serialize_simple_person(&p, resp_buf);
                // send it back
                if (send(connfd, resp_buf->buffer.data(), resp_buf->buffer.size(), 0) == -1){
                    return ;
                }
                break;
            }
            default:{
                printf("Invalid rpc id: %d\n", header.id);
                break;
            }
        }

    }
}

// Driver function
int rpc_server_run()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli{};

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    while(true) {
        // Now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0) {
            printf("Listen failed...\n");
            exit(0);
        } else
            printf("Server listening..\n");
        len = sizeof(cli);

        connfd = accept(sockfd, (SA *) &cli, reinterpret_cast<socklen_t *>(&len));
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(0);
        }
        handle_rpc(connfd);
        close(sockfd);
    }
}

#endif //RPC_SERVER_HPP

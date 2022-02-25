//
// Created by User on 2022/2/25.
//

#ifndef RPC_CLIENT_HPP
#define RPC_CLIENT_HPP
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../rpc/rpc_header.hpp"
#include "../rpc/serialize.hpp"
#include "tcp_util.hpp"
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[BUFFER_SIZE];
    int n;
    bzero(buff, sizeof(buff));
    /**
     * to run rpc, we will follow the steps:
     * 1. serialize RPC header to a buffer
     * 2. serialize RPC argument to a buffer
     * 3. send RPC package through socket
     * 4. receive the response
     * 5. deserialize RPC package
     */
    // 1. serialize RPC header to a buffer
    RPCHeader rpcHeader{};
    rpcHeader.id = 1;
    // TODO: size will be assigned after argument serialize

    // 2. serialize RPC argument to a buffer
    SimplePerson p1{"Tony", 23, 160};
    SimplePerson p2{"Jenny", 21, 100};


    auto argumentBuffer = make_unique<SerBuffer>();
    serialize_simple_person(&p1, argumentBuffer);
    serialize_simple_person(&p2, argumentBuffer);
    rpcHeader.size = argumentBuffer->buffer.size();

    // combine 1 & 2
    auto buffer = make_unique<SerBuffer>();
    serializeRPCHeader(buffer, &rpcHeader);
    copy(argumentBuffer->buffer.begin(), argumentBuffer->buffer.end(), back_inserter(buffer->buffer));

    cout << "send request -> p1: " << p1 << "\t\tp2: " << p2 << endl;
    // cout << "buf sent: " << buffer->buffer.size() << endl;
    send(sockfd, buffer->buffer.data(), buffer->buffer.size(), 0);

    rpcHeader = {};
    read_n_bytes(sockfd, (char*)&rpcHeader, sizeof(rpcHeader));
    // printf("From Server : %s", buff);
    read_n_bytes(sockfd, buff, rpcHeader.size);
    resetSerBuffer(argumentBuffer);
    copy(buff, buff+rpcHeader.size, back_inserter(argumentBuffer->buffer));
    auto resp = deserialize_simple_person(argumentBuffer, 0);

    cout << "Get response -> " << *resp << endl;
}


int start_client()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
#endif //RPC_CLIENT_HPP

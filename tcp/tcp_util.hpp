//
// Created by User on 2022/2/25.
//

#ifndef RPC_TCP_UTIL_HPP
#define RPC_TCP_UTIL_HPP

#define BUFFER_SIZE (1024*8)
#define PORT 8080
#define SA struct sockaddr


int read_n_bytes(int connfd, char* buffer, int n_bytes){
    int tmp = 0;
    int cnt = 0;
    while (cnt < n_bytes) {
        if ((tmp = read(connfd, buffer + cnt, n_bytes - cnt)) == -1) {cout << "failed reading" << endl;return -1;}
        cnt += tmp;
    }

    return 0;
}



#endif //RPC_TCP_UTIL_HPP

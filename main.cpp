#include <iostream>
#include <memory>
#include <vector>
#include <cstring>
#include "rpc/serialize.hpp"
#include "business/SimplePerson.hpp"
#include "business/NestedPerson.hpp"
#include "business/PointerPerson.hpp"
#include "tcp/server.hpp"
#include "tcp/client.hpp"
using namespace std;


int main() {
    if (fork() == 0) {
        rpc_server_run();
    }
    else{
        sleep(2);
        start_client();
    }
}




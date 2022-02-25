//
// Created by User on 2022/2/24.
//

#ifndef RPC_SIMPLEPERSON_HPP
#define RPC_SIMPLEPERSON_HPP

#include <iostream>
#include <memory>
#include <algorithm>
#include "../rpc/serialize.hpp"
using namespace std;
using byte=char;

struct SimplePerson{
    byte name[30];
    uint32_t age;
    uint32_t weight;

    friend ostream &operator<<(ostream &os, const SimplePerson &person) {
        os << "name: " << person.name << " age: " << person.age << " weight: " << person.weight;
        return os;
    }
};


void serialize_simple_person(const SimplePerson* const obj, const unique_ptr<SerBuffer>& buffer){
    if(!obj){
        fillNull(buffer);
        return ;
    }
    serializeData(buffer, obj->name, sizeof(obj->name));
    serializeData(buffer, (char*)&obj->age, sizeof(obj->age));
    serializeData(buffer, (char*)&obj->weight, sizeof(obj->weight));
}

unique_ptr<SimplePerson> deserialize_simple_person(const unique_ptr<SerBuffer>& buffer, uint32_t offset = 0){
    if (*(uint32_t*)buffer->buffer.data() == 0xFFFFFFFF)
        return {nullptr};

    auto ptr = make_unique<SimplePerson>();
    auto& person = *ptr;
    copy(buffer->buffer.begin() + offset, buffer->buffer.begin() + sizeof(person) + offset, person.name);
    offset += sizeof(person.name);

    person.age = *(decltype(person.age)*)(buffer->buffer.data() + offset);
    offset += sizeof(person.age);

    person.weight = *(decltype(person.weight)*)(buffer->buffer.data() + offset);
    offset += sizeof(person.weight);

    return move(ptr);
}


void testSimplePerson(){
    SimplePerson p{"1234", 1, 1};
    auto buf = make_unique<SerBuffer>();
    printf("-------------------> SimplePerson\n");
    serialize_simple_person(&p, buf);
    printf("Full size of person after serialize: %zu, expected is %d\n", buf->buffer.size(), 30+4+4);
    cout << "Before  serialize: " << p << "\nAfter deserialize: " << *deserialize_simple_person(buf) << endl << endl;
}

#endif //RPC_SIMPLEPERSON_HPP

//
// Created by User on 2022/2/24.
//

#ifndef RPC_NESTEDPERSON_HPP
#define RPC_NESTEDPERSON_HPP
#include "../rpc/serialize.hpp"
#include <iostream>
#include <memory>
#include "occupation.h"

using namespace std;
using byte = char;

struct NestedPerson{
    char name[30];
    Occupation occupation;
    uint32_t age;
    uint32_t weight;
    friend ostream &operator<<(ostream &os, const NestedPerson &person) {
        os << "name: " << person.name << " occupation: " << person.occupation << " age: " << person.age << " weight: "
           << person.weight;
        return os;
    }

};


void serialize_nested_person(const NestedPerson* const obj, const unique_ptr<SerBuffer>& buffer){
    if (!obj){
        fillNull(buffer);
        return ;
    }
    serializeData(buffer, obj->name, sizeof(obj->name));
    serialize_simple_occupation(&obj->occupation, buffer);
    serializeData(buffer, (char*)&obj->age, sizeof(obj->age));
    serializeData(buffer, (char*)&obj->weight, sizeof(obj->weight));
}

unique_ptr<NestedPerson> deserialize_nested_person(const unique_ptr<SerBuffer>& buffer, uint32_t offset = 0){
    if (*(uint32_t*)buffer->buffer.data() == 0xFFFFFFFF)
        return {nullptr};

    auto ptr = make_unique<NestedPerson>();
    auto& person = *ptr;
    copy(buffer->buffer.begin() + offset, buffer->buffer.begin() + sizeof(person) + offset, person.name);
    offset += sizeof(person.name);

    person.occupation = *deserialize_simple_occupation(buffer, offset);
    offset += sizeof(person.occupation.title) + sizeof(person.occupation.id);

    person.age = *(decltype(person.age)*)(buffer->buffer.data() + offset);
    offset += sizeof(person.age);

    person.weight = *(decltype(person.weight)*)(buffer->buffer.data() + offset);
    offset += sizeof(person.weight);

    return move(ptr);
}


void testNestedPerson(){
    NestedPerson p{"1234", {"doctor", 294}, 1, 1};
    auto buf = make_unique<SerBuffer>();
    printf("-------------------> NestedPerson\n");
    serialize_nested_person(&p, buf);
    printf("Full size of nested person after serialize: %zu, expected is %d\n", buf->buffer.size(), 30+4+4+30+4);
    cout << "Before  serialize: " << p << "\nAfter deserialize: " << *deserialize_nested_person(buf) << endl << endl;
}





#endif //RPC_NESTEDPERSON_HPP

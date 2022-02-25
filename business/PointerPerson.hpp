//
// Created by User on 2022/2/24.
//

#ifndef RPC_POINTERPERSON_HPP
#define RPC_POINTERPERSON_HPP
#include "../rpc/serialize.hpp"
#include <iostream>
#include <memory>
#include "occupation.h"

using namespace std;
using byte = char;

struct PointerPerson{
    char name[30];
    Occupation* occupation;
    uint32_t age;
    uint32_t weight;

    friend ostream &operator<<(ostream &os, const PointerPerson &person) {
        os << "name: " << person.name << " occupation: " << *person.occupation << " age: " << person.age << " weight: "
           << person.weight;
        return os;
    }
};


void serialize_pointer_person(const PointerPerson* const obj, const unique_ptr<SerBuffer>& buffer){
    if (!obj){
        fillNull(buffer);
        return ;
    }
    serializeData(buffer, obj->name, sizeof(obj->name));
    serialize_simple_occupation(obj->occupation, buffer);
    serializeData(buffer, (char*)&obj->age, sizeof(obj->age));
    serializeData(buffer, (char*)&obj->weight, sizeof(obj->weight));
}

unique_ptr<PointerPerson> deserialize_pointer_person(const unique_ptr<SerBuffer>& buffer, uint32_t offset = 0){
    if (*(uint32_t*)buffer->buffer.data() == 0xFFFFFFFF)
        return {nullptr};

    auto ptr = make_unique<PointerPerson>();
    auto& person = *ptr;
    copy(buffer->buffer.begin() + offset, buffer->buffer.begin() + sizeof(person) + offset, person.name);
    offset += sizeof(person.name);

    person.occupation = deserialize_simple_occupation(buffer, offset).release();
    offset += sizeof(person.occupation->title) + sizeof(person.occupation->id);

    person.age = *(decltype(person.age)*)(buffer->buffer.data() + offset);
    offset += sizeof(person.age);

    person.weight = *(decltype(person.weight)*)(buffer->buffer.data() + offset);
    offset += sizeof(person.weight);

    return move(ptr);
}





void testPointerPerson(){
    Occupation occ{"doctor", 294};
    PointerPerson p{"1234", &occ, 1, 1};
    auto buf = make_unique<SerBuffer>();
    printf("-------------------> PointerPerson\n");
    serialize_pointer_person(&p, buf);
    printf("Full size of pointer person after serialize: %zu, expected is %d\n", buf->buffer.size(), 30+4+4+30+4);
    cout << "Before  serialize: " << p << "\nAfter deserialize: " << *deserialize_pointer_person(buf) << endl << endl;
}


#endif //RPC_POINTERPERSON_HPP

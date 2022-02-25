//
// Created by User on 2022/2/24.
//

#ifndef RPC_OCCUPATION_H
#define RPC_OCCUPATION_H

#include <ostream>

struct Occupation{
    char title[30];
    uint32_t id;

    friend ostream &operator<<(ostream &os, const Occupation &occupation) {
        os << "title: " << occupation.title << " id: " << occupation.id;
        return os;
    }
};

void serialize_simple_occupation(const Occupation* const obj, const unique_ptr<SerBuffer>& buffer){
    serializeData(buffer, obj->title, sizeof(obj->title));
    serializeData(buffer, (char*)&obj->id, sizeof(obj->id));
}

unique_ptr<Occupation> deserialize_simple_occupation(const unique_ptr<SerBuffer>& buffer, uint32_t offset = 0){
    auto ptr = make_unique<Occupation>();
    auto& occupation = *ptr;
    copy(buffer->buffer.begin() + offset, buffer->buffer.begin() + offset + sizeof(occupation.title), occupation.title);
    offset += sizeof(occupation.title);

    occupation.id = *(decltype(occupation.id)*)(buffer->buffer.data() + offset);

    return move(ptr);
}

#endif //RPC_OCCUPATION_H

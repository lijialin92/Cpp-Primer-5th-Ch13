//
// Created by lijialin on 2020/6/22.
//

#ifndef CHAPTER13_STRVEC_H
#define CHAPTER13_STRVEC_H

#include <memory>
#include <utility>

class StrVec{
public:
    StrVec() : element(nullptr), firstFree(nullptr), cap(nullptr) {};
    StrVec(const StrVec&);
    StrVec& operator=(StrVec&);
    void push_back(const std::string&);
    ~StrVec() {free();}

    size_t size(){ return firstFree - element;}
    size_t capacity() {return cap - element;}
private:
    void free();
    void ifAllocate();
    void reallocate();
    static std::allocator<std::string> alloc;
    std::string* element;
    std::string* firstFree;
    std::string* cap;
};

void StrVec::free(){
    for(auto i = element; i != firstFree; i++)
        alloc.destroy(i);
    alloc.deallocate(element, size());
}

void StrVec::ifAllocate() {
    if(size() == capacity())
        reallocate();
};

void StrVec::reallocate(){
    auto newcap = size();
    if(newcap)
        newcap = size() * 2;
    else
        newcap = 1;

    std::string* newelement = alloc.allocate(newcap);
    auto dest = newelement;
    auto ele = element;
    for(auto i = element; i != firstFree; i++)
        alloc.construct(dest++, std::move(*ele++));
    element = newelement;
    firstFree = dest;
    cap = element + newcap;
}

void StrVec::push_back(const std::string& str){
    ifAllocate();
    alloc.construct(firstFree++, str);
}

StrVec::StrVec(const StrVec& strvec) {
    element = alloc.allocate(strvec.firstFree - strvec.element);
    firstFree = cap = std::uninitialized_copy(strvec.element, strvec.firstFree, element);
}
#endif //CHAPTER13_STRVEC_H

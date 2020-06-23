//
// Created by lijialin on 2020/6/22.
//

#ifndef CHAPTER13_STRVEC_H
#define CHAPTER13_STRVEC_H

#include <utility>

class StrVec{
public:
    StrVec() : element(nullptr), firstFree(nullptr), cap(nullptr) {};
    StrVec(std::initializer_list<std::string>);
    StrVec(const StrVec&);
    StrVec& operator=(const StrVec&);
    void push_back(const std::string&);
    ~StrVec() {free();}

    size_t size(){ return firstFree - element;}
    size_t capacity() {return cap - element;}
    std::string* begin() { return element;};
    std::string* end() { return firstFree;};

    void reverse(size_t);
    void reseize(size_t);
private:
    void free();
    void ifAllocate();
    void reallocate();
    void reallocate(size_t);
    static std::allocator<std::string> alloc;
    std::string* element;
    std::string* firstFree;
    std::string* cap;
};

/*void StrVec::free(){
    while(firstFree != element)
        alloc.destroy(--firstFree);
    alloc.deallocate(element, size());
}*/

void StrVec::free(){
    for_each(element, firstFree, [](std::string& s){alloc.destroy(&s);});
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

void StrVec::reallocate(size_t n) {
    auto newelement = alloc.allocate(n);
    auto dest = newelement;
    auto ele = element;
    for(auto i = element; i != firstFree; i++)
        alloc.construct(dest++, std::move(*ele++));
    element = newelement;
    firstFree = dest;
    cap = element + n;
}

void StrVec::push_back(const std::string& str){
    ifAllocate();
    alloc.construct(firstFree++, str);
}

StrVec::StrVec(const StrVec& strvec) {
    element = alloc.allocate(strvec.firstFree - strvec.element);
    firstFree = cap = std::uninitialized_copy(strvec.element, strvec.firstFree, element);
}

StrVec& StrVec::operator=(const StrVec& strvec){
    free();
    element = alloc.allocate(strvec.firstFree - strvec.element);
    firstFree = cap = std::uninitialized_copy(strvec.element, strvec.firstFree, element);
    return *this;
}

void StrVec::reverse(size_t n) {
    if(n > capacity())
        reallocate(n);
}

void StrVec::reseize(size_t n) {
    if(n > size())
    {
        while(size() < n)
            push_back("");
    }
    else if(n < size())
    {
        while(size() > n)
            alloc.destroy(--firstFree);
    }
}

StrVec::StrVec(std::initializer_list<std::string> il) {
    auto element = alloc.allocate(il.size());
    firstFree = cap = std::uninitialized_copy(il.begin(), il.end(), element);
}


#endif //CHAPTER13_STRVEC_H

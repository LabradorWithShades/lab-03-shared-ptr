#include "SharedPtr.hpp"

#include <iostream>

int main() {
    SharedPtr<int> ptr1;
    const SharedPtr<int> ptr2 = SharedPtr<int>::make_shared(1);
    ptr1 = ptr2;
    std::cout << ptr1.use_count();


    return 0;
}
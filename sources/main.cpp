// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com

#include "SharedPtr.hpp"
#include <iostream>

template<typename T>
SharedPtr<T> testFunc(SharedPtr<T> sharedPtr) {
  return SharedPtr<T>(sharedPtr);
}

static int DestructorCallCount = 0;

struct Example {
  Example(int x, int y) {
    m_x = x; m_y = y;
  }
  ~Example() {
      ++DestructorCallCount;
      std::cout << "OBJECT (" << m_x << ", " << m_y << ") ";
      std::cout << "DESTRUCTED!" << std::endl;
  }
  int m_x = 0, m_y = 0;
};

int main() {
    //1. Creating pointer with nothing to point to
    {
        std::cout << "= Part 1 ============================" << std::endl;
        SharedPtr<int> ptr;
        std::cout << ptr.use_count() << std::endl;  // 0
        if (!ptr)
          std::cout << "Pointer does not point to anything - OK" << std::endl;
    }
    //2. Creating a pointer using each of the 3 remaining constructors
    {
        DestructorCallCount = 0;
        std::cout << "= Part 2 ============================" << std::endl;
        int* p_x = new int(5);
        SharedPtr<int> ptr1(p_x);
        if (ptr1)
            std::cout << "Pointer 1 does point to something - OK" << std::endl;
        std::cout << ptr1.use_count() << std::endl;  // 1
        std::cout << *ptr1.get() << " = " << *ptr1 << std::endl; //5 = 5
        SharedPtr<int> ptr2(ptr1);
        if (ptr1)
          std::cout << "Pointer 1 does point to something - OK" << std::endl;
        std::cout << ptr1.use_count() << std::endl;  // 2
        std::cout << *ptr1.get() << " = " << *ptr1 << std::endl; //5 = 5
        if (ptr2)
            std::cout << "Pointer 2 does point to something - OK" << std::endl;
        std::cout << ptr2.use_count() << std::endl;  // 2
        std::cout << *ptr2.get() << " = " << *ptr2 << std::endl; //5 = 5
        SharedPtr<int> ptr3(testFunc(ptr1));
        if (ptr1)
            std::cout << "Pointer 1 does point to something - OK" << std::endl;
        std::cout << ptr1.use_count() << std::endl;  // 3
        std::cout << *ptr1.get() << " = " << *ptr1 << std::endl; //5 = 5
        if (ptr2)
            std::cout << "Pointer 2 does point to something - OK" << std::endl;
        std::cout << ptr2.use_count() << std::endl;  // 3
        std::cout << *ptr2.get() << " = " << *ptr2 << std::endl; //5 = 5
        if (ptr3)
            std::cout << "Pointer 3 does point to something - OK" << std::endl;
        std::cout << ptr3.use_count() << std::endl;  // 3
        std::cout << *ptr3.get() << " = " << *ptr3 << std::endl; //5 = 5
    }
    //3. Using make_shared() with something more complex
    {
        std::cout << "= Part 3 ============================" << std::endl;
        SharedPtr<Example> ptr = SharedPtr<Example>::make_shared(3, 4);
        if (ptr)
            std::cout << "Pointer does point to something - OK" << std::endl;
        std::cout << ptr.use_count() << std::endl; // 1
        std::cout << "(" << ptr->m_x << ", " << ptr->m_y << ")"
                  << std::endl;//(3, 4)

        SharedPtr<Example> ptr_copy = ptr;
        if (ptr)
            std::cout << "Pointer does point to something - OK" << std::endl;
        std::cout << ptr.use_count() << std::endl; // 2
        std::cout << "(" << ptr->m_x << "," << ptr->m_y << ")"
                  << std::endl;//(3, 4)
        if (ptr_copy)
            std::cout << "Pointer 2 does point to something - OK" << std::endl;
        std::cout << ptr_copy.use_count() << std::endl; // 2
        std::cout << "(" << ptr_copy->m_x << ", " << ptr_copy->m_y << ")"
                  << std::endl;//(3, 4)

        SharedPtr<Example> ptr_move = testFunc(ptr);
        if (ptr)
            std::cout << "Pointer does point to something - OK" << std::endl;
        std::cout << ptr.use_count() << std::endl; // 3
        std::cout << "(" << ptr->m_x << "," << ptr->m_y << ")" << std::endl;//(3,4)
        if (ptr_copy)
            std::cout << "Pointer 2 does point to something - OK" << std::endl;
        std::cout << ptr_copy.use_count() << std::endl; // 3
        std::cout << "(" << ptr_copy->m_x << ", " << ptr_copy->m_y << ")"
                  << std::endl;//(3, 4)
        if (ptr_move)
            std::cout << "Pointer 3 does point to something - OK" << std::endl;
        std::cout << ptr.use_count() << std::endl; // 3
        std::cout << "(" << ptr_move->m_x << ", " << ptr_move->m_y << ")"
                  << std::endl;//(3, 4)
    }

    //4. Swapping
    {
        std::cout << "= Part 4 ============================" << std::endl;
        SharedPtr<Example> ptr1_1 = SharedPtr<Example>::make_shared(3, 4);
        SharedPtr<Example> ptr1_2 = ptr1_1;
        SharedPtr<Example> ptr1_3 = ptr1_2;
        std::cout << "(" << ptr1_1->m_x << ", " << ptr1_1->m_y << ")"
                  << "(" << ptr1_2->m_x << ", " << ptr1_2->m_y << ")"
                  << "(" << ptr1_3->m_x << ", " << ptr1_3->m_y << ")"
                  << std::endl;// (3, 4)(3, 4)(3, 4)
        std::cout << ptr1_1.use_count() << " = "
                  << ptr1_2.use_count() << " = "
                  << ptr1_3.use_count() << std::endl;//3 = 3 = 3

        SharedPtr<Example> ptr2_1 = SharedPtr<Example>::make_shared(20, 5);
        SharedPtr<Example> ptr2_2 = ptr2_1;
        std::cout << "(" << ptr2_1->m_x << ", " << ptr2_1->m_y << ")"
                  << "(" << ptr2_2->m_x << ", " << ptr2_2->m_y << ")"
                  << std::endl;// (20, 5)(20, 5)
        std::cout << ptr2_1.use_count() << " = "
                  << ptr2_2.use_count() << std::endl;//2 = 2

        ptr1_1.swap(ptr2_1);
        std::cout << "(" << ptr1_1->m_x << ", " << ptr1_1->m_y << ")"
                  << "(" << ptr2_1->m_x << ", " << ptr2_1->m_y << ")"
                  << std::endl;// (20, 5)(3, 4)
        std::cout << ptr1_1.use_count() << " != "
                  << ptr2_1.use_count() << std::endl;//2 != 3
    }

    //5. Object life-time
    {
        std::cout << "= Part 5 ============================" << std::endl;
        SharedPtr<Example> ptr = SharedPtr<Example>::make_shared(3, 4);
        {
            SharedPtr<Example> ptr_tmp1 = ptr;
            {
                SharedPtr<Example> ptr_tmp2 = ptr;
            }
        }
        //Destructor should not be called
        SharedPtr<Example> ptr_tmp = ptr;
    }
    //Destructor should be called (once)
    return 0;
}
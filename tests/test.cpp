// Copyright 2020 Your Name <your_email>

#include "SharedPtr.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(Example, EmptyTest) {
    EXPECT_TRUE(true);
}

TEST(Basics, DefaultConstructor) {
    SharedPtr<int> ptr;
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(!ptr, true);
}

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
  }
  int m_x = 0, m_y = 0;
};

TEST(Basics, AdvancedConstructors) {
    int* p_x = new int(5);
    SharedPtr<int> ptr1(p_x);
    EXPECT_EQ(!ptr1, false);
    EXPECT_EQ(ptr1.use_count(), 1);
    EXPECT_EQ(*ptr1.get(), *ptr1);
    EXPECT_EQ(*ptr1, 5);
    SharedPtr<int> ptr2(ptr1);
    EXPECT_EQ(!ptr1, false);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(*ptr1.get(), *ptr1);
    EXPECT_EQ(*ptr1, 5);
    EXPECT_EQ(!ptr2, false);
    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_EQ(*ptr2.get(), *ptr2);
    EXPECT_EQ(*ptr2, 5);
    SharedPtr<int> ptr3(testFunc(ptr1));
    EXPECT_EQ(!ptr1, false);
    EXPECT_EQ(ptr1.use_count(), 3);
    EXPECT_EQ(*ptr1.get(), *ptr1);
    EXPECT_EQ(*ptr1, 5);
    EXPECT_EQ(!ptr2, false);
    EXPECT_EQ(ptr2.use_count(), 3);
    EXPECT_EQ(*ptr2.get(), *ptr2);
    EXPECT_EQ(*ptr2, 5);
    EXPECT_EQ(!ptr3, false);
    EXPECT_EQ(ptr3.use_count(), 3);
    EXPECT_EQ(*ptr3.get(), *ptr3);
    EXPECT_EQ(*ptr3, 5);
}

TEST(Basics, ObjectLifetime) {
    DestructorCallCount = 0;
    {
        SharedPtr<Example> ptr = SharedPtr<Example>::make_shared(3, 4);
        {
            SharedPtr<Example> ptr_tmp1 = ptr;
            {
                SharedPtr<Example> ptr_tmp2 = ptr;
                EXPECT_EQ(DestructorCallCount, 0);
            }
            EXPECT_EQ(DestructorCallCount, 0);
        }
        SharedPtr<Example> ptr_tmp = ptr;
        EXPECT_EQ(DestructorCallCount, 0);
    }
    EXPECT_EQ(DestructorCallCount, 1);
}

TEST(Advanced, MakeShared) {
    std::stringstream out_str;

    SharedPtr<Example> ptr = SharedPtr<Example>::make_shared(3, 4);
    EXPECT_EQ(!ptr, false);
    EXPECT_EQ(ptr.use_count(), 1);
    out_str << "(" << ptr->m_x << ", " << ptr->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)");
    out_str.str("");

    SharedPtr<Example> ptr_copy = ptr;
    EXPECT_EQ(!ptr, false);
    EXPECT_EQ(ptr.use_count(), 2);
    out_str << "(" << ptr->m_x << ", " << ptr->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)");
    out_str.str("");

    EXPECT_EQ(!ptr_copy, false);
    EXPECT_EQ(ptr_copy.use_count(), 2);
    out_str << "(" << ptr_copy->m_x << ", " << ptr_copy->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)");
    out_str.str("");

    SharedPtr<Example> ptr_move = testFunc(ptr);
    EXPECT_EQ(!ptr, false);
    EXPECT_EQ(ptr.use_count(), 3);
    out_str << "(" << ptr->m_x << ", " << ptr->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)");
    out_str.str("");

    EXPECT_EQ(!ptr_copy, false);
    EXPECT_EQ(ptr_copy.use_count(), 3);
    out_str << "(" << ptr_copy->m_x << ", " << ptr_copy->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)");
    out_str.str("");

    EXPECT_EQ(!ptr_move, false);
    EXPECT_EQ(ptr_move.use_count(), 3);
    out_str << "(" << ptr_move->m_x << ", " << ptr_move->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)");
}

TEST(Advanced, Swapping) {
    std::stringstream out_str;

    SharedPtr<Example> ptr1_1 = SharedPtr<Example>::make_shared(3, 4);
    SharedPtr<Example> ptr1_2 = ptr1_1;
    SharedPtr<Example> ptr1_3 = ptr1_2;
    out_str << "(" << ptr1_1->m_x << ", " << ptr1_1->m_y << ")"
            << "(" << ptr1_2->m_x << ", " << ptr1_2->m_y << ")"
            << "(" << ptr1_3->m_x << ", " << ptr1_3->m_y << ")";
    EXPECT_EQ(out_str.str(), "(3, 4)(3, 4)(3, 4)");
    out_str.str("");
    out_str << ptr1_1.use_count() << " = "
            << ptr1_2.use_count() << " = "
            << ptr1_3.use_count();
    EXPECT_EQ(out_str.str(), "3 = 3 = 3");
    out_str.str("");

    SharedPtr<Example> ptr2_1 = SharedPtr<Example>::make_shared(20, 5);
    SharedPtr<Example> ptr2_2 = ptr2_1;
    out_str << "(" << ptr2_1->m_x << ", " << ptr2_1->m_y << ")"
            << "(" << ptr2_2->m_x << ", " << ptr2_2->m_y << ")";
    EXPECT_EQ(out_str.str(), "(20, 5)(20, 5)");
    out_str.str("");
    out_str << ptr2_1.use_count() << " = "
            << ptr2_2.use_count();
    EXPECT_EQ(out_str.str(), "2 = 2");
    out_str.str("");

    ptr1_1.swap(ptr2_1);

    out_str << "(" << ptr1_1->m_x << ", " << ptr1_1->m_y << ")"
            << "(" << ptr2_1->m_x << ", " << ptr2_1->m_y << ")";
    EXPECT_EQ(out_str.str(), "(20, 5)(3, 4)");
    out_str.str("");
    out_str << ptr1_1.use_count() << " != "
            << ptr2_1.use_count();
    EXPECT_EQ(out_str.str(), "2 != 3");
    out_str.str("");
}

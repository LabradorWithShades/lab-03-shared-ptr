// Copyright 2020 LabradorWithShades
// 70892548+LabradorWithShades@users.noreply.github.com

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <cstdint>
#include <utility>

template <typename T>
class SharedPtr {
 public:
    template<typename... Args>
    static SharedPtr make_shared(Args ...args) {
        char* buffer = new char[sizeof(T) + sizeof(std::size_t)];
        SharedPtr shared_ptr;
        shared_ptr.m_useCount = reinterpret_cast<std::size_t*>(buffer);
        *(shared_ptr.m_useCount) = 1;
        shared_ptr.m_object = new (buffer + sizeof(std::size_t)) T(args...);
        shared_ptr.m_coherent = true;
        return shared_ptr;
    }

    SharedPtr() : m_useCount(nullptr), m_object(nullptr), m_coherent(false) {}
    explicit SharedPtr(T* ptr)
        : m_useCount(new std::size_t(1))
        , m_object(ptr)
        , m_coherent(false) {}
    SharedPtr(const SharedPtr& r)
        : m_useCount(r.m_useCount)
        , m_object(r.m_object)
        , m_coherent(r.m_coherent) {
      ++(*m_useCount);
    }
    SharedPtr(SharedPtr&& r)
        : m_useCount(r.m_useCount)
        , m_object(r.m_object)
        , m_coherent(r.m_coherent) {
      r.m_object = nullptr;
      r.m_useCount = nullptr;
    }
    ~SharedPtr() {
        if (m_useCount == nullptr)
            return;
        --(*m_useCount);
        if (*m_useCount == 0) {
            if (!m_coherent) {
                delete m_object;
                delete m_useCount;
            } else {
                m_object->~T();
                delete[] reinterpret_cast<char*>(m_useCount);
            }
        }
    }

    auto operator=(const SharedPtr& r) -> SharedPtr& {
        this->reset();
        m_useCount = r.m_useCount;
        m_object = r.m_object;
        m_coherent = r.m_coherent;
        ++(*m_useCount);
        return *this;
    }
    auto operator=(SharedPtr&& r) -> SharedPtr& {
        this->reset();
        m_useCount = r.m_useCount;
        m_object = r.m_object;
        m_coherent = r.m_coherent;

        r.m_useCount = nullptr;
        r.m_object = nullptr;
        r.m_coherent = false;

        return *this;
    }

    operator bool() const { return (m_object != nullptr); }
    auto operator*() const -> T& { return *m_object; }
    auto operator->() const -> T* { return m_object; }

    auto get() -> T* { return m_object; }
    void reset() {
        if (m_useCount == nullptr)
            return;
        --(*m_useCount);
        if (*m_useCount == 0) {
            if (!m_coherent) {
                delete m_object;
                delete m_useCount;
            } else {
                m_object->~T();
                delete[] reinterpret_cast<char*>(m_useCount);
            }
        }
        m_object = nullptr;
        m_useCount = nullptr;
        m_coherent = false;
    }
    void reset(T* ptr) {
        reset();
        m_object = ptr;
        m_useCount = new std::size_t(1);
        m_coherent = false;
    }
    void swap(SharedPtr& r) {
        std::swap(r.m_useCount, m_useCount);
        std::swap(r.m_object, m_object);
        std::swap(r.m_coherent, m_coherent);
    }
    auto use_count() const -> std::size_t {
        if (m_useCount == nullptr)
          return 0;
        else
          return *m_useCount;
    }

 private:
    std::size_t* m_useCount;
              T* m_object;
            bool m_coherent;
};

#endif // INCLUDE_SHAREDPTR_HPP_

#pragma once

#include <functional>
#include <typeindex>
#include <utility>

class AnyPtr
{
    using Deleter = std::function<void(void*)>;

    template <typename T>
    static void delete_func(void* data)
    {
        delete reinterpret_cast<T*>(data);
    }

public:
    template <typename T, typename... Args>
    AnyPtr(T* data) : data(data), type(typeid(T)), deleter(delete_func<T>) {}
    AnyPtr(const AnyPtr&) = delete;
    AnyPtr(AnyPtr&& other) : type(other.type) { *this = std::move(other); }
    AnyPtr& operator=(AnyPtr&& other)
    {
        data = other.data;
        other.data = nullptr;
        type = other.type;
        deleter = other.deleter;
        return *this;
    }

    ~AnyPtr()
    {
        if (data != nullptr)
        {
            deleter(data);
        }
    }

    template <typename T> auto& as() { return *reinterpret_cast<T*>(data); }
    template <typename T> auto& as() const { return *reinterpret_cast<T*>(data); }

private:
    void* data;
    std::type_index type;
    Deleter deleter;
};

template <typename T, typename... Args>
AnyPtr make_any(Args&&... args)
{
    return AnyPtr(new T(std::forward<Args>(args)...));
}

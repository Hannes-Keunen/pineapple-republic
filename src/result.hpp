#pragma once

#include <variant>

template <typename T, typename... Errors>
class Result
{
public:
    Result(T&& val) : res(std::forward<T>(val)) {}
    template <typename E>
    Result(E&& err) : res(std::forward<E>(err)) {}

    template <typename... E>
    Result(std::variant<E...>&& other)
    {
        int dummy[sizeof...(E)] = { (lift_error<E>(res), 0)... };
    }

    T unwrap()
    {
        return std::get<T>(std::move(res));
    }

    auto& value() { return std::get<T>(res); }
    auto& value() const { return std::get<T>(res); }
    bool is_ok() const { return std::holds_alternative<T>(res); }

    template <typename E>
    bool is_error() const { return std::holds_alternative<E>(res); }

    auto propagate() -> std::variant<Errors...>
    {
        std::variant<Errors...> result;
        int dummy[sizeof...(Errors)] = { (lift_error<Errors>(result), 0)... };
        return result;
    }

    template <typename E>
    E& error_as()
    {
        return std::get<E>(res);
    }
private:
    template <typename E, typename... Ts>
    void lift_error(std::variant<Ts...>& result)
    {
        if (std::holds_alternative<E>(res))
        {
            result = std::get<E>(std::move(res));
        }
    }

private:
    std::variant<T, Errors...> res;
};
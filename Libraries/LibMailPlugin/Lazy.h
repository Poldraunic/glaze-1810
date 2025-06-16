#pragma once

#include <functional>
#include <optional>

namespace LibBase {

template <typename T>
class Lazy
{
public:
    using Initializer = std::function<T()>;

    explicit Lazy(Initializer initializer)
        : _initializer(std::move(initializer))
        , _value(std::nullopt)
    {
    }

    [[nodiscard]] T &get()
    {
        initializeIfNeeded();
        return _value.value();
    }

    [[nodiscard]] const T &get() const
    {
        initializeIfNeeded();
        return _value.value();
    }

    void set(T value)
    {
        _initializer = {};
        _value = std::move(value);
    }

    [[nodiscard]] bool isInitialized() const { return _value.has_value(); }

    [[nodiscard]] T &operator*() { return get(); }
    [[nodiscard]] const T &operator*() const { return get(); }

    [[nodiscard]] T *operator->() { return &get(); }
    [[nodiscard]] const T *operator->() const { return &get(); }

private:
    void initializeIfNeeded() const
    {
        if (!_value.has_value()) {
            _value = _initializer();
        }
    }

    Initializer _initializer;
    mutable std::optional<T> _value;
};

} // namespace LibBase

/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <optional>

namespace OvTools::Utils
{
	/**
	* Wrapper for std::optional<std::reference_wrapper<T>> with some utility functions
	*/
    template <typename T>
    class OptRef
    {
    public:
        // Constructors
        OptRef() : m_storage() {}
        OptRef(T& value) : m_storage(std::ref(value)) {}
        OptRef(std::nullopt_t) : m_storage() {}
        OptRef(const OptRef& other) : m_storage(other.m_storage) {}

        OptRef& operator=(const OptRef& other)
        {
            m_storage = other.m_storage;
            return *this;
        }

        T& get() const
        {
            return m_storage.value().get();
        }

        T& get_or(T&& other) const
        {
            return has_value() ? get() : other;
        }

        bool has_value() const
        {
            return m_storage.has_value();
        }

        void reset()
        {
            return m_storage = std::nullopt;
        }

        explicit operator bool() const
        {
            return has_value();
        }

    private:
        std::optional<std::reference_wrapper<T>> m_storage;
    };
}
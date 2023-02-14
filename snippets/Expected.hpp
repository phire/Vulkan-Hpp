
#if !defined(VULKAN_HPP_NO_EXCEPTIONS)

// With exceptions, Expected just passes though the type
template<class T>
using Expected = T;
using Unexpected = void;

#elif defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

// Use the C++23 std::expected if it's available
template<class T>
using Expected = std::expected<T, Result>;
using Unexpected = Expected::unexpected_type;

#define VULKAN_HPP_INPLACE_EXPECTED

#elif defined(VULKAN_HPP_CUSTOM_EXPECTED)

// Or accept a user supplied implementation of std::expected
template<class T>
using Expected = VULKAN_HPP_CUSTOM_EXPECTED<T, Result>;
using Unexpected = Expected::unexpected_type;

#else
// Provide our own Expected that implements a subset of std::expected

class Unexpected {
public:

    VULKAN_HPP_CONSTEXPR explicit Unexpected(Result result) noexcept : val(result) {}
    VULKAN_HPP_CONSTEXPR Result error() const noexcept { return val; }
private:
    Result val;
};

template<class T>
class Expected {
public:
    using value_type = T;
    using error_type = Result;
    using unexpected_type = Unexpected;

// constructors
    explicit Expected(const Expected& e) = delete;
    VULKAN_HPP_CONSTEXPR Expected(Expected&& e) VULKAN_HPP_NOEXCEPT = default;
    template<class U>
    VULKAN_HPP_CONSTEXPR Expected(Expected<U>&& e) : has_val(e.has_value())
    {
        if (has_val)
            val = std::move(e.value());
        else
            result = e.error();
    }
    VULKAN_HPP_CONSTEXPR Expected(const Unexpected&& u) VULKAN_HPP_NOEXCEPT : has_val(false), result(u.error()) {}
    VULKAN_HPP_CONSTEXPR explicit Expected(T&& v) VULKAN_HPP_NOEXCEPT : val(std::move(v)) {}

    VULKAN_HPP_CONSTEXPR ~Expected() VULKAN_HPP_NOEXCEPT { if (has_val) val.~T(); }

// assignment
    VULKAN_HPP_CONSTEXPR_14 Expected& operator=(Expected&& e) VULKAN_HPP_NOEXCEPT = default;

// swap
    VULKAN_HPP_CONSTEXPR_14 void swap(Expected& other) VULKAN_HPP_NOEXCEPT
    {
        std::swap(result, other.result);
        std::swap(val, other.val);
    }

// observers
    VULKAN_HPP_CONSTEXPR bool has_value() const VULKAN_HPP_NOEXCEPT { return has_val; }
    VULKAN_HPP_CONSTEXPR_14 const T& value() const& VULKAN_HPP_NOEXCEPT
    {
        VULKAN_HPP_ASSERT_ON_RESULT(has_value());
        return val;
    }
    VULKAN_HPP_CONSTEXPR_14 T& value() & VULKAN_HPP_NOEXCEPT
    {
        VULKAN_HPP_ASSERT_ON_RESULT(has_value());
        return val;
    }
    VULKAN_HPP_CONSTEXPR_14 T&& value() && VULKAN_HPP_NOEXCEPT { return std::move(value()); }
    VULKAN_HPP_CONSTEXPR_14 const Result& error() const VULKAN_HPP_NOEXCEPT
    {
        VULKAN_HPP_ASSERT(!has_value());
        return result;
    }

    VULKAN_HPP_CONSTEXPR const T* operator->() const VULKAN_HPP_NOEXCEPT { return &value(); }
    VULKAN_HPP_CONSTEXPR_14 T* operator->() VULKAN_HPP_NOEXCEPT { return &value(); }
    VULKAN_HPP_CONSTEXPR const T& operator*() const& VULKAN_HPP_NOEXCEPT { return value(); }
    VULKAN_HPP_CONSTEXPR_14 T& operator*() & VULKAN_HPP_NOEXCEPT { return value(); }
    VULKAN_HPP_CONSTEXPR const T&& operator*() const&& VULKAN_HPP_NOEXCEPT { return std::move(value()); }
    VULKAN_HPP_CONSTEXPR_14 T&& operator*() && VULKAN_HPP_NOEXCEPT { return std::move(value()); }
    VULKAN_HPP_CONSTEXPR explicit operator bool() const VULKAN_HPP_NOEXCEPT { return has_value(); }

    template<class U>
    VULKAN_HPP_CONSTEXPR_14 T value_or(U&& u) && VULKAN_HPP_NOEXCEPT
    {
        if (has_value())
            return val;
        return static_cast<T>(std::forward<U>(u));
    }
// equality
    VULKAN_HPP_CONSTEXPR bool operator==(const Unexpected& u) const VULKAN_HPP_NOEXCEPT
    {
        return !has_value() && error() == u.error();
    }

    template<class U>
    friend VULKAN_HPP_CONSTEXPR_14 void swap(Expected&, Expected&);

private:
    bool has_val = true;
    union
    {
        Result result;
        T val;
    };
};

template<class T>
VULKAN_HPP_CONSTEXPR_14 void swap(Expected<T>& x, Expected<T>& y) VULKAN_HPP_NOEXCEPT {
    x.swap(y);
}

// Specialize for void
  template <>
  class Expected<void>
  {
  public:
    using value_type      = void;
    using error_type      = Result;
    using unexpected_type = Unexpected;

    // constructors
    explicit Expected( const Expected & e )                            = delete;
    VULKAN_HPP_CONSTEXPR Expected( Expected && e ) VULKAN_HPP_NOEXCEPT = default;

    template <class U>
    VULKAN_HPP_CONSTEXPR Expected( Expected<U> && e )
    {
      if ( e.has_val() )
        result = Result::eSuccess;
      else
        result = e.error();
    }

    VULKAN_HPP_CONSTEXPR Expected( const Unexpected && u ) VULKAN_HPP_NOEXCEPT
      : result( u.error() )
    {
    }
    // assignment
    VULKAN_HPP_CONSTEXPR_14 Expected & operator=( Expected && e ) VULKAN_HPP_NOEXCEPT = default;

    // observers
    VULKAN_HPP_CONSTEXPR bool has_value() const VULKAN_HPP_NOEXCEPT
    {
      return Result::eSuccess == result;
    }

    VULKAN_HPP_CONSTEXPR_14 const Result & error() const VULKAN_HPP_NOEXCEPT
    {
      VULKAN_HPP_ASSERT( !has_value() );
      return result;
    }

    VULKAN_HPP_CONSTEXPR explicit operator bool() const VULKAN_HPP_NOEXCEPT
    {
      return has_value();
    }

  private:
      Result result;
  };

#endif

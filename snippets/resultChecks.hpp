
  VULKAN_HPP_INLINE bool resultIsSuccess( Result result )
  {
    return result == Result::eSuccess;
  }

  VULKAN_HPP_INLINE bool resultIsSuccess( Result result, std::initializer_list<Result> successCodes )
  {
    return std::find( successCodes.begin(), successCodes.end(), result ) != successCodes.end();
  }

  VULKAN_HPP_INLINE void resultCheck( Result result, char const * message )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    ignore( message );
    VULKAN_HPP_ASSERT_ON_RESULT( result == Result::eSuccess );
#else
    if ( resultIsSuccess(result) )
    {
      throwResultException( result, message );
    }
#endif
  }

  VULKAN_HPP_INLINE void resultCheck( Result result, char const * message, std::initializer_list<Result> successCodes )
  {
#ifdef VULKAN_HPP_NO_EXCEPTIONS
    ignore( result );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    ignore( message );
    ignore( successCodes );  // just in case VULKAN_HPP_ASSERT_ON_RESULT is empty
    VULKAN_HPP_ASSERT_ON_RESULT( resultIsSuccess(result, successCodes) );
#else
    if ( resultIsSuccess(result, successCodes) )
    {
      throwResultException( result, message );
    }
#endif
  }
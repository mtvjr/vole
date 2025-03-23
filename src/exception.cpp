
#include <stdexcept>
#include <vole/exception.hpp>


namespace vole {

    /************************************************************
     * 
     *                  exception
     * 
     ************************************************************/


    exception::exception(std::string text)
        : text(std::move(text))
    { }


    const char * exception::what() const noexcept {
        return text.c_str();
    }


    /************************************************************
     * 
     *            invalid_operation_exception
     * 
     ************************************************************/


    invalid_operation_exception::invalid_operation_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view invalid_operation_exception::type() const noexcept {
        return "invalid_operation_exception";
    }


    /************************************************************
     * 
     *            duplicate_key_exception
     * 
     ************************************************************/


    duplicate_key_exception::duplicate_key_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view duplicate_key_exception::type() const noexcept {
        return "duplicate_key_exception";
    }


    /************************************************************
     * 
     *            no_such_element_exception
     * 
     ************************************************************/


    no_such_element_exception::no_such_element_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view no_such_element_exception::type() const noexcept {
        return "no_such_element_exception";
    }


    /************************************************************
     *
     *            unsupported_element_exception
     *
     ************************************************************/


    unsupported_element_exception::unsupported_element_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view unsupported_element_exception::type() const noexcept(true) {
        return "unsupported_element_exception";
    }


}


#include <vole/exception.hpp>


namespace vole {

    /************************************************************
     * 
     *                  vole::exception
     * 
     ************************************************************/


    vole::exception::exception(std::string text)
        : text(std::move(text))
    { }


    const char * vole::exception::what() const noexcept {
        return text.c_str();
    }


    /************************************************************
     * 
     *            vole::invalid_operation_exception
     * 
     ************************************************************/


    vole::invalid_operation_exception::invalid_operation_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view vole::invalid_operation_exception::type() const noexcept {
        return "invalid_operation_exception";
    }


    /************************************************************
     * 
     *            vole::duplicate_key_exception
     * 
     ************************************************************/


    vole::duplicate_key_exception::duplicate_key_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view vole::duplicate_key_exception::type() const noexcept {
        return "duplicate_key_exception";
    }


    /************************************************************
     * 
     *            vole::no_such_element_exception
     * 
     ************************************************************/


    vole::no_such_element_exception::no_such_element_exception(std::string text)
        : exception(std::move(text))
    {}


    std::string_view vole::no_such_element_exception::type() const noexcept {
        return "no_such_element_exception";
    }


}
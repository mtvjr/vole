
#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace vole {
    /**
     * The common super-class of all exceptions thrown by vole
     */
    class exception : public std::exception {
    public:
        explicit exception(std::string text);
        const char * what() const noexcept override;
        [[nodiscard]] virtual std::string_view type() const noexcept(true) = 0;
    private:
        std::string text;
    };


    class duplicate_key_exception : public vole::exception {
    public:
        explicit duplicate_key_exception(std::string text);
        [[nodiscard]] std::string_view type() const noexcept(true) override;
    };


    class invalid_operation_exception : public vole::exception {
    public:
        explicit invalid_operation_exception(std::string text);
        [[nodiscard]] std::string_view type() const noexcept(true) override;
    };


    class no_such_element_exception : public vole::exception {
    public:
        explicit no_such_element_exception(std::string text);
        [[nodiscard]] std::string_view type() const noexcept(true) override;
    };

}

#include <cmath>
#include <vole/datamodel.hpp>

#include <regex>
#include <type_traits>

#include <vole/exception.hpp>

using namespace vole;
using namespace std::string_literals;



namespace vole::datamodel {

    /************************************************************
     * 
     *                  vole::datamodel::node
     * 
     ************************************************************/


    std::string node::format_debug() const {
        return std::format("{}<{}>", type(), name());
    }


    /************************************************************
     * 
     *                  vole::datamodel::array_node
     * 
     ************************************************************/


    std::string array_node::type() const {
        return "array_node";
    }


    shared_node array_node::get_child(size_t index) const {
        try {
            return children.at(index);
        } catch (std::out_of_range) {
            throw no_such_element_exception(
                std::format("{} {} does not contain an element at index{}",
                    type(), name(), index)
            );
        }
    }


    void array_node::apply(node_visitor &visitor) const {
        visitor.visit(*this);
    }


    void array_node::add_child(shared_node newChild) {
        children.push_back(std::move(newChild));
    }


    const node_list& array_node::get_children() const {
        return children;
    }


    /************************************************************
     * 
     *                  vole::datamodel::literal_node
     * 
     ************************************************************/

    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };


    std::string literal_node::type() const {
        auto type_printer = overloaded {
            [](const num_type &) {return "number";},
            [](const string_type &) {return "string";},
            [](const bool_type &) {return "bool";},
            [](const null_type) {return "null";}
        };
        std::string held_type{std::visit(type_printer, value)};
        return std::format("literal_node<{}>", held_type);
    }


    std::string literal_node::render() const {
        auto value_printer = overloaded {
            [](const num_type &v) {
                // if it's an integer, don't display zeroes
                double integral = 0.0;
                if (std::modf(v, &integral) == 0.0) {
                    return std::format("{:.0f}", v);
                }
                return std::to_string(v);
            },
            [](const string_type &v) {return v;},
            [](const bool_type &v) {return v ? "true"s : "false"s;},
            [](const null_type) {return "null"s;}
        };
        std::string rendered = std::visit(value_printer, value);
        return rendered;
    }


    void literal_node::apply(node_visitor &visitor) const {
        visitor.visit(*this);
    }


    /************************************************************
     * 
     *                  vole::datamodel::object_node
     * 
     ************************************************************/


    shared_node object_node::get_child(std::string_view name) const {
        auto childItr = std::find_if(children.begin(), children.end(),
            [name](const auto &child){return child->name() == name;});

        if (childItr == children.end()) {
            throw no_such_element_exception(
                std::format("{} {} does not contain an element with name {}",
                    type(), this->name(), name)
            );
        }

        return *childItr;
    }


    std::string object_node::type() const {
        return "object_node";
    }


    void object_node::add_child(shared_node newChild) {
        auto existingChildItr = std::find_if(children.begin(), children.end(),
            [&newChild](const auto &child){return child->name() == newChild->name();});
        
        if (existingChildItr != children.end()) {
            throw duplicate_key_exception(
                std::format(
                    "Unable to add {} to {} as there already exists an element with that name",
                    newChild->name(), this->name()
                )
            );
        }
        
        children.push_back(std::move(newChild));
    }


    const node_list& object_node::get_children() const {
        return children;
    }


    void object_node::apply(node_visitor &visitor) const {
        visitor.visit(*this);
    }

}



#include <cmath>
#include <vole/datamodel.hpp>

#include <regex>
#include <type_traits>

#include <vole/exception.hpp>

#include "vole/datamodel_parsers.hpp"

using namespace vole;
using namespace std::string_literals;



namespace vole::datamodel {

    /************************************************************
     * 
     *                  vole::datamodel::node
     * 
     ************************************************************/


    std::string node::format_debug() const {
        return std::format("{}{{{}}}", type(), name());
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


    void array_node::apply(const_node_visitor &visitor) const {
        visitor.visit(*this);
    }


    void array_node::apply(node_visitor &visitor) {
        visitor.visit(*this);
    }


    void array_node::add_child(shared_node newChild) {
        children.push_back(std::move(newChild));
    }


    const node_list& array_node::get_children() const {
        return children;
    }

    bool array_node::operator==(const node &other) const {
        const auto other_array = dynamic_cast<const array_node*>(&other);
        if (other_array == nullptr) {
            // Not an array
            return false;
        }

        if (this->name() != other_array->name()) {
            return false;
        }

        if (this->children.size() != other_array->children.size()) {
            return false;
        }

        for (size_t i = 0; i < this->children.size(); i++) {
            if (*this->children[i] != *other_array->children[i]) {
                return false;
            }
        }

        return true;
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


    void literal_node::apply(const_node_visitor &visitor) const {
        visitor.visit(*this);
    }


    void literal_node::apply(node_visitor &visitor) {
        visitor.visit(*this);
    }

    bool literal_node::operator==(const node &other) const {
        const auto other_literal = dynamic_cast<const literal_node*>(&other);
        if (other_literal == nullptr) {
            // Not a literal
            return false;
        }

        if (this->name() != other_literal->name()) {
            return false;
        }

        if (this->value != other_literal->value) {
            return false;
        }

        return true;
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


    void object_node::apply(const_node_visitor &visitor) const {
        visitor.visit(*this);
    }


    void object_node::apply(node_visitor &visitor) {
        visitor.visit(*this);
    }


    bool object_node::operator==(const node &other) const {
        const auto other_object = dynamic_cast<const object_node*>(&other);
        if (other_object == nullptr) {
            // Not an object
            return false;
        }

        if (this->name() != other_object->name()) {
            return false;
        }

        if (this->children.size() != other_object->children.size()) {
            return false;
        }

        for (size_t i = 0; i < this->children.size(); i++) {
            if (*this->children[i] != *other_object->children[i]) {
                return false;
            }
        }

        return true;
    }


    /************************************************************
     *
     *                  vole::datamodel::lambda_node_visitor
     *
     ************************************************************/


    lambda_node_visitor::lambda_node_visitor(
        const array_function &on_array,
        const literal_function &on_literal,
        const object_function &on_object
    )
        : on_array(on_array), on_literal(on_literal), on_object(on_object)
    {}


    void lambda_node_visitor::visit(array_node &node) {
        on_array(node);
    }


    void lambda_node_visitor::visit(literal_node &node) {
        on_literal(node);
    }


    void lambda_node_visitor::visit(object_node &node) {
        on_object(node);
    }


    /************************************************************
     *
     *                  vole::datamodel::lambda_node_visitor
     *
     ************************************************************/


    lambda_const_node_visitor::lambda_const_node_visitor(
        const array_function &on_array,
        const literal_function &on_literal,
        const object_function &on_object
    )
        : on_array(on_array), on_literal(on_literal), on_object(on_object)
    {}


    void lambda_const_node_visitor::visit(const array_node &node) {
        on_array(node);
    }


    void lambda_const_node_visitor::visit(const literal_node &node) {
        on_literal(node);
    }


    void lambda_const_node_visitor::visit(const object_node &node) {
        on_object(node);
    }


    /************************************************************
     *
     *                  vole::datamodel::node_descender
     *
     ************************************************************/


    void node_descender::visit(const array_node &node) {
        on_enter(node);
        depth++;
        for (const auto &child : node.get_children()) {
            child->apply(*this);
        }
        depth--;
        on_exit(node);
    }


    void node_descender::visit(const literal_node &node) {
        on_enter(node);
        on_exit(node);
    }


    void node_descender::visit(const object_node &node) {
        on_enter(node);
        depth++;
        for (const auto &child : node.get_children()) {
            child->apply(*this);
        }
        depth--;
        on_exit(node);
    }

    size_t node_descender::get_depth() const {
        return depth;
    }




}


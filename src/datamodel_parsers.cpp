
#include "vole/datamodel_parsers.hpp"

#include <optional>
#include <fstream>
#include <nlohmann/json.hpp>

#include "vole/exception.hpp"

namespace vole::datamodel {

    class json_sax_listener : public nlohmann::json_sax<nlohmann::json> {
    public:

        shared_node get_built_node() {
            return builder.get_root();
        }

        bool binary(binary_t &) override {
            throw unsupported_element_exception("Binary data is not supported");
        }

        bool boolean(bool) override {
            builder.add_child(make_literal(gen_node_name("Boolean"), true));
            return true;
        }

        bool start_array(size_t) override {
            builder.add_child(make_array(gen_node_name("Array")));
            return true;
        }

        bool end_array() override {
            builder.close_parent();
            return true;
        }

        bool start_object(size_t elements) override {
            builder.add_child(make_object(gen_node_name("Object")));
            return true;
        }

        bool end_object() override {
            builder.close_parent();
            return true;
        }

        bool key(string_t &key) override {
            // Save the key to use in object naming later
            last_key = key;
            return true;
        }

        bool null() override {
            builder.add_child(make_literal(gen_node_name("Null"), nullptr));
            return true;
        }

        bool number_integer(number_integer_t val) override {
            builder.add_child(make_literal(gen_node_name("Number"), static_cast<double>(val)));
            return true;
        }

        bool number_unsigned(number_unsigned_t val) override {
            builder.add_child(make_literal(gen_node_name("Number"), static_cast<double>(val)));
            return true;
        }

        bool number_float(number_float_t val, const string_t &s) override {
            builder.add_child(make_literal(gen_node_name("Number"), val));
            return true;
        }

        bool string(string_t &val) override {
            builder.add_child(make_literal(gen_node_name("String"), val));
            return true;
        }

        bool parse_error(std::size_t location, const std::string &token, const nlohmann::detail::exception& e) override {
            throw unsupported_element_exception("Errors are not supported");
        }

        std::string gen_node_name(std::string_view type) {
            if (last_key.has_value()) {
                const std::string name = last_key.value();
                last_key.reset();
                return name;
            }
            // Last parent was an array, number it!
            if (auto last_array = std::dynamic_pointer_cast<array_node>(builder.last_parent())) {
                auto length = last_array->get_children().size();
                return std::format("{}[{}]", last_array->name(), length);
            }
            return std::format("Unnamed{}{}", type, unnamed_object_count++);
        }
    private:
        node_tree_builder builder;
        std::optional<std::string> last_key = "RootNode";
        size_t unnamed_object_count = 0;
    };

    shared_node json_parser::parse(std::string_view input) {
        json_sax_listener listener;
        nlohmann::json::sax_parse(input, &listener);
        return listener.get_built_node();
    }


    /************************************************************
     *
     *                  vole::datamodel::node_tree_builder
     *
     ************************************************************/


    node_tree_builder::node_tree_builder() = default;


    void node_tree_builder::add_child(const shared_node &newNode) {
        if (node_stack.empty()) {
            root_node = newNode;
            node_stack.push_back(newNode);
            return;
        }

        // Create the inserter object. This enables us to insert elements into the node_tree
        lambda_node_visitor inserter {
            [&](array_node &node) {
                node.add_child(newNode);
            },
            [&](literal_node &) {
                throw invalid_operation_exception("Cannot add children to a literal node");
            },
            [&](object_node &node) {
                node.add_child(newNode);
            },
        };
        // Create the visitor object. This enables us to determine what type of node is being added.
        lambda_node_visitor visitor {
            [&](const array_node &node) {
                node_stack.back()->apply(inserter);
                node_stack.push_back(newNode);
            },
            [&](const literal_node &) {
                node_stack.back()->apply(inserter);
            },
            [&](const object_node &) {
                node_stack.back()->apply(inserter);
                node_stack.push_back(newNode);
            },
        };
        newNode->apply(visitor);
    }


    void node_tree_builder::close_parent() {
        node_stack.pop_back();
    }


    shared_node node_tree_builder::get_root() {
        return root_node;
    }


    shared_node node_tree_builder::last_parent() {
        return this->node_stack.back();
    }



}
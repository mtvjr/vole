
#pragma once

#include <filesystem>

#include "vole/datamodel.hpp"

namespace vole::datamodel {

    class parser {
    public:
        virtual ~parser() = default;
        [[nodiscard]] virtual shared_node parse(std::string_view input) = 0;
    };

    class json_parser : public parser {
    public:
        [[nodiscard]] shared_node parse(std::string_view input) override;
    };

    class node_tree_builder {
    public:
        node_tree_builder();
        void add_child(const shared_node &node);
        void close_parent();
        shared_node get_root();
        shared_node last_parent();
    protected:
        node_list node_stack;
        shared_node root_node = nullptr;
    };

    // class xml_parser : public parser {
    //     [[nodiscard]] shared_node parse(std::string file) override;
    // };
}

#include "vole/node_printer.hpp"

namespace vole {


    std::string node_printer::render(const datamodel::node &node) {
        renderer.clear();
        node.apply(*this);
        return renderer.str();
    }

    void node_printer::on_enter(const datamodel::array_node &node) {
        append(node.format_debug());
    }


    void node_printer::on_enter(const datamodel::literal_node &node) {
        append(node.format_debug());
    }


    void node_printer::on_enter(const datamodel::object_node &node) {
        append(node.format_debug());
    }


    void node_printer::append(std::string_view text) {
        constexpr size_t TAB_SIZE = 2;
        renderer << std::string(get_depth() * TAB_SIZE, ' ') << text << '\n';
    }


}
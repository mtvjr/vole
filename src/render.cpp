
#include "vole/render.hpp"

namespace vole {


    std::string debug_renderer::render(const datamodel::node &node) {
        renderer.clear();
        node.apply(*this);
        return renderer.str();
    }

    void debug_renderer::on_enter(const datamodel::array_node &node) {
        append(node.format_debug());
    }


    void debug_renderer::on_enter(const datamodel::literal_node &node) {
        append(node.format_debug());
    }


    void debug_renderer::on_enter(const datamodel::object_node &node) {
        append(node.format_debug());
    }


    void debug_renderer::append(std::string_view text) {
        constexpr size_t TAB_SIZE = 2;
        renderer << std::string(get_depth() * TAB_SIZE, ' ') << text << '\n';
    }


}
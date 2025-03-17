
#pragma once

#include <cstdint>
#include <string>
#include <sstream>

#include "vole/datamodel.hpp"

namespace vole {
    /**
     * The debug renderer class is able to render a node datamodel
     * tree as a string.
     */
    class debug_renderer : public datamodel::node_descender {
    public:
        ~debug_renderer() override = default;

        /**
         * Render the given node-tree as a tree for debug purposes.
         * @param node The root of the tree
         * @return A string representing the tree on one or more lines
         */
        std::string render(const datamodel::node &node);
    protected:
        void on_enter(const datamodel::array_node &node) override;
        void on_enter(const datamodel::literal_node &node) override;
        void on_enter(const datamodel::object_node &node) override;
        virtual void append(std::string_view text);
    private:
        std::stringstream renderer;
    };
}

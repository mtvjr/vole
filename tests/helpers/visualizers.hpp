
#pragma once
#include <ostream>

#include <vole/datamodel.hpp>
#include <vole/node_printer.hpp>

namespace vole::datamodel {

    template <typename T>
    concept node_type = std::is_base_of_v<node, T>;

    template<node_type N>
    void PrintTo(const N &node, std::ostream *os) {
        node_printer renderer;
        *os << renderer.render(node);
    }

}

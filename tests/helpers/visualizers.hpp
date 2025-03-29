
#pragma once
#include <ostream>

#include <vole/datamodel.hpp>
#include <vole/node_printer.hpp>

namespace vole::datamodel {

    void PrintTo(const datamodel::node &node, std::ostream *os) {
        node_printer renderer;
        *os << renderer.render(node);
    }

}

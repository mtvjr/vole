
#include <iostream>
#include <cstdlib>

#include <vole/exception.hpp>
#include <vole/datamodel.hpp>
#include <vole/render.hpp>

using namespace vole::datamodel;

int main() {
    auto node = make_object("RootNode");
    auto array = make_array("ArrayNode");
    node->add_child(array);
    array->add_child(make_literal("Child1", "Hello World!"));
    array->add_child(make_literal("Child2", 2.0));
    array->add_child(make_literal("Child3", true));
    array->add_child(make_literal("Child4", nullptr));
    vole::debug_renderer renderer;
    std::cout << renderer.render(*node) << std::endl;

    return EXIT_SUCCESS;
}
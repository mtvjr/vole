
#include <vole/datamodel.hpp>

#include <gtest/gtest.h>
#include "helpers/assertions.h"
#include "vole/exception.hpp"

using namespace std::literals;


TEST(literal_node, render_string) {
    const vole::datamodel::literal_node node("RootNode", "Hello!"s);
    EXPECT_EQ(node.render(), "Hello!");
}


TEST(literal_node, render_number) {
    const vole::datamodel::literal_node whole_number("WholeNumber", 200.0);
    EXPECT_EQ(whole_number.render(), "200");

    const vole::datamodel::literal_node fractional_number("RootNode", 200.250);
    EXPECT_STARTS_WITH(fractional_number.render(), "200.250");
}


TEST(array_node, get_child) {
    vole::datamodel::array_node array("RootNode");
    auto child1 = std::make_shared<vole::datamodel::literal_node>("Child1", "I am one");
    auto child2 = std::make_shared<vole::datamodel::literal_node>("Child2", 2.0);
    array.add_child(child1);
    array.add_child(child2);
    EXPECT_EQ(array.get_child(0), child1);
    EXPECT_EQ(array.get_child(1), child2);

    EXPECT_THROW((void)array.get_child(2), vole::no_such_element_exception);
}


TEST(object_node, get_child) {
    vole::datamodel::object_node object("RootNode");
    auto child1 = std::make_shared<vole::datamodel::literal_node>("Child1", "I am one");
    auto child2 = std::make_shared<vole::datamodel::literal_node>("Child2", 2.0);
    object.add_child(child1);
    object.add_child(child2);
    EXPECT_EQ(object.get_child("Child1"), child1);
    EXPECT_EQ(object.get_child("Child2"), child2);
}

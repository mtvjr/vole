

#include <vole/datamodel_parsers.hpp>
#include "helpers/visualizers.hpp"

#include <gtest/gtest.h>

#include "helpers/assertions.h"

TEST(json_parser, BasicObject) {
    vole::datamodel::json_parser parser;
    const auto text = R"({"name": "John", "age": 7, "alive": true})";
    auto parsed_node = parser.parse(text);
    auto expected_node = vole::datamodel::make_object("RootNode");
    expected_node->add_child(vole::datamodel::make_literal("name", "John"));
    expected_node->add_child(vole::datamodel::make_literal("age", 7.0));
    expected_node->add_child(vole::datamodel::make_literal("alive", true));
    ASSERT_NODE_EQ(*parsed_node, *expected_node);
}

TEST(json_parser, Basic_Array) {
    vole::datamodel::json_parser parser;
    const auto text = R"([1.2, 3.4, 28.0])";
    auto parsed_node = parser.parse(text);
    auto expected_node = vole::datamodel::make_array("RootNode");
    expected_node->add_child(vole::datamodel::make_literal("RootNode[0]", 1.2));
    expected_node->add_child(vole::datamodel::make_literal("RootNode[1]", 3.4));
    expected_node->add_child(vole::datamodel::make_literal("RootNode[2]", 28.0));
    ASSERT_NODE_EQ(*parsed_node, *expected_node);
}


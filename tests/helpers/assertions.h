//
// Created by Michael Volling on 3/16/2025.
//

#pragma once

#include <gtest/gtest.h>
#include <vole/render.hpp>

/**
 * Validate that a string begins with the expected prefix.
 * Continue test execution if the test fails.
 * @param actual A string to test
 * @param expected The expected prefix the actual string must start with
 */
#define EXPECT_STARTS_WITH(actual, expected) \
    EXPECT_TRUE(actual.starts_with(expected))


#define ASSERT_NODE_EQ(v1, v2)                            \
    ASSERT_EQ(v1, v2)                                     \
    << "\nNode 1:\n" << vole::debug_renderer().render(v1) \
    << "\nNode 2:\n" << vole::debug_renderer().render(v2);

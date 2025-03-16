//
// Created by Michael Volling on 3/16/2025.
//

#pragma once

#include <gtest/gtest.h>

/**
 * Validate that a string begins with the expected prefix.
 * Continue test execution if the test fails.
 * @param actual A string to test
 * @param expected The expected prefix the actual string must start with
 */
#define EXPECT_STARTS_WITH(actual, expected) \
    EXPECT_TRUE(actual.starts_with(expected))

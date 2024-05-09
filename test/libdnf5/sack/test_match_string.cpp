/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "test_match_string.hpp"

#include <libdnf5/common/exception.hpp>
#include <libdnf5/common/sack/match_string.hpp>


using namespace libdnf5::sack;


CPPUNIT_TEST_SUITE_REGISTRATION(SackMatchStringTest);


void SackMatchStringTest::test() {
    const std::string value{"AbCdEfGhIjKlMnOp"};

    const std::string test_patterns[]{
        "AbCdEfGhIjKlMnOp",
        "ABcdEfGhIjKlMNop",
        "AbCdEf",
        "ABcdEf",
        "KlMnOp",
        "KlMNoP",
        "EfGh",
        "EFgh",
        "A[a-d]Cd*Gh*Ij?lMnOp",
        "A[A-D]Cd*Gh*Ij?lMNoP",
        "A[bdCE]+fGhIj.lMnOp",
        "A[b-e]+fGhIj.lMNop"};

    struct OperatorTests {
        // tested operator
        QueryCmp cmp;
        union {
            // expected results by name
            struct {
                bool exact;
                bool iexact;
                bool start;
                bool istart;
                bool end;
                bool iend;
                bool contain;
                bool icontain;
                bool glob;
                bool iglob;
                bool reg;
                bool ireg;
            };
            // expected results as array
            bool results[12];
        };
    };

    const OperatorTests tests[]{
        {QueryCmp::EXACT, {true, false, false, false, false, false, false, false, false, false, false, false}},
        {QueryCmp::IEXACT, {true, true, false, false, false, false, false, false, false, false, false, false}},
        {QueryCmp::GLOB, {true, false, false, false, false, false, false, false, true, false, false, false}},
        {QueryCmp::IGLOB, {true, true, false, false, false, false, false, false, true, true, false, false}},
        {QueryCmp::REGEX, {true, false, false, false, false, false, false, false, false, false, true, false}},
        {QueryCmp::IREGEX, {true, true, false, false, false, false, false, false, false, false, true, true}},
        {QueryCmp::CONTAINS, {true, false, true, false, true, false, true, false, false, false, false, false}},
        {QueryCmp::ICONTAINS, {true, true, true, true, true, true, true, true, false, false, false, false}},
        {QueryCmp::STARTSWITH, {true, false, true, false, false, false, false, false, false, false, false, false}},
        {QueryCmp::ISTARTSWITH, {true, true, true, true, false, false, false, false, false, false, false, false}},
        {QueryCmp::ENDSWITH, {true, false, false, false, true, false, false, false, false, false, false, false}},
        {QueryCmp::IENDSWITH, {true, true, false, false, true, true, false, false, false, false, false, false}}};

    for (const auto & operator_tests : tests) {
        for (std::size_t i = 0; i < sizeof(test_patterns) / sizeof(test_patterns[0]); ++i) {
            CPPUNIT_ASSERT_EQUAL(operator_tests.results[i], match_string(value, operator_tests.cmp, test_patterns[i]));
        }

        // Tests with the NOT modifier/flag.
        for (std::size_t i = 0; i < sizeof(test_patterns) / sizeof(test_patterns[0]); ++i) {
            CPPUNIT_ASSERT_EQUAL(
                !operator_tests.results[i], match_string(value, operator_tests.cmp | QueryCmp::NOT, test_patterns[i]));
        }
    }
}


void SackMatchStringTest::test_invalid() {
    const std::string VALUE = "VALUE";
    const std::string PATTERN = "PATTERN";

    CPPUNIT_ASSERT_THROW(match_string(VALUE, QueryCmp::NOT, PATTERN), libdnf5::AssertionError);
    CPPUNIT_ASSERT_THROW(match_string(VALUE, QueryCmp::ICASE, PATTERN), libdnf5::AssertionError);
    CPPUNIT_ASSERT_THROW(match_string("VALUE", QueryCmp::NOT | QueryCmp::ICASE, PATTERN), libdnf5::AssertionError);

    CPPUNIT_ASSERT_THROW(match_string("VALUE", QueryCmp::ISNULL, PATTERN), libdnf5::AssertionError);
    CPPUNIT_ASSERT_THROW(match_string("VALUE", QueryCmp::GT, PATTERN), libdnf5::AssertionError);
    CPPUNIT_ASSERT_THROW(match_string("VALUE", QueryCmp::GTE, PATTERN), libdnf5::AssertionError);
    CPPUNIT_ASSERT_THROW(match_string("VALUE", QueryCmp::LT, PATTERN), libdnf5::AssertionError);
    CPPUNIT_ASSERT_THROW(match_string("VALUE", QueryCmp::LTE, PATTERN), libdnf5::AssertionError);
}

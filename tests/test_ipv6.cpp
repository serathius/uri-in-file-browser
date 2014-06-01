#include "gtest/gtest.h"
#include "../include/parser.h"

TEST (IPv6Test, test_example0)
{
    const char arg[] = "1:2:3:4:5:6:7:8";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example1)
{
    const char arg[] = "0123:4567:89ab:cdef:0123:4567:89ab:cdef";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example2)
{
    const char arg[] = "0123:4567:89ab:cdef:0123:4567:127.0.0.1";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example3)
{
    const char arg[] = "0123:4567::89ab:cdef";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example4)
{
    const char arg[] = "0123:4567:89ab:cdef:0123:4567:89ab::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example5)
{
    const char arg[] = "0123:4567:89ab:cdef:0123:4567::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example6)
{
    const char arg[] = "0123:4567:89ab:cdef:0123::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example7)
{
    const char arg[] = "0123:4567:89ab:cdef::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example8)
{
    const char arg[] = "0123:4567:89ab::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example9)
{
    const char arg[] = "0123:4567::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example10)
{
    const char arg[] = "0123::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example11)
{
    const char arg[] = "::";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example12)
{
    const char arg[] = "::0123";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example13)
{
    const char arg[] = "::0123:4567";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}
 
TEST (IPv6Test, test_example14)
{
    const char arg[] = "::0123:4567:89ab";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example15)
{
    const char arg[] = "::0123:4567:89ab:cdef";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example16)
{
    const char arg[] = "::0123:4567:89ab:cdef:0123";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (IPv6Test, test_example17)
{
    const char arg[] = "::0123:4567:89ab:cdef:0123:4567:89ab";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_ipv6address(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}
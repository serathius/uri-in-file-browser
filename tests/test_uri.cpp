#include "gtest/gtest.h"
#include "../include/parser.h"

TEST (URITest, test_example0)
{
    const char arg[] = "g:h";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example1)
{
    const char arg[] = "http://a";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example2)
{
    const char arg[] = "http://a/";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example3)
{
    const char arg[] = "http://a/b";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example4)
{
    const char arg[] = "http://a/b/";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example5)
{
    const char arg[] = "http://a/b/c";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example6)
{
    const char arg[] = "http://a/b/c/";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example7)
{
    const char arg[] = "http://a/b/c/d;p?y";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example8)
{
    const char arg[] = "http://a/b/c/g?y";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example9)
{
    const char arg[] = "http://a/b/c/d;p?q#s";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example10)
{
    const char arg[] = "http://a/b/c/g#s";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example11)
{
    const char arg[] = "http://a/b/c/g?y#s";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example12)
{
    const char arg[] = "http://a/b/c/;x";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example13)
{
    const char arg[] = "http://a/b/c/g;x";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example14)
{
    const char arg[] = "http://a/b/c/g;x?y#s";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example15)
{
    const char arg[] = "http://a/b/c/d;p?q";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example16)
{
    const char arg[] = "http://a/b/c/g.";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example17)
{
    const char arg[] = "http://a/b/c/.g";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example18)
{
    const char arg[] = "http://a/b/c/g..";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example19)
{
    const char arg[] = "http://a/b/c/..g";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example20)
{
    const char arg[] = "http://a/b/c/g:h";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example21)
{
    const char arg[] = "http://www.w%33.org";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example22)
{
    const char arg[] = "http://www.w3.org";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example23)
{
    const char arg[] = "http://r%C3%A4ksm%C3%B6rg%C3%A5s.josefsson.org";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example24)
{
    const char arg[] = "http://xn--rksmrgs-5wao1o.josefsson.org";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example25)
{
    const char arg[] = "http://%E7%B4%8D%E8%B1%86.w3.mag.keio.ac.jp";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example26)
{
    const char arg[] = "http://xn--99zt52a.w3.mag.keio.ac.jp";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example27)
{
    const char arg[] = "http://www.xn--n8jaaaaai5bhf7as8fsfk3jnknefdde3fg11amb5gzdb4wi9bya3kc6lra.w3.mag.keio.ac.jp/";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}

TEST (URITest, test_example28)
{
    const char arg[] = "http://xn--n8jaaaaai5bhf7as8fsfk3jnknefdde3fg11amb5gzdb4wi9bya3kc6lra.xn--n8jaaaaai5bhf7as8fsfk3jnknefdde3fg11amb5gzdb4wi9bya3kc6lra.xn--n8jaaaaai5bhf7as8fsfk3jnknefdde3fg11amb5gzdb4wi9bya3kc6lra.w3.mag.keio.ac.jp/";
    StringAccess * access = new StringAccess(arg, sizeof(arg));
    TextCursor cursor(access);
    cursor = parse_uri(cursor);
    EXPECT_EQ(sizeof(arg) - 1, cursor.get_offset());
}
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(FakeTest, PleaseDeleteOnceYouWriteSome) {
  // If no tests exist, GoogleTest fails to compile with a fairly confusing
  // message. We have this empty test that does nothing to allow an empty
  // autograder submission to compile and regularly fail. Once you write your
  // own tests for the project, you can delete this one.
  EXPECT_EQ(1, 1);
}

// Clean Token Testing
TEST(CleanToken, PreSuffMidCleaning) {
  // Tokens without any punctuation or special characters
  ASSERT_THAT(cleanToken("helloisthiswork1ng"), StrEq("helloisthiswork1ng"));
  ASSERT_THAT(cleanToken("willipas5"),
              StrEq("willipas5"));           // #s at the end of a string
  ASSERT_THAT(cleanToken("o"), StrEq("o"));  // one letter long string

  ASSERT_THAT(cleanToken("!"), StrEq(""));  // no letters
  // lots of punctuation + punctuation in the beginning and end
  ASSERT_THAT(cleanToken("!;yuh!?"), StrEq("yuh"));

  // Tokens with punctuation at the beginning
  ASSERT_THAT(cleanToken("lol__"), StrEq("lol"));
  // Punctuation at the middle and end
  ASSERT_THAT(cleanToken("umm...U.like.2.code?!"),
              StrEq("umm...u.like.2.code"));
  // Start and end with non punctuation
  ASSERT_THAT(cleanToken("Yes):I.do"), StrEq("yes):i.do"));
  // Punctuation at the middle and start
  ASSERT_THAT(cleanToken("__l_o_l"), StrEq("l_o_l"));
}

// GaherTokens Testing
TEST(GatherTokens, Normal) {
  string text = "Hi! Is this working?";
  set<string> expected = {"hi", "is", "this", "working"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, leadingSpaces) {
  string text = "   HELLO?!";
  set<string> expected = {"hello"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, middleSpaces) {
  string text = "mama   a girl behind you";
  set<string> expected = {"mama", "a", "girl", "behind", "you"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, multipleSpaces) {
  string text = "autograder!  are  YOU  seeing  this??";
  set<string> expected = {"autograder", "are", "you", "seeing", "this"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(GatherTokens, trailingSpaces) {
  string text = "BYE?!   ";
  set<string> expected = {"bye"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected))
      << "text=\"" << text << "\"";
}

TEST(BuildIndex, FileNotFound) {
  string filename = "thisFileDNE.txt";
  map<string, set<string>> expectedIndex = {};
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(0)) << retTestFeedback;
}

TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}},
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

map<string, set<string>> TinyTxt = {
    {"eggs", {"www.shoppinglist.com"}},
    {"milk", {"www.shoppinglist.com"}},
    {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
    {"bread", {"www.shoppinglist.com"}},
    {"cheese", {"www.shoppinglist.com"}},
    {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
    {"gre-en", {"www.rainbow.org"}},
    {"orange", {"www.rainbow.org"}},
    {"yellow", {"www.rainbow.org"}},
    {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
    {"indigo", {"www.rainbow.org"}},
    {"violet", {"www.rainbow.org"}},
    {"one", {"www.dr.seuss.net"}},
    {"two", {"www.dr.seuss.net"}},
    {"i'm", {"www.bigbadwolf.com"}},
    {"not", {"www.bigbadwolf.com"}},
    {"trying", {"www.bigbadwolf.com"}},
    {"to", {"www.bigbadwolf.com"}},
    {"eat", {"www.bigbadwolf.com"}},
    {"you", {"www.bigbadwolf.com"}},
};

TEST(FindQueryMatches, notInIndex) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(TinyTxt, "bye"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(TinyTxt, "blue -fish +walmart"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(TinyTxt, "you +Hello! CS251"),
              ContainerEq(expected));

  expected = {"www.bigbadwolf.com"};
  EXPECT_THAT(findQueryMatches(TinyTxt, "you -bruh"), ContainerEq(expected));
}
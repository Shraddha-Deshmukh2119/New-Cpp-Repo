#include <gtest/gtest.h>
#include <cstring>
#include <sstream>
#include "Complaint_Base.h"
#include "test_helpers.h"

TEST(ServerComplaintBaseTest, WriteAndSeeComplaint)
{
    const std::string file = test_helpers::tempFile("complaints.bin");
    test_helpers::removeIfExists(file);

    Complaint_Base base(file);
    const char client[] = "Alice";
    const char text[] = "Late delivery";

    EXPECT_TRUE(base.write(text, client));

    Complaint read{};
    EXPECT_TRUE(base.see(read, client, true));
    EXPECT_STREQ(text, read.text);
    EXPECT_TRUE(read.seen);

    test_helpers::removeIfExists(file);
}

TEST(ServerComplaintBaseTest, ViewFormatsOutput)
{
    Complaint c{};
    strcpy(c.client, "Bob");
    strcpy(c.text, "Damaged item");
    strcpy(c.answer, "*no answer*");
    c.answered = false;
    c.seen = false;

    std::ostringstream os;
    Complaint_Base base("unused.bin");
    base.view(c, os);
    EXPECT_NE(std::string::npos, os.str().find("Bob"));
    EXPECT_NE(std::string::npos, os.str().find("Damaged item"));
}

TEST(ServerComplaintBaseTest, UpdateComplaintText)
{
    const std::string file = test_helpers::tempFile("complaints_update.bin");
    test_helpers::removeIfExists(file);

    Complaint_Base base(file);
    const char client[] = "Carol";
    const char text[] = "Wrong item";
    ASSERT_TRUE(base.write(text, client));

    bool answered = true;
    const char updated[] = "Wrong item - urgent";
    EXPECT_TRUE(base.update(updated, client, answered));
    EXPECT_FALSE(answered);

    test_helpers::removeIfExists(file);
}

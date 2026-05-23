#include <gtest/gtest.h>
#include "Complaint_C.h"
#include "test_helpers.h"

TEST(ServerComplaintCTest, WriteAndReview)
{
    const std::string file = test_helpers::tempFile("complaints_c.bin");
    test_helpers::removeIfExists(file);

    const char clientName[] = "Dave";
    Complaint_C complaint(clientName, file);
    const char text[] = "Billing issue";
    EXPECT_TRUE(complaint.write(text));

    std::ostringstream os;
    complaint.reView(os);
    EXPECT_NE(std::string::npos, os.str().find("Dave"));

    test_helpers::removeIfExists(file);
}

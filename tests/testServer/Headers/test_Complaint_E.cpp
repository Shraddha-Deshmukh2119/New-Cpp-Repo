#include <gtest/gtest.h>
#include <cstring>
#include <sstream>
#include "Complaint_Base.h"
#include "Complaint_E.h"
#include "test_helpers.h"

TEST(ServerComplaintETest, AnswerAndView)
{
    const std::string file = test_helpers::tempFile("complaints_e.bin");
    test_helpers::removeIfExists(file);

    Complaint_Base base(file);
    const char client[] = "Eve";
    const char text[] = "Missing product";
    ASSERT_TRUE(base.write(text, client));

    Complaint_E employee(file);
    const char answer[] = "We are investigating";
    EXPECT_TRUE(employee.answer(answer, client));

    std::ostringstream os;
    employee.view(true, os);
    EXPECT_NE(std::string::npos, os.str().find("investigating"));

    employee.clear();
    test_helpers::removeIfExists(file);
}

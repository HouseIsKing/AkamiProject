// Filename: TestShortner.cpp
// Created by shiningblack on 4/2/25.
// Reviewer:
// Review Status: 
//

#include <assert.h>

#include "Shortner.hpp"

void TestShortner()
{
    std::string s = "Hello World";
    try
    {
        Shortner::GetInstance().Longen(s);
        //should not come here
        assert(false);
    }
    catch(...)
    {
    }

    std::string val = Shortner::GetInstance().Shorten(s);
    const std::string expected = "Hello World";
    val = Shortner::GetInstance().Longen(val);
    assert(val == expected);

    (void)Shortner::GetInstance().Longen(Shortner::GetInstance().Shorten(""));
}

int main()
{
    TestShortner();
    return 0;
}

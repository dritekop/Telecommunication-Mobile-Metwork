#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <NetConfAgentMock.hpp>
#include <MobileClient.hpp>

class MobileClientTest : public testing::Test
    {
    protected:
    void SetUp() override
    {
        NetConfAgentMock mock;
        _mobileClient = std::make_unique<mobileclient::MobileClient>(mock);
    }

    std::unique_ptr<mobileclient::MobileClient> _mobileClient;
    testing::StrictMock<NetConfAgentMock> _mock;
};

using ::testing::Return;

TEST_F(MobileClientTest, shouldSuccedToDoSomething)
{
    std::string guestXpathState;
    std::string guestValueState;
    std::map<std::string, std::string> XpathValue = {
        {guestXpathState, guestValueState},
    };
    EXPECT_CALL(_mock, fetchData(XpathValue))
                .WillOnce(Return(false));
    _mobileClient->call("+380911111111");
    // TO DO EXPECT_TRUE();
}

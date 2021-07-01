#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <NetConfAgentMock.hpp>
#include <MobileClient.hpp>

class MobileClientTest : public testing::Test
{
protected:
    void SetUp() override
    {
        auto tempMock = std::make_unique<testing::StrictMock<NetConfAgentMock>>();
        _mock = tempMock.get();
        _mobileClient = std::make_unique<mobileclient::MobileClient>(std::move(tempMock));
    }

    std::unique_ptr<mobileclient::MobileClient> _mobileClient;
    testing::StrictMock<NetConfAgentMock> *_mock;
};

using ::testing::Return;
using ::testing::StrictMock;
using ::testing::AtLeast;
using ::testing::_;

TEST_F(MobileClientTest, shouldSucceedToSetName)
{
    _mobileClient->setName("Edo");
}

TEST_F(MobileClientTest, shouldSucceedToHandleModuleChange)
{
    _mobileClient->handleModuleChange("shit");
}

TEST_F(MobileClientTest, shouldSucceedToHandleOperData)
{
    std::string one;
    std::string two;
    _mobileClient->handleOperData(one, two);
}

TEST_F(MobileClientTest, shouldSuccedToCall)
{
    // std::string one = "/mobile-network:core/subscribers[number='+380911111111']/state";
    // std::string two;
    // std::map<std::string, std::string> mapP = {
    //     {one, two},
    // };
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->call("+380911111111");
}

TEST_F(MobileClientTest, shouldSucceedToReject)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->reject();
}

TEST_F(MobileClientTest, shouldSucceedToStopClient)
{
    EXPECT_CALL(*_mock, closeSys());
    _mobileClient->stopClient();
}

TEST_F(MobileClientTest, shouldSucceedToUnregister)
{
    EXPECT_CALL(*_mock, changeData(_, _));
    EXPECT_CALL(*_mock, closeSys());
    _mobileClient->unregisterClient();
}

TEST_F(MobileClientTest, shouldSucceedToChangeData)
{
    std::string one = "/mobile-network:core/subscribers[number='']";
    std::string two;
    EXPECT_CALL(*_mock, changeData(one, two));
    EXPECT_CALL(*_mock, closeSys());
    _mobileClient->unregisterClient();
}

// int main() {
//     RUN_ALL_TESTS();    
//     in the testlog.txt 
//     IMPORTANT NOTICE - DO NOT IGNORE:
//     This test program did NOT call testing::InitGoogleTest() before calling RUN_ALL_TESTS(). 
//     This is INVALID. Soon Google Test will start to enforce the valid usage. Please fix it ASAP, or IT WILL START TO FAIL.  
// }

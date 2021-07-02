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

using ::testing::_;

TEST_F(MobileClientTest, shouldSucceedToSetName)
{
    _mobileClient->setName("Edo");
}

TEST_F(MobileClientTest, shouldSucceedToRegister)
{
    EXPECT_CALL(*_mock, initSysrepo());
    EXPECT_CALL(*_mock, fetchData(_));
    EXPECT_CALL(*_mock, registerOperData(_, _));
    EXPECT_CALL(*_mock, changeData(_, _));
    EXPECT_CALL(*_mock, subscribeForModelChanges(_, _));
    _mobileClient->registerClient("+380911111111");
}

TEST_F(MobileClientTest, shouldSucceedToUnregister)
{
    EXPECT_CALL(*_mock, changeData(_, _));
    EXPECT_CALL(*_mock, closeSys());
    _mobileClient->unregisterClient();
}


TEST_F(MobileClientTest, shouldSuccedToCall)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->call("+380911111111");
}

TEST_F(MobileClientTest, shouldSuccedToAnswer)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->answer();
}

TEST_F(MobileClientTest, shouldSucceedToReject)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->reject();
}

TEST_F(MobileClientTest, shouldSuccedToCallEnd)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->callEnd();
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

TEST_F(MobileClientTest, shouldSucceedToStopClient)
{
    EXPECT_CALL(*_mock, closeSys());
    _mobileClient->stopClient();
}

// int main() {
//     RUN_ALL_TESTS();    
//     in the testlog.txt 
//     IMPORTANT NOTICE - DO NOT IGNORE:
//     This test program did NOT call testing::InitGoogleTest() before calling RUN_ALL_TESTS(). 
//     This is INVALID. Soon Google Test will start to enforce the valid usage. Please fix it ASAP, or IT WILL START TO FAIL.  
// }

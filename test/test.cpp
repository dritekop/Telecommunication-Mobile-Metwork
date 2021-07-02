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

    void userAlreadyCreated() {
        _mobileClient->registerClient("+380911111111");
    }

    std::unique_ptr<mobileclient::MobileClient> _mobileClient;
    testing::StrictMock<NetConfAgentMock> *_mock;
};

using ::testing::_;
using ::testing::Return;

TEST_F(MobileClientTest, shouldSucceedToCreate)
{
    mobileclient::MobileClient user;
}

TEST_F(MobileClientTest, shouldSucceedToSetName)
{
    _mobileClient->setName("Edo");
}

TEST_F(MobileClientTest, shouldSucceedToRegister)
{
    EXPECT_CALL(*_mock, initSysrepo());
    std::string one = "/mobile-network:core/subscribers[number='+380911111111']/state";
    std::string two;
    std::map<std::string, std::string> testMap = {
        {one, two}
    };
    EXPECT_CALL(*_mock, fetchData(testMap));
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


TEST_F(MobileClientTest, shouldFailToCall)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->call("+380911111111");
}

TEST_F(MobileClientTest, shouldFailToAnswer)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->answer();
}

TEST_F(MobileClientTest, shouldSucceedToAnswer)
{
    _mobileClient->handleModuleChange("active");
    EXPECT_CALL(*_mock, fetchData(_));
    EXPECT_CALL(*_mock, changeData(_, _))
            .Times(2);
    _mobileClient->answer();
}

TEST_F(MobileClientTest, shouldFailToReject)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->reject();
}

TEST_F(MobileClientTest, shouldSucceedToReject)
{
    _mobileClient->handleModuleChange("active");
    EXPECT_CALL(*_mock, fetchData(_));
    EXPECT_CALL(*_mock, changeData(_, _))
            .Times(4);
    _mobileClient->reject();
}

TEST_F(MobileClientTest, shouldFailToCallEnd)
{
    EXPECT_CALL(*_mock, fetchData(_));
    _mobileClient->callEnd();
}

TEST_F(MobileClientTest, shouldSuccedToCallEnd)
{
    _mobileClient->handleModuleChange("busy");
    EXPECT_CALL(*_mock, fetchData(_));
    EXPECT_CALL(*_mock, changeData(_, _))
            .Times(4);
    _mobileClient->callEnd();
}

TEST_F(MobileClientTest, shouldSucceedToHandleOperData)
{
    std::string one;
    std::string two;
    _mobileClient->handleOperData(one, two);
}

TEST_F(MobileClientTest, shouldFailToStopClient)
{
    EXPECT_CALL(*_mock, closeSys());
    _mobileClient->stopClient();
}

TEST_F(MobileClientTest, shouldSucceedToStopClient)
{
    _mobileClient->handleModuleChange("busy");
    _mobileClient->stopClient();
}

// int main(int argc, char** argv) {
//     testing::InitGoogleTest();
//     if (RUN_ALL_TESTS()) 
//         std::cout << "To avoid compiler warning about unused returning int value from the RUN_ALL_TESTS()\n";    
//     // use this main function if 'main : true' isn't set in the gtest dependency 
// }

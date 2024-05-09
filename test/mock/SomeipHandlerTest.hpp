#ifndef SOMEIP_HANDLER_TEST_HPP
#define SOMEIP_HANDLER_TEST_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vsomeip/vsomeip.hpp>
#include <memory>
#include <vector>
#include "SomeipHandler.hpp"
#include "MessageTranslator.hpp"
#include "MockSomeipInterface.hpp"
#include "MockSomeipRouterInterface.hpp"
#include <VsomeipUTransport.hpp>
#include <up-cpp/transport/builder/UAttributesBuilder.h>
#include <up-core-api/uri.pb.h>
#include <up-cpp/uuid/factory/Uuidv8Factory.h>
#include "UURIHelper.hpp"

using ::testing::Return;
using namespace uprotocol::utransport;
using namespace uprotocol::uuid;
using namespace uprotocol::v1;
using ::testing::NiceMock;
using namespace up::vsomeip_client;
using ::testing::_;
using namespace vsomeip_v3;

/**
 *  @brief SomeipHandler test fixture for a SomeipHandler Server type.
 */
class SomeipHandlerServerTests : public ::testing::Test {
protected:
    /**
     *  @brief Mocks for SomeipHandler.
     */
    ::testing::NiceMock<MockSomeipInterface> mockSomeipInterface;
    ::testing::NiceMock<MockSomeipRouterInterface> mockRouterInterface;
    /**
     *  @brief SomeipHandler object for testing.
     */
    std::unique_ptr<SomeipHandler> handlerServer;
    /**
     *  @brief UEntity and UAuthority objects for initializing handler.
     */
    UEntity uEntity;
    UAuthority uAuthority;

    /**
     *  @brief Setup for SomeipHandler. Initializes required variables.
     */
    void SetUp() override;

    /**
     *  @brief Teardown for SomeipHandler.
     */
    void TearDown() override;
};

/**
 *  @brief SomeipHandler test fixture for a SomeipHandler client type.
 */
class SomeipHandlerClientTests : public ::testing::Test {
protected:
    /**
     *  @brief Mocks for SomeipHandler.
     */
    ::testing::NiceMock<MockSomeipInterface> mockSomeipInterface;
    ::testing::NiceMock<MockSomeipRouterInterface> mockRouterInterface;
    /**
     *  @brief SomeipHandler object for testing.
     */
    std::unique_ptr<SomeipHandler> handlerClient;
    /**
     *  @brief UEntity and UAuthority objects for initializing handler.
     */
    UEntity uEntity;
    UAuthority uAuthority;

    /**
     *  @brief Setup for SomeipHandler. Initializes required variables.
     */
    void SetUp() override;
    
    /**
     *  @brief Teardown for SomeipHandler.
     */
    void TearDown() override;

    /**
     *  @brief Create a UMessage object.
     */
    std::unique_ptr<SomeipHandler::QItem> createQItem(HandlerMsgType type, unsigned long messageData, std::shared_ptr<void> uMsgPtr) {
        return std::make_unique<SomeipHandler::QItem>(type, messageData, uMsgPtr);
    }

    /**
     *  @brief Getters/setters for private methods and variables in SomeipHandler.
     */
    bool getDoesInboundSubscriptionExist(eventgroup_t const eventGroup) {
        return handlerClient->doesInboundSubscriptionExist(eventGroup);
    }

    void gethandleOfferUResource(std::shared_ptr<UUri> const uriPtr) {
        handlerClient->handleOfferUResource(uriPtr);
    }

    void gethandleInboundSubscription(std::shared_ptr<subscriptionStatus> const subStatus) {
        handlerClient->handleInboundSubscription(subStatus);
    }

    void getActOnBehalfOfSubscriptionAck(eventgroup_t const eventGroup) {
        handlerClient->running_ = true;
        handlerClient->actOnBehalfOfSubscriptionAck(eventGroup);
    }

    size_t getQueueSize() {
        return handlerClient->queue_[0].size();
    }

    bool getaddSubscriptionForRemoteService(UResourceId_t resourceid,
                                            std::shared_ptr<ResourceInformation> resourceInfo) {
        handlerClient->addSubscriptionForRemoteService(resourceid, resourceInfo);

        return handlerClient->subscriptionsForRemoteServices_.find(resourceid) !=
            handlerClient->subscriptionsForRemoteServices_.end();
    }

    bool getRemoveSubscriptionForRemoteService(UResourceId_t resourceid) {
        handlerClient->removeSubscriptionForRemoteService(resourceid);

        return handlerClient->subscriptionsForRemoteServices_.find(resourceid) ==
            handlerClient->subscriptionsForRemoteServices_.end();
    }

    bool getdoesSubscriptionForRemoteServiceExist(eventgroup_t eventGroup) {
        return handlerClient->doesSubscriptionForRemoteServiceExist(eventGroup);
    }

    void getHandleOutboundResponse(std::shared_ptr<UMessage> const message) {
        handlerClient->handleOutboundResponse(message);
    }

    void addToUuidToSomeipRequestLookup(std::string strUUID, std::shared_ptr<message> msg) {
        handlerClient->uuidToSomeipRequestLookup_.insert({strUUID, msg});
    }

    void getHandleSubscriptionRequestForRemoteService(std::shared_ptr<uprotocol::utransport::UMessage> const uMsg) {
        handlerClient->handleSubscriptionRequestForRemoteService(uMsg);
    }

    void getHandleOutboundMsg(std::shared_ptr<UMessage> const uMsg) {
        handlerClient->handleOutboundMsg(uMsg);
    }
    void getProcessMessage(std::unique_ptr<SomeipHandler::QItem>& item) {
        handlerClient->processMessage(std::move(item));
    }

    void getHandleInboundSubscriptionAck(std::shared_ptr<subscriptionStatus> const subStatus) {
        handlerClient->handleInboundSubscriptionAck(subStatus);
    }

    void addToSomeipReqIdToUTransportRequestLookup(request_t requestId, std::shared_ptr<UMessage> uMsgPtr) {
        handlerClient->someipReqIdToUTransportRequestLookup_.insert({requestId, uMsgPtr});
    }

    void setIsReadable(bool isReadable) {
        handlerClient->isServiceAvailable_.setValueAndNotify(isReadable);
    }

    bool getIsReadable() {
        return handlerClient->isServiceAvailable_.isReadable();
    }

    bool doesRequestExist(request_t requestId) {
        return handlerClient->someipReqIdToUTransportRequestLookup_.find(requestId) !=
            handlerClient->someipReqIdToUTransportRequestLookup_.end();
    }

    void getQuit() {
        handlerClient->quit();
    }

    void setRunning(bool value) {
       handlerClient->running_ = value;
    }

    std::vector<uint8_t> getBuildSomeipPayloadFromUMessage(const UMessage &uMessage) {
        return handlerClient->buildSomeipPayloadFromUMessage(uMessage);
    }

    void getHandleInboundMsg(std::shared_ptr<message> const msg){
        handlerClient->handleInboundMsg(msg);
    }

/**
 *  @brief Member variables for testing.
 */
static service_t const service = 0x1234;
static eventgroup_t const eventGroup = 0x0102;
};

/**
 *  @brief Create a UResource object.
 */
std::unique_ptr<UResource> createUResource();

/**
 *  @brief Build a UAttributes object.
 */
UAttributes createUAttributes(UMessageType type = UMESSAGE_TYPE_PUBLISH);

/**
 *  @brief Create a message object for testing.
 */
std::shared_ptr<vsomeip::message> createMessage();

#endif /* SOMEIP_HANDLER_TEST_HPP */
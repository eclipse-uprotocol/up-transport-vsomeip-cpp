/********************************************************************************
 * Copyright (c) 2024 Contributors to the Eclipse Foundation
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0
 *
 * SPDX-License-Identifier: Apache-2.0
 ********************************************************************************/

#ifndef SOMEIP_HANDLER_HPP
#define SOMEIP_HANDLER_HPP

#include <queue>
#include <up-cpp/transport/UTransport.h>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>
#include <up-client-vsomeip-cpp/utils/Utils.hpp>
#include "SomeipRouterInterface.hpp"
#include "SomeipInterface.hpp"

constexpr uint16_t DEFAULT_PRIORITY_LEVELS = 2U;
using UResourceId_t = uint32_t; //This has to be in sync with type of uprotocol::v1::UResource::id in uri.proto

/**
 *    @brief Handler message types.
 */
enum class HandlerMsgType : uint8_t {
    Outbound = 0,
    Inbound,
    InboundSubscriptionAck,
    InboundSubscription,
    Stop,
    OfferUResource
};

/**
 *    @brief Handler type.
 */
enum class HandlerType : uint8_t {
    Undefined = 0,
    Client,
    Server
};

/**
 *    @brief ResourceInformation.
 */
class ResourceInformation final {
private:
    /**
     * @brief UResource  proto object
    */
    uprotocol::v1::UResource resourceInfo_;
    /**
     * @brief subscriberCount_ : This count will hold number of active subscribers
     */
    uint16_t subscriberCount_;

public:
    /**
     * @brief Construct a new Listener Info object
     *
     * @param resourceIdentifier
     * @param resourceName
     * @param resourceInstance
     */
    ResourceInformation(
        const uprotocol::v1::UResource &resourceInformation) :
            resourceInfo_(resourceInformation),
            subscriberCount_(0U) {}
    /**
     * @brief Get the UResource proto object reference
     *
     * @return UResource proto object reference
     */
    const uprotocol::v1::UResource& getUResource() const noexcept {
        return resourceInfo_;
    }

    /**
     * @brief Check if subscription exists for this resource
     *
     * @return Returns true if subscription exists and false otherwise
     */
    bool doesSubscriptionExist() const noexcept {
        return (subscriberCount_ > 0U) ;
    }

    /**
     * @brief increment the subscriber count
     */
    void addSubscriber() noexcept {
        subscriberCount_++;
        SPDLOG_INFO("{} Current subscription count for event[0x{:x}] is [{}]",
            __FUNCTION__, resourceInfo_.id(), subscriberCount_);
    }

    /**
     * @brief decrement the subscriber count
     */
    void removeSubscriber() noexcept {
        if(subscriberCount_ > 0U) {
            subscriberCount_--;
        }
        SPDLOG_INFO("{} Current subscription count for event[0x{:x}] is [{}]",
            __FUNCTION__, resourceInfo_.id(), subscriberCount_);
    }

    uint16_t getSubscriberCount() {
        return subscriberCount_;
    }
};


/**
 *    @brief ListenerInformation.
 */
class ListenerInformation final {
private:
    /**
     * @brief uri
     */
    std::shared_ptr<uprotocol::v1::UUri> uri_;
    /**
     * @brief listener
     */
    uprotocol::utransport::UListener const &listener_;

public:
    /**
     * @brief Construct a new Listener Info object
     *
     * @param uri
     * @param listener
     */
    ListenerInformation(
        std::shared_ptr<uprotocol::v1::UUri> uri,
        uprotocol::utransport::UListener const &listener) :
            uri_(uri),
            listener_(listener) {}
    /**
     * @brief Get the Uri object
     *
     * @return UUri&
     */
    uprotocol::v1::UUri &getUri() const noexcept {
        return *uri_;
    }
    /**
     * @brief Get the Uri object
     *
     * @return UUri&
     */
    std::shared_ptr<uprotocol::v1::UUri> getUriPtr() const noexcept {
        return uri_;
    }

    /**
     * @brief Get the Listener object
     *
     * @return UListener&
     */
    uprotocol::utransport::UListener const &getListener() const noexcept {
        return listener_;
    }
};

/**
 *    @brief Subscription status.
 */
struct subscriptionStatus {
    /**
     *    @brief Subscription status.
     */
    bool isSubscribed;
    /**
     *    @brief Subscription event group.
     */
    vsomeip_v3::eventgroup_t eventgroup;
};

/**
 *    @brief Handler for SomeIp messages. Adds messages to queues,
 *           starts/stops threads, and handles message routing.
 */
#ifdef BUILD_TESTING
class SomeipHandler {
    friend class SomeipHandlerServerTests;
    friend class SomeipHandlerClientTests;
#else
class SomeipHandler final {
#endif //BUILD_TESTING
public:
    /**
     *  @brief     Constructor for SomeipHandler.
     *
     *  @param[in] someIpLibInterface - The SomeIPInterface object used for communication with the SomeIp stack.
     *  @param[in] someIpRouterLibInterface - Template for plugins that has apis exposed to the Message Router.
     *  @param[in] someIpRouterHandlerType - tracks if handler is client or server based.
     *  @param[in] uEntityInfo - uEntity proto object.
     *  @param[in] instanceIdentifier - Service instance identifier.
     *  @param[in] qPriorityLevels - queue priority levels.
     */
    SomeipHandler(
        SomeipInterface& someIpLibInterface,
        SomeipRouterInterface& someIpRouterLibInterface,
        HandlerType const someIpRouterHandlerType,
        uprotocol::v1::UEntity const &uEntityInfo,
        uprotocol::v1::UAuthority const &uAuthorityInfo,
        vsomeip_v3::instance_t const instanceIdentifier=vsomeip_v3::ANY_INSTANCE,
        uint16_t qPriorityLevels=DEFAULT_PRIORITY_LEVELS);

    /**
     * @brief queue offer resource
     *
     * @param UUri shared pointer
     */
    void queueOfferUResource(std::shared_ptr<uprotocol::v1::UUri> uriPtr);
    /**
     *  @brief     Add outbound message to queue.
     *
     *  @param[in] msg - message to be placed in queue.
     */
    void queueOutboundMsg(const uprotocol::utransport::UMessage &msg);
    /**
     *  @brief  Start a new handler thread.
     *
     *  @return bool - true if a new thread was created.
     */
    bool startThread();
    /**
     *  @brief Stop thread.
     */
    void stopThread() noexcept;

    /**
     *  @brief  Check if handler is running.
     *
     *  @return bool - true if handler is running.
     */
    bool isRunning() const noexcept { return running_; }

    /**
     *  @brief     Add message to queue.
     *
     *  @param[in] someIpRHandlerType - tracks if handler is client or server based.
     *  @param[in] data - data to be added to queue.
     *  @param[in] ptr - pointer to message that will be added to queue.
     *  @param[in] priority - queue priority level.
     *
     *  @return bool - true if a message was added to queue.
     */
    bool postMessageToQueue(
        HandlerMsgType someIpRHandlerType,
        unsigned long data=0UL,
        std::shared_ptr<void> ptr = nullptr,
        uint16_t const priority=0U);

    /**
     *  @brief     Add inbound message to queue.
     *
     *  @param[in] msg - message to be placed in queue.
     */
    void onMessage(std::shared_ptr< vsomeip_v3::message > const& msg);
    /**
     *  @brief     Set availability status of SomeipHandler.
     *
     *  @param[in] service - service identifier.
     *  @param[in] instance - service instance identifier.
     *  @param[in] isAvailable - availability status of SomeipHandler.
     */
    void onAvailability(vsomeip_v3::service_t service, vsomeip_v3::instance_t instance, const bool isAvailable);
    /**
     *  @brief     Called when subscription is requested.
     *
     *  @param[in] clientId - client identifier.
     *  @param[in] secClient - vSomeIp sec client type.
     *  @param[in] eventIdStr - eventId for which subscription is received.
     *  @param[in] isSubscribed - subscription status.
     *
     *  @return bool - true after subscription is called.
     */
    bool onSubscription(
        vsomeip_v3::client_t const clientId,
        secClientType const* const secClient,
        std::string const& eventIdStr,
        bool const isSubscribed) noexcept;
    /**
     *  @brief     Callback function for ACK of a message. When a message is sent,
     *             an ACK is added to the stack.
     *             The ACK verifies whether or not the message was received successfully.
     *
     *  @param[in] service - service identifier.
     *  @param[in] instance - service instance identifier.
     *  @param[in] eventGroup - subscription event group.
     *  @param[in] event - unused parameter.
     *  @param[in] status - subscription status.
     */
    void onSubscriptionStatus(
        vsomeip_v3::service_t const service,
        vsomeip_v3::instance_t const instance,
        vsomeip_v3::eventgroup_t const eventGroup,
        vsomeip_v3::event_t const event,
        uint16_t const status);

protected:

    /**
      * @brief This function is used to offer event for given service. This is needed step for inbound subscription.
      *
      * @param[in] eventGroupSetPtr Set of vsomeip_v3::eventgroup_t. Each element is event group ID to offer
      */
      void offerEvent(std::shared_ptr<std::set<vsomeip_v3::eventgroup_t>> eventGroupSetPtr);

    /**
     * @brief This struct is used to store the message in the queue.
     */
    class QItem final {
    public:
        /**
         *  @brief     Stores messages in queue.
         *
         *  @param[in] type - tracks if handler is client or server based.
         *  @param[in] messageData - message data.
         *  @param[in] messagePtr - message for queue.
         */
        QItem(
            HandlerMsgType const type,
            unsigned long const messageData=0UL,
            std::shared_ptr<void> messagePtr = nullptr) :
                msgType_(type),
                data_(messageData),
                ptr_(messagePtr) {}

        /**
         *  @brief  getter for msgType_.
         *
         *  @return HandlerMsgType - tracks if handler is client or server based.
         */
        HandlerMsgType getMsgType() const noexcept {
            return msgType_;
        }
        /**
         *  @brief  getter for ptr_.
         *
         *  @return shared_ptr - message for queue.
         */
        std::shared_ptr<void> getPtr() const {
            return ptr_;
        }

    private:
        /**
         *  @brief tracks if handler is client or server based.
         */
        HandlerMsgType msgType_;
        /**
         *  @brief message data.
         */
        unsigned long data_ = 0UL;
        /**
         *  @brief message for queue.
         */
        std::shared_ptr<void> ptr_ = nullptr;
    };

    /**
     *  @brief     Process message based on message type.
     *
     *  @param[in] item - queue message for processing.
     */
    void processMessage(std::unique_ptr<QItem>const item);
    /**
     *  @brief Quit thread and wake up worker. Called from derived class to ensure background thread is joined
     *         before derived orject gets destroyed.
     */
    void quit();

private:
    /**
     *  @brief Process a message with thread safety.
     */
    void executor();
    /**
     *  @brief Empty the queue.
     */
    void flush();
	/**
     *  @brief     Handle outbound message based on type.
     *
     *  @param[in] uMsg - message for handling.
     */
    void handleOutboundMsg(std::shared_ptr<uprotocol::utransport::UMessage> const uMsgPtr);
    /**
     *  @brief     Handle inbound message based on type.
     *
     *  @param[in] msg - message for handling.
     */
    void handleInboundMsg(std::shared_ptr<vsomeip_v3::message> const msg);
    /**
     *  @brief     Route subscription ack message through UTransport if the event group of the message
     *             is found in the map.
     *
     *  @param[in] subStatus - message for handling. Contains eventgroup and subscription status.
     */
    void handleInboundSubscriptionAck(std::shared_ptr<subscriptionStatus> const subStatus);

    /* Inbound message handlers */
    /**
     *  @brief     Handle inbound notifications by changing them to a UMessage type and then routing them.
     *
     *  @param[in] sMsg - message for handling.
     */
    void handleInboundNotification(std::shared_ptr<vsomeip_v3::message> sMsg);
    /**
     *  @brief     Handle inbound requests by changing them to a UMessage type,
     *             adding them to the someIp request map, and then routing them.
     *
     *  @param[in] sMsg - message for handling.
     */
    void handleInboundRequest(std::shared_ptr<vsomeip_v3::message> sMsg);
    /**
     *  @brief     Handle inbound response by retrieving the original request message,
     *             changing the message to a UMessage type, and then routing the message.
     *
     *  @param[in] sMsg - message for handling.
     */
    void handleInboundResponse(std::shared_ptr<vsomeip_v3::message> sMsg);
    /**
     *  @brief     Handle inbound subscription. In the future, it will ensure the message is the right type,
     *             and then route the message.
     *
     *  @param[in] subStatus - subscription status (subscribed or unsubscribed) and event groupID.
     */
    void handleInboundSubscription(std::shared_ptr<subscriptionStatus> const subStatus);

    /* Outbound message handlers */
    /**
     *  @brief     Handle Outbound notification.
     *
     *  @param[in] uMsg - message for handling.
     */
    void handleOutboundNotification(std::shared_ptr<uprotocol::utransport::UMessage> const uMsgPtr);
    /**
     *  @brief     Handles an outbound request by changing the uMsg to a SomeIp message,
     *             sending it using the SomeIp interface, and updating the request-to-UUID mapping.
     *
     *  @param[in] uMsg - contains the outbound request.
     */
    void handleOutboundRequest(std::shared_ptr<uprotocol::utransport::UMessage> const uMsgPtr);
    /**
     *  @brief     Handles the outbound response messages by creating a response message, setting the payload,
     *             and sending the reponse.
     *
     *  @param[in] uMsg - contains the outbound response.
     */
    void handleOutboundResponse(std::shared_ptr<uprotocol::utransport::UMessage> const uMsgPtr);
    /**
     *  @brief     Handles outbound subscriptions by sending a request to the eventgroup,
     *             and adds a subscription entry if the subscription doesn't already exist.
     *             If the subscription exists, adds a new UUID to the map, and simulates a subscription ack.
     *
     *  @param[in] uMsg - contains the outbound subscription details.
     */
    void handleSubscriptionRequestForRemoteService(std::shared_ptr<uprotocol::utransport::UMessage> const uMsgPtr);

    /**
     *  @brief     Handles offer resource.
     *
     *  @param[in] UUri - contains UURI pointer.
     */
    void handleOfferUResource(std::shared_ptr<uprotocol::v1::UUri> const listenerInfo);

    /**
     *  @brief     Registers a subscription status handler when a subscription status event is received.
     *             Registers a subscription listener and adds an ack to the stack.
     *
     *  @param[in] eventGroup - the event group to register the handler for.
     */
    void registerSubscriptionStatusHandler(vsomeip_v3::eventgroup_t const eventGroup);
    /**
     *  @brief Registers a message handler for the specific event.
     */
    void registerMessageHandler();
    /**
     *  @brief     Checks if a subscription exists for the given event group.
     *
     *  @param[in] eventGroup - the event group to check for subscription.
     *
     *  @return    bool - true if a subscription exists.
     */
    bool doesSubscriptionForRemoteServiceExist(vsomeip_v3::eventgroup_t const eventGroup);
    /**
     *  @brief     Simulates the acknowledgment of a subscription event.
     *
     *  @param[in] eventGroup The event group identifier.
     */
    void actOnBehalfOfSubscriptionAck(vsomeip_v3::eventgroup_t const eventGroup);
    /**
     *  @brief     Adds a subscription entry to 'subscriptionsForRemoteServices_' with the given eventGroup, uuid, and uPTopic.
     *
     *  @param[in] eventGroup - event group to which the subscription entry belongs.
     *  @param[in] listenerInfo - subscription information.
     */
    void addSubscriptionForRemoteService(
        UResourceId_t resourceId,
        std::shared_ptr<ResourceInformation> resource);
    /**
     * @brief This function is used to remove the subscription entry from 'subscriptionsForRemoteServices_'.
     *
     * @param resourceId
     */
    void removeSubscriptionForRemoteService(UResourceId_t resourceId);
    /**
     * @brief This function is used to check if the event group has atleast one subscription.
     *
     * @param[in] eventGroup - Event Group ID to check for subscription.
     */
    bool doesInboundSubscriptionExist(vsomeip_v3::eventgroup_t const eventGroup);

    /**
     * @brief Create vsomeip Payload message from UMessage
     * @param[in] uMessage - UMessage object
     * @return std::vector<uint8_t> - vsomeip payload
     */
    std::vector<uint8_t> buildSomeipPayloadFromUMessage(const uprotocol::utransport::UMessage &uMessage);

    /**
     *  @brief Mutex for the queue.
     */
    std::mutex qMutex_;
    /**
     *  @brief Condition variable that waits for non empty queue.
     */
    std::condition_variable qCondVar_;
    /**
     *  @brief Interface to interact with SomeIp.
     */
    SomeipInterface& someIpInterface_;
    /**
     *  @brief Interface to interact with the SomeIp router.
     */
    SomeipRouterInterface& routerInterface_;
    /**
     *  @brief Type of the handler.
     */
    HandlerType type_;
    /**
     *  @brief Service instance ID.
     */
    vsomeip_v3::instance_t instanceId_;
    /**
     *  @brief UEntity proto object.
     */
    uprotocol::v1::UEntity uEntityData_;
    /**
     * @brief UAuthority proto object.
     */
    uprotocol::v1::UAuthority uAuthorityData_;
    /**
     *  @brief Is current handler running in a thread
     */
    std::atomic<bool> running_= false;
    /**
     *  @brief Average processing time per queue item in milliseconds.
     */
    uint64_t averageProcessingTimeMs_=0U;
    /**
     *  @brief If queue size exceeds this threshold, then request for more threads.
     */
    uint16_t queueSizeThreshold_=0U;
    /**
     *  @brief Queue of items with different priority levels.
     */
    std::vector<std::queue<std::unique_ptr<QItem>>> queue_ =
            std::vector<std::queue<std::unique_ptr<QItem>>>(DEFAULT_PRIORITY_LEVELS);
     /**
     *  @brief Flag indicating whether the service is available.
     */
    up::vsomeip_client::CompoundVariable <bool> isServiceAvailable_;
	/**
     *  @brief Map of resource ID and ResourceInformation.
     */
	std::unordered_map<UResourceId_t, std::shared_ptr<ResourceInformation>> offeredResources_;
    /**
     *  @brief Map of subscriptions.
     */
    std::unordered_map<UResourceId_t, std::shared_ptr<ResourceInformation>> subscriptionsForRemoteServices_;
    /**
     *  @brief Map someip request id to utransport request.
     */
    std::unordered_map<uint32_t, std::shared_ptr<uprotocol::utransport::UMessage>> someipReqIdToUTransportRequestLookup_;
    /**
     *  @brief Map from UUID to SomeIp request.
     */
    std::unordered_map<std::string, std::shared_ptr<vsomeip_v3::message>> uuidToSomeipRequestLookup_;
};

#endif /* SOMEIP_HANDLER_HPP */

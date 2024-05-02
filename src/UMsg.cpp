#include <up-client-vsomeip-cpp/translation/UMsg.hpp>
#include <up-cpp/uuid/serializer/UuidSerializer.h>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>
#include <sstream>
#include <iostream>

using namespace uprotocol::v1;
using namespace std;
using namespace uprotocol::utransport;

/**
 *  @brief     @see @ref UMsg::UMsg
 *
 *  @param[in] msgUri
 *  @param[in] msgAttributes
 *  @param[in] msgPayload
 */
UMsg::UMsg(
    shared_ptr<UUri> msgUri,
    shared_ptr<UAttributes> msgAttributes,
    shared_ptr<UPayload> msgPayload) : uri_(msgUri),
                                       attrs_(msgAttributes),
                                       payload_(msgPayload),
                                       uEId_(0U),
                                       uEResourceId_(0U),
                                       type_(UMsgType::NotDefined),
                                       isSubscriptionSuccessful_(false),
                                       isResponseReceivedFlag_(false) {

    setUUIDAsStr();
}

/**
 *  @brief  @see @ref UMsg::getPayloadAsStr
 *
 *  @return
 */
std::string UMsg::getPayloadAsStr() const {
    std::string payloadData(static_cast<const char*>(static_cast<const void*>(payload_->data())));
    return payloadData;
}

/**
 *  @brief  @see @ref UMsg::getUri
 *
 *  @return
 */
const UUri& UMsg::getUri() const noexcept {
    return *uri_;
}

/**
 *  @brief  @see @ref UMsg::getPayload
 *
 *  @return
 */
const UPayload& UMsg::getPayload() const noexcept {
    return *payload_;
}

/**
 *  @brief  @see @ref UMsg::getAttributes()
 *
 *  @return
 */
const UAttributes& UMsg::getAttributes() const noexcept {
    return *attrs_;
}

/**
 *  @brief  @see @ref UMsg::isResponseReceived
 *
 *  @return
 */
bool  UMsg::isResponseReceived() {
    std::unique_lock lock(responseReceivedMutex_);
    responseReceivedCV_.wait(lock, [this]() -> bool {
        return isResponseReceivedFlag_;}
    );
    return isResponseReceivedFlag_;
}

/**
 *  @brief     @see @ref UMsg::setSubscriptionResponse
 *
 *  @param[in] isSubscribed
 */
void UMsg::setSubscriptionResponse(bool const isSubscribed) {
    {
        std::lock_guard const lock(responseReceivedMutex_);
        isResponseReceivedFlag_ = true;
        isSubscriptionSuccessful_ = isSubscribed;
    }
    responseReceivedCV_.notify_one();
}

/**
 *  @brief @see @ref UMsg::setResponseReceived
 */
void UMsg::setResponseReceived() {
    {
        std::lock_guard const lock(responseReceivedMutex_);
        isResponseReceivedFlag_ = true;
    }
    responseReceivedCV_.notify_one();
}

/**
 *  @brief @see @ref UMsg::evaluateSubscriptionPayload
 */
void UMsg::evaluateSubscriptionPayload() {
    SPDLOG_INFO("{}", __FUNCTION__);
    std::string payloadData(static_cast<const char*>(static_cast<const void*>(payload_->data())));
    std::stringstream ssPayload(payloadData);
    int iCount = 0;
    while (ssPayload.good()) {
        std::string substr;
        std::stringstream token;
        (void)std::getline(ssPayload, substr, ',');
        token << std::hex << substr;
        if (iCount == 0) {
            token >> uEId_;
        }
        else if (iCount == 1) {
            token >> uEResourceId_;
        } else {
            SPDLOG_WARN("{} Payload data improperly formed !!!", __FUNCTION__);
        }
        iCount++;
    }
    SPDLOG_INFO("{} ueId[0x{:x}], ueResourceId[0x{:x}]", __FUNCTION__, uEId_, uEResourceId_);
}

/**
 *  @brief @see @ref UMsg::setUUIDAsStr
 */
void UMsg::setUUIDAsStr() {
    std::string const retVal= uprotocol::uuid::UuidSerializer::serializeToString(attrs_->id());
    const char *uuidStr = retVal.c_str();
    SPDLOG_INFO("{} setting attributes uuid {}", __FUNCTION__, uuidStr);
    uuId_ = retVal;
}

/**
 *  @brief @see @ref UMsg::findMsgTypeFromUprotocol
 */
void UMsg::findMsgTypeFromUprotocol() noexcept {
    //TODO - Implement the ability to find the type of message based on the attributes
    //       of the message.
    type_ = UMsgType::InSubscription;
}

/**
 *  @brief  @see @ref UMsg::getPublishTopicFromSubscriptionPayload
 *
 *  @return
 */
std::string UMsg::getPublishTopicFromSubscriptionPayload() const {
    std::string payloadData(static_cast<const char*>(static_cast<const void*>(payload_->data())));
    return payloadData;
}

/**
  * @brief This function is used to set uE ID
  * @param uEId
  */
void UMsg::setUEID(uint16_t const &uEId) {
    uEId_ = uEId;
}

/**
 *  @brief     @see @ref UMsgTypeToStr()
 *
 *  @param[in] type
 *
 *  @return
 */
std::string UMsgTypeToStr(UMsgType const type) {
    switch (type) {
        case UMsgType::InNotification: {
            return "InNotification";
            break;
        }
        case UMsgType::InRequest: {
            return "InRequest";
            break;
        }
        case UMsgType::InResponse: {
            return "InResponse";
            break;
        }
        case UMsgType::InSubscription: {
            return "InSubscription";
            break;
        }
        case UMsgType::OutNotification: {
            return "OutNotification";
            break;
        }
        case UMsgType::OutRequest: {
            return "OutRequest";
            break;
        }
        case UMsgType::OutResponse: {
            return "OutResponse";
            break;
        }
        case UMsgType::OutSubscription: {
            return "OutSubscription";
            break;
        }
        default: {
            return "Unknown";
            break;
        }
    }
}


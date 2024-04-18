#ifndef UMSG_HPP
#define UMSG_HPP
#include <string>
#include <memory>
#include <condition_variable>

#include <up-cpp/transport/UTransport.h>

/**
 * @brief UMsg type.
 */
enum class UMsgType : uint8_t {
    NotDefined = 0,
    InNotification,
    InRequest,
    InResponse,
    InSubscription,
    InSubscriptionAck,

    OutPublish,
    OutNotification,
    OutRequest,
    OutResponse,
    OutSubscription
};

/**
 *  @brief  Converts a UMsgType enum value to a string.
 *
 *  @return string - UMsgType converted to a string.
 */
std::string UMsgTypeToStr(UMsgType const);

/**
 *  @brief UMsg encapsulates the details of a message in the uProtocol.
 */
class UMsg final {
public:
    /**
     *  @brief     UMsg constructor with values needed for a message.
     *
     *  @param[in] msgUri - URI of the UMsg. UUri is a data representation of uProtocol
     *                      containing the source and sink portion.
     *  @param[in] msgAttributes - Attributes of the UMsg.
     *  @param[in] msgPayload - Payload of the UMsg.
     */
    UMsg(
        std::shared_ptr<uprotocol::v1::UUri> msgUri,
        std::shared_ptr<uprotocol::v1::UAttributes> msgAttributes,
        std::shared_ptr<uprotocol::utransport::UPayload> msgPayload);
    /**
     *  @brief  Getter for uEId_.
     *
     *  @return uint16_t - Ultifi entity identifier.
     */
    uint16_t getUeId() const noexcept {
        return uEId_;
    }
    /**
     *  @brief  Getter for uEResourceId_.
     *
     *  @return uint16_t - Ultifi entity resource identifier.
     */
    inline uint16_t getResourceId() const noexcept {
        return uEResourceId_;
    }

    /**
     * @brief  Getter for uri_ as a shared pointer.
     * @return shared pointer to UUri.
     */
    const std::shared_ptr<uprotocol::v1::UUri> getUriAsPtr() const noexcept {
        return uri_;
    }

    /**
     *  @brief  Returns the raw payload data converted to a string.
     *
     *  @return string - Raw payload data in string format.
     */
    std::string getPayloadAsStr() const;
    /**
     *  @brief  Getter for uri_.
     *
     *  @return UUri - URI of the UMsg.
     */
    const uprotocol::v1::UUri& getUri() const noexcept;
    /**
     *  @brief  Getter for payload_.
     *
     *  @return UPayload - Payload of the UMsg.
     */
    const uprotocol::utransport::UPayload& getPayload() const noexcept;
    /**
     *  @brief  Getter for attrs_.
     *
     *  @return UAttributes - Attributes of the UMsg.
     */
    const uprotocol::v1::UAttributes& getAttributes() const noexcept;


    /**
     *  @brief This function is used to evaluate the payload of a subscription
     *  request. It is used to extract the ueID/service id and
     *  ue resource id/event group id from the payload
     *  And then update the attributes of the UMsg object.
     *  sample subscription payload (temporary): "ueId,ueResourceId"
     */
    //TODO temporarily using a string to serialize the payload
    //     which has comma separated values of ueId and ueResourceId.
    void evaluateSubscriptionPayload();
    /**
     *  @brief This function is used to find the type of the message
     *  based on the attributes of the message and populate the
     *  type_ member variable
     */
    void findMsgTypeFromUprotocol() noexcept;

    /**
     *  @brief This function is used to set the type of the message
     *  externally.
     *
     *  @param msgType - UMsg type.
     */
    inline void setMsgType(UMsgType const msgType) noexcept {
        type_ = msgType;
    };
    /**
     *  @brief Set attributes ptr.
     *
     *  @param attributes - UMsg attributes.
     */
    inline void setAttributes(std::shared_ptr<uprotocol::v1::UAttributes> const attributes) noexcept {
        attrs_ = attributes;
    };
    /**
     *  @brief  Getter for type_.
     *
     *  @return UMsgType - UMsg type.
     */
    inline UMsgType getMsgType() const noexcept {
        return type_;
    };
    /**
     * @brief This function is used to set uE ID
     * @param uEId
    */
    void setUEID(uint16_t const &uEId);

    /**
     * @brief This function is used to set uE Resource ID
     * @param uEResourceId
    */
    void setUEResourceId(const uint16_t &uEResourceId) {
        uEResourceId_ = uEResourceId;
    }

    bool isResponseReceived();
    /**
     *  @brief Sets the response received flag for the UMsg.
     */
    void setResponseReceived();
    /**
     *  @brief Sets the subscription response for the UMsg.
     *
     *  @param isSubscribed - True if subscribed.
     */
    void setSubscriptionResponse(bool const isSubscribed);
    /**
     *  @brief  Getter for uuId_.
     *
     *  @return const string& - Unique identifier of UMsg.
     */
    inline std::string const& getUuid() const noexcept {
        return uuId_;
    };
    /**
     *  @brief Sets the UP topic for the UMsg.
     *
     *  @param topic - uProtocol topic for UMsg.
     */
    inline void setUPTopic(std::string const& topic) {
        uPTopic_ = topic;
    };
    /**
     *  @brief  Getter for uPTopic_.
     *
     *  @return string - uProtcol topic.
     */
    inline std::string const& getUPTopic() const noexcept {
        return uPTopic_;
    };
    /**
     *  @brief  Extracts and returns the publish topic from the subscription payload of the UMsg.
     *
     *  @return string - Publish topic data of UMsg.
     */
    std::string getPublishTopicFromSubscriptionPayload() const;

private:
    /**
     *  @brief Sets the UUID of the UMsg as a string.
     */
	void setUUIDAsStr();
    /**
     *  @brief URI of the UMsg. UUri is a data representation of uProtocol containing the
     *         source and sink portion.
     */
    std::shared_ptr<uprotocol::v1::UUri> uri_;
    /**
     *  @brief Attributes of the UMsg.
     */
    std::shared_ptr<uprotocol::v1::UAttributes> attrs_;
    /**
     *  @brief Payload of the UMsg.
     */
    std::shared_ptr<uprotocol::utransport::UPayload> payload_;
    /**
     *  @brief Unique identifier of UMsg.
     */
    std::string uuId_;
    /**
     *  @brief Ultifi entity identifier.
     */
    uint16_t uEId_;
    /**
     *  @brief Ultifi entity resource identifier.
     */
    uint16_t uEResourceId_;
    /**
     *  @brief UMsg type.
     */
    UMsgType type_;
    /**
     *  @brief uProtocol topic.
     */
    std::string uPTopic_;
    //TODO Value is being updated but currently not used.
    /**
     *  @brief Can be used in UTransportEndpoint to check if the subscription
     *  was successful or not for subscription request.
     */
    bool isSubscriptionSuccessful_;
    /**
     *  @brief Flag indicating whether a response has been received for the UMsg.
     */
    bool isResponseReceivedFlag_;
    /**
     *  @brief Mutex for isResponseReceivedFlag_.
     */
    std::mutex responseReceivedMutex_;
    /**
     *    @brief Used to wait for a response to the UMsg.
     */
    std::condition_variable responseReceivedCV_;
};

#endif /* UMSG_SIM_HPP */
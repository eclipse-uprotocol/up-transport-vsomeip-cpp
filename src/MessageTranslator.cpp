#include <up-client-vsomeip-cpp/translation/MessageTranslator.hpp>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>
#include <up-client-vsomeip-cpp/routing/SomeipInterface.hpp>
#include <up-cpp/uri/serializer/LongUriSerializer.h>
#include <up-cpp/uuid/serializer/UuidSerializer.h>
#include <up-cpp/uuid/factory/Uuidv8Factory.h>
#include <up-cpp/transport/builder/UAttributesBuilder.h>
#include <sstream>

using namespace uprotocol::utransport;
using namespace uprotocol::v1;

/**
 * @brief @see @ref MessageTranslator::MessageTranslator()
 * @param someipInterfaceObj reference to some ip interface used for translating umsg to some ip message
 * and vice versa
 * @param uStreamerObj reference to uStreamer required to stream the translated messages.
 */
MessageTranslator::MessageTranslator(
    SomeipInterface& someipInterfaceObj) :
    someipInterfaceRef_(someipInterfaceObj) {

    LogTrace("{}", __FUNCTION__);

}

/**
 * @brief @return @see @ref MessageTranslator::translateUMessageToSomeipMsgForRequest()
 * @param uMessage, shared pointer to uMessage which is translated to the some ip message.
 */
std::shared_ptr<message> MessageTranslator::translateUMessageToSomeipMsgForRequest(std::shared_ptr<UMessage> uMessage) {

    LogTrace("{}", __FUNCTION__);
    std::shared_ptr<message> someIpMessage = someipInterfaceRef_.createRequest();

    someIpMessage->set_service(static_cast<service_t>(uMessage->attributes().sink().entity().id()));
    someIpMessage->set_method(static_cast<method_t>(uMessage->attributes().sink().resource().id()));
    someIpMessage->set_instance(INSTANCE_ID_PER_SPEC);
    someIpMessage->set_message_type(message_type_e::MT_REQUEST);

    std::shared_ptr<payload> const payload = someipInterfaceRef_.createPayload();
    std::vector<uint8_t> payloadData;
    for(size_t iCount = 0 ; iCount < uMessage->payload().size(); iCount++) {
        payloadData.push_back(uMessage->payload().data()[iCount]);
    }
    payload->set_data(payloadData);
    someIpMessage->set_payload(payload);

    return someIpMessage;
}

/**
  * @brief @return @see @ref MessageTranslator::buildUPayload()
  * @param[in] someIpMessage @see @ref MessageTranslator::buildUPayload()
  */
std::shared_ptr<UPayload> MessageTranslator::buildUPayload(std::shared_ptr<message> const someIpMessage) const {

    const std::shared_ptr<payload> someipPayload = someIpMessage->get_payload();
    std::stringstream its_message;

    for (uint32_t i = 0U; i < someipPayload->get_length(); ++i) {
        its_message << static_cast<int>(someipPayload->get_data()[i]);
    }

    std::shared_ptr<UPayload> uPayload =
        std::make_shared<UPayload>(
            reinterpret_cast<uint8_t const *>(its_message.str().c_str()),
            its_message.str().size() + 1, UPayloadType::VALUE);

    return uPayload;
 }

/**
  * @brief @return @see @ref MessageTranslator::buildUAttribute()
  * @param[in] uriSource @see @ref MessageTranslator::buildUAttribute()
  * @param[in] uriSink @see @ref MessageTranslator::buildUAttribute()
  * @param[in] generatedUUID @see @ref MessageTranslator::buildUAttribute()
  * @param[in] messageType @see @ref MessageTranslator::buildUAttribute()
  */
std::shared_ptr<UAttributes> MessageTranslator::buildUAttribute(
    UUri const &uriSource,
    uprotocol::v1::UUri const &uriSink,
    UUID const &generatedUUID,
    message_type_e const &messageType,
    UPriority priority) const {

    //TODO: Pass correct URI according to 1_5_7 to UAttributesBuilder
    UMessageType const type = convertSomeipToUMessageType(messageType);
    UAttributesBuilder builder(uriSource ,generatedUUID, type, priority);
    builder.setSink(uriSink);
    std::shared_ptr<UAttributes> attributes = std::make_shared<UAttributes>(builder.build());
    return attributes;
}

/**
  * @brief @return @see @ref MessageTranslator::translateSomeipToUMsgForNotification()
  * @param[in] someIpMessage @see @ref MessageTranslator::translateSomeipToUMsgForNotification().
  * @param[in] strTopic @see @ref MessageTranslator::translateSomeipToUMsgForNotification().
*/
std::shared_ptr<UMessage> MessageTranslator::translateSomeipToUMsgForNotification(
    std::shared_ptr<message> const someIpMessage,
    UEntity const &uEntity ,
    UAuthority const &uAuthority,
    UResource const &uResource) const {

    LogInfo("{} serviceid[0x{:x}] instance[0x{:x}] eventgrpid[0x{:x}]",
            __FUNCTION__, someIpMessage->get_service(), someIpMessage->get_instance(), someIpMessage->get_method());

    //build URI
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();
    uriPtr->mutable_authority()->CopyFrom(uAuthority);
    uriPtr->mutable_entity()->CopyFrom(uEntity);
    uriPtr->mutable_resource()->CopyFrom(uResource);

    //build Attribute using the URI
    UUID const &generatedUUID = uprotocol::uuid::Uuidv8Factory::create();
    UMessageType const type = convertSomeipToUMessageType(someIpMessage->get_message_type());
    UAttributesBuilder builder(*uriPtr ,generatedUUID, type, uprotocol::v1::UPriority::UPRIORITY_CS0);
    std::shared_ptr<UAttributes> attributes = std::make_shared<UAttributes>(builder.build());

    //build Payload
    std::shared_ptr<UPayload> uPayload = buildUPayload(someIpMessage);

    //build UMessage using attribute and payload
    std::shared_ptr<UMessage> umsg = std::make_shared<UMessage>();
    umsg->setPayload(*uPayload);
    umsg->setAttributes(*attributes);

    return umsg;

}

/**
  * @brief @return @see @ref MessageTranslator::translateSomeipToUMsgForSubscriptionAck()
  * @param[in] uEntity @see @ref MessageTranslator::translateSomeipToUMsgForSubscriptionAck()
  * @param[in] uAuthority @see @ref MessageTranslator::translateSomeipToUMsgForSubscriptionAck()
  * @param[in] uResource @see @ref MessageTranslator::translateSomeipToUMsgForSubscriptionAck()
  */
std::shared_ptr<UMessage> MessageTranslator::translateSomeipToUMsgForSubscriptionAck(UEntity const &uEntity, UAuthority const &uAuthority ,UResource const &uResource) const {
    // build URI
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();
    uriPtr->mutable_authority()->CopyFrom(uAuthority);
    uriPtr->mutable_entity()->CopyFrom(uEntity);
    uriPtr->mutable_resource()->CopyFrom(uResource);

    // build Attribute
    UUID const &generatedUUID = uprotocol::uuid::Uuidv8Factory::create();
    UMessageType const type = UMessageType::UMESSAGE_TYPE_UNSPECIFIED;
    UAttributesBuilder builder(*uriPtr, generatedUUID, type, uprotocol::v1::UPriority::UPRIORITY_CS0);
    std::shared_ptr<UAttributes> attributes = std::make_shared<UAttributes>(builder.build());

    // build UMessage using attribute and payload
    std::shared_ptr<UMessage> umsg = std::make_shared<UMessage>();
    umsg->setAttributes(*attributes);

    return umsg;
}

/**
  * @brief @return @see @ref MessageTranslator::translateSomeipToUMsgForResponse()
  * @param[in] someIpMessage @see @ref MessageTranslator::translateSomeipToUMsgForResponse().
  * @param[in] uri @see @ref MessageTranslator::translateSomeipToUMsgForResponse().
  * @param[in] uuid @see @ref MessageTranslator::translateSomeipToUMsgForResponse().
  */
std::shared_ptr<UMessage> MessageTranslator::translateSomeipToUMsgForResponse(
    std::shared_ptr<message> const &someIpMessage,
    std::shared_ptr<uprotocol::utransport::UMessage> originalRequestMsg) const {

    std::shared_ptr<UMessage> uMsg = std::make_shared<UMessage>();

    //UUID const &generatedUUID = uprotocol::uuid::Uuidv8Factory::create(); //TODO : Remove this comment once zenoh uTransport fix the issue.

    // Build Payload
    std::shared_ptr<UPayload> uPayload = buildUPayload(someIpMessage);

    // Build Attributes
    //TODO: attributes.id field of response should be newly generated but currently it is not as per uProtocol 1_5_7
    //so need to set original resquet uuid to attributes.id as a workaround. This needs to be fixed in zenoh uTransport
    UMessageType const type = convertSomeipToUMessageType(message_type_e::MT_RESPONSE);
    UAttributesBuilder builder(
        originalRequestMsg->attributes().source() ,
        originalRequestMsg->attributes().id(),
        type, originalRequestMsg->attributes().priority());

    builder.setSink(originalRequestMsg->attributes().sink());
    builder.setReqid(originalRequestMsg->attributes().id());
    std::shared_ptr<UAttributes> attributes = std::make_shared<UAttributes>(builder.build());


    uMsg->setPayload(*uPayload);
    uMsg->setAttributes(*attributes);

    return uMsg;
}

/**
 * @brief @return @see @ref MessageTranslator::translateSomeipToUMsgForRequest()
 * @param[in] someIpMessage @see @ref MessageTranslator::translateSomeipToUMsgForRequest().
 * @param[in] uEntity @see @ref MessageTranslator::translateSomeipToUMsgForRequest().
 * @param[in] uResource @see @ref MessageTranslator::translateSomeipToUMsgForRequest().
 */
std::shared_ptr<UMessage> MessageTranslator::translateSomeipToUMsgForRequest(
    std::shared_ptr<message> const &someIpMessage,
    UEntity const &uEntity,
    UResource const &uResource) const {

    LogInfo("{} serviceid[0x{:x}] methodId[0x{:x}]",
            __FUNCTION__, someIpMessage->get_service(), someIpMessage->get_method());

    UUID const &generatedUUID = uprotocol::uuid::Uuidv8Factory::create();

    std::shared_ptr<UPayload> uPayload = buildUPayload(someIpMessage);

    UAuthority uauthority;
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();
    uriPtr->mutable_authority()->CopyFrom(uauthority);
    uriPtr->mutable_entity()->CopyFrom(uEntity);
    uriPtr->mutable_resource()->CopyFrom(uResource);

    //TODO: Currenty assigning source URI same value as sink URI, since it is not been used anywhere
    //But this needs to be aligned to uattributes.proto definations of source and sink URI.
    std::shared_ptr<UAttributes> attributes =
        buildUAttribute(*uriPtr,
                        *uriPtr,
                        generatedUUID,
                        someIpMessage->get_message_type(),
                        UPriority::UPRIORITY_CS4);

    std::shared_ptr<UMessage> uMessage = std::make_shared<UMessage>();
    uMessage->setPayload(*uPayload);
    uMessage->setAttributes(*attributes);

    return uMessage;
}

/**
  * @brief @return @see @ref MessageTranslator::convertSomeipToUMessageType()
  * @param[in] messageType @see @ref MessageTranslator::convertSomeipToUMessageType()
  */
UMessageType MessageTranslator::convertSomeipToUMessageType(message_type_e const &messageType) const {

    UMessageType type =  UMessageType::UMESSAGE_TYPE_UNSPECIFIED;
    if (messageType == message_type_e::MT_REQUEST) {
        type = UMessageType::UMESSAGE_TYPE_REQUEST;
    } else if (messageType == message_type_e::MT_NOTIFICATION) {
        type = UMessageType::UMESSAGE_TYPE_PUBLISH;
    } else if (messageType == message_type_e::MT_RESPONSE) {
        type = UMessageType::UMESSAGE_TYPE_RESPONSE;
    } else if (messageType == message_type_e::MT_REQUEST_ACK) {
        type = UMessageType::UMESSAGE_TYPE_RESPONSE;
    } else {
        LogErr("{} Undefined message type", __FUNCTION__);
    }

    return type;
}

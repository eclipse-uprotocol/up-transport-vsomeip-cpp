#include "common.h"
#include <up-client-vsomeip-cpp/utils/Utils.hpp>
#include <condition_variable>

const uint16_t UEntityID             = 0x1102; //Service ID
const std::string UEntityName        = "0x1102";
const uint32_t UEntityVersionMajor   = 0x1; //Major Version
const uint32_t UEntityVersionMinor   = 0x0; //Minor Version

const std::string UAuthorityIP       = "172.17.0.1";

const uint16_t UResourceID           = 0x0102; //Method ID
const std::string UResourceName      = "rpc";
const std::string UResourceInstance  = "0x0102";
bool gTerminate = false;

class UEListener;
up::vsomeip_client::CompoundVariable <bool> isResponseAvailable(false);

std::string UMessageTypeToString(UMessageType type) ;
std::shared_ptr<UUri> getListenerURI() ;
void registerListener(UEListener &listner) ;
std::shared_ptr<UUri> buildSrcURI() ;
bool sendRPC(UUri &uri, std::string payloadBuffer);
bool waitForResponse(const std::chrono::milliseconds timeout=std::chrono::milliseconds(60000UL));

UUID requestUuid = Uuidv8Factory::create();

class UEListener : public UListener
{
    UStatus onReceive(UMessage &message) const {

        spdlog::info("type = {} dataPayload = {}",
             UMessageTypeToString(message.attributes().type()),
             reinterpret_cast<const char *>(message.payload().data()));

        UUID responseUuid = message.attributes().reqid();

        if(requestUuid.msb() == responseUuid.msb() && requestUuid.lsb() == responseUuid.lsb()) {
            spdlog::info("request UUID matches with response UUID");
            isResponseAvailable.setValueAndNotify(true);
        } else {
            spdlog::info("request UUID does not matches with response UUID");
        }
        /*
             The Response can be processed here
        */
        UStatus status;
        status.set_code(UCode::OK);
        return status;
    }
};

std::string UMessageTypeToString(UMessageType type) {
    switch (type) {
        case UMessageType::UMESSAGE_TYPE_PUBLISH:
            return "UMESSAGE_TYPE_PUBLISH";
        case UMessageType::UMESSAGE_TYPE_REQUEST:
            return "UMESSAGE_TYPE_REQUEST";
        case UMessageType::UMESSAGE_TYPE_RESPONSE:
            return "UMESSAGE_TYPE_RESPONSE";
        default:
            return "Unknown UMessageType";
    }
}

std::shared_ptr<UUri> getListenerURI() {
    uint32_t streamerUEntityID = 0x1;
    std::string streamerUEntityName = "1";
    uint32_t streamerUResourceID = 0x1;
    std::string streamerUResourceName = "1";

    auto u_entity = uprotocol::uri::BuildUEntity().setId(streamerUEntityID).setName(streamerUEntityName).build();
    auto u_resource = uprotocol::uri::BuildUResource().setName(streamerUResourceName).setID(streamerUResourceID).build();
    auto uri = uprotocol::uri::BuildUUri().setEntity(u_entity).setResource(u_resource).build();

    return std::make_shared<UUri>(uri);
}

void registerListener(UEListener &listner) {
    LogTrace("{}", __FUNCTION__);

    std::shared_ptr<UUri> listenerURI = getListenerURI();

    if (listenerURI != nullptr) {
        if (UCode::OK != VsomeipUTransport::instance().registerListener(*listenerURI, listner).code()) {
            LogErr("VsomeipUTransport::instance().registerListener failed");
        } else {
            LogInfo("VsomeipUTransport::instance().registerListener success");
        }
    }
}

std::shared_ptr<UUri> buildSrcURI() {
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();

    //Create a UEntity
    UEntity uentity;
    UAuthority uauthority;
    UResource uresource;

    uentity.set_id(UEntityID);
    uentity.set_name(UEntityName.c_str());
    uentity.set_version_major(UEntityVersionMajor);
    uentity.set_version_minor(UEntityVersionMinor);

    uriPtr->mutable_entity()->CopyFrom(uentity);

    //Create a UAuthority
    uriPtr->mutable_authority()->CopyFrom(uauthority);

    //Create a UResource
    uresource.set_id(UResourceID);
    uresource.set_name(UResourceName.c_str());
    uresource.set_instance(UResourceInstance);

    uriPtr->mutable_resource()->CopyFrom(uresource);

    //Return UUri
    return uriPtr;
}

bool sendRPC(UUri &uri, std::string payloadBuffer) {
    auto type = UMessageType::UMESSAGE_TYPE_REQUEST;
    int32_t ttl = 10000; //10 secs

    //Create a UAttribute
    UAttributesBuilder builder(uri, requestUuid, type, UPriority::UPRIORITY_CS4);
    builder.setSink(uri);
    builder.setTTL(ttl);
    UAttributes attributes = builder.build();

    //Create a UPayload
    UPayload payload(
        reinterpret_cast<const uint8_t *>(payloadBuffer.c_str()),
        payloadBuffer.size() + 1,
        UPayloadType::VALUE);

    //Create a UMessage
    std::shared_ptr<UMessage> uMsg = std::make_shared<UMessage>(payload, attributes);

    //Send RPC request
    VsomeipUTransport::instance().send(*uMsg);

    //Wait for response
    const std::chrono::milliseconds requestTimeout = std::chrono::milliseconds(ttl);
    return isResponseAvailable.isReadableWithWait(requestTimeout);
}

int main() {
    spdlog::info("start test : uE sends rpc request and mE responds...");

    //Register common listener with up-vsomeip library
    UEListener listener;
    registerListener(listener);

    //Build Source URI for rpc request
    std::shared_ptr<UUri> uri = buildSrcURI();
    std::string payloadBuffer = "123";

    bool response = sendRPC(*uri, payloadBuffer);
    if( response ) {
        spdlog::info("RPC response received");
    } else {
        spdlog::warn("RPC response not received because of timeout");
    }

    spdlog::info("Exiting from vsomeip client...");
    VsomeipUTransport::instance().terminate();
    return 0;
}
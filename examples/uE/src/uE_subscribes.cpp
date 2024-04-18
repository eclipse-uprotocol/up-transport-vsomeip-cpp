#include "common.h"

const uint16_t UEntityID             = 0x1101; //Service ID
const std::string UEntityName        = "0x1101";
const uint32_t UEntityVersionMajor   = 0x1; //Major Version
const uint32_t UEntityVersionMinor   = 0x0; //Minor Version

const std::string UAuthorityIP       = "172.17.0.1"; //IP address of mE

const uint16_t UResourceID           = 0x8101; //Method ID
const std::string UResourceName      = "0x8101";
const std::string UResourceInstance  = "";

bool gTerminate = false;
class UEListener;

std::shared_ptr<UUri> buildSrcURI();
std::shared_ptr<UMessage> buildUMessage();
std::shared_ptr<UUri> getListenerURI();
void registerListener(UEListener &listner);
void signalHandler(int signal);
std::string UMessageTypeToString(UMessageType type);

class UEListener : public UListener
{
    UStatus onReceive(UMessage &message) const {


        spdlog::info("type = {} dataPayload = {}",
             UMessageTypeToString(message.attributes().type()),
             reinterpret_cast<const char *>(message.payload().data()));


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

    UEntity uentity;
    UAuthority uauthority;
    UResource uresource;

    uentity.set_id(UEntityID);
    uentity.set_name(UEntityName.c_str());
    uentity.set_version_major(UEntityVersionMajor);
    uentity.set_version_minor(UEntityVersionMinor);

    uriPtr->mutable_entity()->CopyFrom(uentity);

    uriPtr->mutable_authority()->CopyFrom(uauthority);

    uresource.set_id(UResourceID);
    uresource.set_name(UResourceName.c_str());
    uresource.set_instance(UResourceInstance);

    uriPtr->mutable_resource()->CopyFrom(uresource);
    return uriPtr;
}

std::shared_ptr<UMessage> buildUMessage() {
    //build src URI
    std::shared_ptr<UUri> srcUri = buildSrcURI();

    // Build sink URI
    constexpr int8_t entityID = 0; //ueid
    auto const& uEntity = BuildUEntity().setId(entityID).build();
    UUri const& sinkUri = BuildUUri().setEntity(uEntity).build();

    // Build UAttributes
    auto const& priority = UPriority::UPRIORITY_CS2;
    auto const& uuid = Uuidv8Factory::create();
    auto const& type = UMessageType::UMESSAGE_TYPE_REQUEST;
    UAttributesBuilder builder = UAttributesBuilder(*srcUri,uuid, type, priority);
    builder.setSink(sinkUri);
    UAttributes attributes = builder.build();

   //Build UMessage
    std::shared_ptr<UMessage> uMessage = std::make_shared<UMessage>();
    uMessage->setAttributes(attributes);

    return uMessage;
}

int main() {
    UEListener listener;
    registerListener(listener);

    std::shared_ptr<UMessage> uMsg = buildUMessage();
    std::ignore = VsomeipUTransport::instance().send(*uMsg);

    while (!gTerminate) {
        sleep(1);
    }
    spdlog::info("Exiting from vsomeip client...");
    VsomeipUTransport::instance().terminate();
    return 0;
}
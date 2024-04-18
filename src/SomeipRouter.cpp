#include <up-client-vsomeip-cpp/routing/SomeipRouter.hpp>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>
#include <up-cpp/uri/serializer/LongUriSerializer.h>
#include <up-client-vsomeip-cpp/routing/SomeipWrapper.hpp>
#include <up-client-vsomeip-cpp/routing/MiscConfig.hpp>
#include <fstream>
#include <filesystem>
#include "rapidjson/document.h"

using namespace uprotocol::utransport;
using namespace uprotocol::v1;

/**
 * @brief @see @ref SomeipRouter::SomeipRouter()
 * @param listener @see @ref SomeipRouter::SomeipRouter()
 */
SomeipRouter::SomeipRouter(UListener const &listener) :
    someipInterface_(std::make_shared<SomeipWrapper>()),
    msgTranslator_(std::make_unique<MessageTranslator>(*someipInterface_)),
    listener_(listener) {

    LogTrace("{} SomeipRouter is initialized !!!", __FUNCTION__);
}

/**
 * @brief @see @ref void SomeipRouter::stopService().
 */
void SomeipRouter::stopService(){
        LogTrace("{}", __FUNCTION__);
        someipInterface_->stop();
        someipInterface_.reset();
        msgTranslator_.reset();
}

/**
 * @brief @see @ref SomeipRouter::~SomeipRouter().
 */
SomeipRouter::~SomeipRouter() {
        try {
        stopService();
    } catch(const std::exception& e) {
        LogErr("{} Destructor failed !!!", __FUNCTION__);
    }
}

/**
 * @brief @see @ref SomeipRouter::init()
 *
 * @return
 */
bool SomeipRouter::init() {
    LogTrace("{}", __FUNCTION__);
    if (!someipInterface_->init()) {
        LogErr("{} Someip Interface init failed !!!", __FUNCTION__);
        return false;
    } else {
        LogTrace("{} Registering state handler", __FUNCTION__);
        someipInterface_->registerStateHandler(
            std::bind(&SomeipRouter::onState, this, std::placeholders::_1));
        LogTrace("{} starting someip eventloop", __FUNCTION__);
        someipInterface_->start();
        LogInfo("{} Someip Interface init successful !!!", __FUNCTION__);
    }

    enableAllLocalServices();

    return true;
}

/**
  * @brief @see @ref SomeipRouter::enableAllLocalServices
  */
void SomeipRouter::enableAllLocalServices() {
    std::shared_ptr<std::vector<std::shared_ptr<UUri>>> uriList = getUriList("services");
    if(uriList != nullptr) {
        for (auto it = (*uriList).begin(); it != (*uriList).end(); ++it) {
            offerServicesAndEvents(*it);
            auto strURI = uprotocol::uri::LongUriSerializer::serialize(**it);
            LogInfo("{} Initializing local service with URI[{}]",__FUNCTION__, strURI);
        }
    } else {
        LogErr("Failed to get URI list");
    }
}

/**
  * @brief @see @ref SomeipRouter::offerServicesAndEvents
  */
void SomeipRouter::offerServicesAndEvents(std::shared_ptr<UUri> uriPtr) {
    LogInfo("{}",__FUNCTION__);
    uint16_t ueId = 0U;
    if (uriPtr->entity().has_id()) {
        ueId = static_cast<uint16_t>(uriPtr->entity().id());
    } else {
        LogErr("{} URI does not have a UE-ID", __FUNCTION__);
        return;
    }

    std::shared_ptr<SomeipHandler> hptr = getHandler(ueId);
    HandlerType handlerType = HandlerType::Undefined;

    if (nullptr == hptr) {
        handlerType = HandlerType::Server;
        hptr = newHandler(handlerType, uriPtr->entity(), uriPtr->authority());
        LogDebug("{} New Server handler created for UEID[0x{:x}]", __FUNCTION__, ueId);
    }

    if (nullptr == hptr) {
        // Something is terribly wrong
        LogCrit("{} Handler not found and creation also failed!!!", __FUNCTION__);
        return;
    }
    if (!hptr->isRunning()) {
        LogInfo("{} Handler UEId[0x{:x}] Starting new thread", __FUNCTION__, ueId);
        if(! hptr->startThread()) {
            LogErr("{} Handler UEId[0x{:x}] Failed to start thread.", __FUNCTION__, ueId);
        }
    } else {
        LogInfo("{} Handler UEId[0x{:x}] already running in a thread.", __FUNCTION__, ueId);
    }
    hptr->queueOfferUResource(uriPtr);
}

/**
  * @brief @see @ref SomeipRouter::getUriList
  * @param @return  serviceType - @see @ref SomeipRouter::getUriList
  */
 std::shared_ptr<std::vector<std::shared_ptr<UUri>>> SomeipRouter::getUriList(const std::string &serviceType) {
    LogTrace("{}", __FUNCTION__);
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string jsonDirectoryPath(currentPath.c_str());
    //TODO: Change this path if you need to run vsomeip-client-example uE test cases.
    //uE testcases already runs in bin folder whereas uStreamer runs outside bin folder.
    //Add/Remove bin folder path depends on whether you run uStreamer or vsoemip-client example test cases.
    jsonDirectoryPath = jsonDirectoryPath + "/bin/src/ustreamer-config.json";
    std::ifstream jsonFile(jsonDirectoryPath);

    LogInfo("{} JSON file Path : {}", __FUNCTION__, jsonDirectoryPath);

    std::string const jsonString((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

      // Parse the JSON string using RapidJSON:
    rapidjson::Document documentObj;
    std::ignore = documentObj.Parse(jsonString.c_str());
    if (documentObj.HasParseError()) {
        LogErr("Failed to parse JSON file");
        return nullptr;
    }
    if (documentObj.HasMember(serviceType.c_str()) && (documentObj[serviceType.c_str()].IsArray())) {
        std::shared_ptr<std::vector<std::shared_ptr<UUri>>> uriList = std::make_shared<std::vector<std::shared_ptr<UUri>>>();
        rapidjson::Value const &attributes = documentObj[serviceType.c_str()];
        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr) {
            rapidjson::Value const &attribute = *itr;
            assert(attribute.IsObject()); // each attribute is an object
            uint16_t serviceID_t = 0x0U;
            uint16_t eventGroupID_t = 0x0U;
            constexpr int8_t NUM_BASE = 16;

            std::shared_ptr<UUri> uri = std::make_shared<UUri>();
            UEntity uentity;
            UAuthority uauthority;
            UResource uresource;

            for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
                std::string const strKey(itr2->name.GetString());
                if (0 == strKey.compare("service")) {
                    std::string const strValue(itr2->value.GetString());
                    serviceID_t = static_cast<uint16_t>(std::stoul(strValue, nullptr, NUM_BASE));
                    uentity.set_id(serviceID_t);
                }
                if(0 == strKey.compare("UEntityName")) {
                    std::string const strValue(itr2->value.GetString());
                    uentity.set_name(strValue);
                }
                if(0 == strKey.compare("UEntityVersionMajor")) {
                    std::string const strMinor(itr2->value.GetString());
                    uentity.set_version_major(static_cast<uint32_t>(std::stoul(strMinor, nullptr, NUM_BASE)));
                }
                if(0 == strKey.compare("UEntityVersionMinor")) {
                    std::string const strMinor(itr2->value.GetString());
                    uentity.set_version_minor(static_cast<uint32_t>(std::stoul(strMinor, nullptr, NUM_BASE)));
                }
                if (0 == strKey.compare("UAuthorityIP")) {
                    //std::string const strIPAddress(itr2->value.GetString());
                    //uauthority.set_ip(strIPAddress.c_str());
                }
                if (0 == strKey.compare("UResourceID")) {
                    std::string const strEventGroup(itr2->value.GetString());
                    eventGroupID_t = static_cast<uint16_t>(std::stoul(strEventGroup, nullptr, NUM_BASE));
                    uresource.set_id(eventGroupID_t);
                }
                if(0 == strKey.compare("UResourceName")) {
                    std::string const strValue(itr2->value.GetString());
                    uresource.set_name(strValue);
                }
                if(0 == strKey.compare("UResourceInstance")) {
                    std::string const strValue(itr2->value.GetString());
                    uresource.set_instance(strValue);
                }
            }
            uri->mutable_entity()->CopyFrom(uentity);
            uri->mutable_authority()->CopyFrom(uauthority);
            uri->mutable_resource()->CopyFrom(uresource);

            if (serviceID_t != 0U) {
                uriList->push_back(uri);
            }
        }
        return uriList;
    }
    return nullptr;
}


/**
   * @brief This function is used to route outbound messages
   *
   * @param[in] umsg
   *
   * @return true if outbound message is routed successfully and false otherwise
   */
bool SomeipRouter::routeOutboundMsg(const UMessage &umsg) {
    LogTrace("{}", __FUNCTION__);
    uint16_t ueId = 0U;
    std::shared_ptr<SomeipHandler> hptr = nullptr;

    UMessageType msgType = umsg.attributes().type();
    switch (msgType) {
        case UMessageType::UMESSAGE_TYPE_REQUEST: {
            const UUri &sinkUri = umsg.attributes().sink();
            const UUri &srcUri = umsg.attributes().source();
            bool isRequestTypeSubscription = (umsg.attributes().sink().entity().id() == UT_SUBSCRIPTION_REQUEST_SINK_URI_UEID);
            if(isRequestTypeSubscription) {
                // This is subscription request
                ueId = static_cast<uint16_t>(srcUri.entity().id());
            } else {
                // This is a normal RPC request
                ueId = static_cast<uint16_t>(sinkUri.entity().id());
            }

            hptr = getHandler(ueId);
            if (nullptr == hptr) {
                if(isRequestTypeSubscription) {
                    hptr = newHandler(HandlerType::Client, srcUri.entity(), srcUri.authority());
                } else {
                    hptr = newHandler(HandlerType::Client, sinkUri.entity(), sinkUri.authority());
                }
                LogDebug("{} New client handler created for UEID[0x{:x}]", __FUNCTION__, ueId);
            }
        }
        break;
        case UMessageType::UMESSAGE_TYPE_RESPONSE:
        /* Fall through expected */
        case UMessageType::UMESSAGE_TYPE_PUBLISH: {
            if (umsg.attributes().source().entity().has_id()) {
                ueId =  static_cast<uint16_t> (umsg.attributes().source().entity().id());
            }
            else {
                LogErr("{} Source URI entityId is not found for outbound message.", __FUNCTION__);
                return false;
            }

            hptr = getHandler(ueId);
            if (nullptr == hptr) {
                LogErr("{} No service handler found for UEID[0x{:x}]", __FUNCTION__, ueId);
                return false;
            }
        }
        break;
        default:
            LogErr("{} Invalid message type", __FUNCTION__);
            return false;
    }

    if (nullptr == hptr) {
        // Something is terribly wrong
        LogCrit("{} Handler not found and creation also failed!!!", __FUNCTION__);
        return false;
    }

    if (!hptr->isRunning()) {
        LogTrace("{} - borrowing new thread", __FUNCTION__);
        if(! hptr->startThread()) {
            LogErr("{} Failed to start thread", __FUNCTION__);
            return false;
        }
    } else {
        LogTrace("{} - handler already exist. No new thread created.", __FUNCTION__);
    }

    hptr->queueOutboundMsg(umsg);

    return true;
}

/**
 * @brief @see @ref SomeipRouter::routeInboundMsg
 *
 * @param[in] umsg @see @ref SomeipRouter::routeInboundMsg
 * @return @see @ref SomeipRouter::routeInboundMsg
 */
bool SomeipRouter::routeInboundMsg(uprotocol::utransport::UMessage &umsg) {
    LogTrace("{}", __FUNCTION__);
    std::ignore = listener_.onReceive(umsg);
    return true;
}

/**
  * @brief @see @ref SomeipRouter::routeInboundSubscription
  *
  * @param[in] strTopic @see @ref SomeipRouter::routeInboundSubscription
  * @param[in] isSubscribe @see @ref SomeipRouter::routeInboundSubscription
  *
  * @return @see @ref SomeipRouter::routeInboundSubscription
  */
bool SomeipRouter::routeInboundSubscription(std::string const &strTopic, bool const &isSubscribe) const {
    static_cast<void>(strTopic);
    static_cast<void>(isSubscribe);

    // return (uStreamerObj_.getMessageRouter().handleInboundSubscription(strTopic, isSubscribe));
    return true;
}

/**
 * @brief @see @ref SomeipRouter::getMessageTranslator()
 *
 * @return A reference to the message translator object
 */
MessageTranslator& SomeipRouter::getMessageTranslator() {
    return *msgTranslator_;
}

/**
 * @brief @see Retrieves the SomeipHandler associated with the given ID
 *
 * @param[in] UEId The unique identifier of the handler
 *
 * @return Handler Id A shared pointer to the SomeipHandler object, or nullptr if no handler is found.
 */
std::shared_ptr<SomeipHandler> SomeipRouter::getHandler(uint16_t UEId) {
    LogTrace("{}",__FUNCTION__);
    if (const auto search=handlers_.find(UEId); search != handlers_.end()) {
        LogTrace("{} handler found with UEID [0x{:x}]",__FUNCTION__, UEId);
        return search->second;
    } else {
        return nullptr;
    }
}

/**
 * @brief Creates a new SomeipHandler object of the specified type and associates it with the given ID.
 *
 * @param[in] type The type of the handler to create.
 * @param[in] UEId The unique identifier of the handler.
 *
 * @return A shared pointer to the newly created SomeipHandler object
 */
std::shared_ptr<SomeipHandler> SomeipRouter::newHandler(
    HandlerType type,
    const UEntity &uEntityInfo,
    const uprotocol::v1::UAuthority &uAuthorityInfo) {

    LogTrace("{} Creating new handler with UEID [0x{:x}]", __FUNCTION__, uEntityInfo.id());

    try {
        std::shared_ptr<SomeipHandler> newSIPHandler =
            std::make_shared<SomeipHandler>(*someipInterface_,
                                            *this,
                                            type,
                                            uEntityInfo,
                                            uAuthorityInfo);
        auto const iter = handlers_.insert({uEntityInfo.id(), std::move(newSIPHandler)}).first;

        return iter->second;

    } catch(const std::exception& e) {
        LogErr("{} Exception occurred while creating new handler with UEID [0x{:x}]", __FUNCTION__, uEntityInfo.id());
        return nullptr;
    }
}

/**
 * @brief Removes the SomeipHandler associated with the given ID.
 *
 * @note This method removes the SomeipHandler object that is associated with the provided "ID".
 *
 * @param[in] UEId The unique identifier of the handler to remove.
 */
void SomeipRouter::removeHandler(uint16_t const UEId) {
    //RCTODO
    // properly close handler, unsubscribe from all events, etc.
    std::ignore = handlers_.erase(UEId);
}

/**
 * @brief Handles a state change event.
 *
 * @note This method is called when a state change event occurs.
 *
 * @param[in] currentState The new state of the SomeipRouter.
 */
void SomeipRouter::onState(state_type_e const currentState) {
    LogInfo("{} SomeIP state changed to {}", __FUNCTION__, stateTypeAsStr(currentState));
    state_ = currentState;
}

/**
 * @brief @see @ref SomeipRouter::isStateRegistered()
 *
 * @return
 */
bool SomeipRouter::isStateRegistered() {
    return (state_ == state_type_e::ST_REGISTERED);
}

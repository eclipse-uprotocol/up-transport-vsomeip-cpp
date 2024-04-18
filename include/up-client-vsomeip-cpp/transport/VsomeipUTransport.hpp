#ifndef _VSOMEIP_UTRANSPORT_H_
#define _VSOMEIP_UTRANSPORT_H_

// #include <cstddef>
// #include <unordered_map>
// #include <atomic>
#include <up-cpp/transport/UTransport.h>
#include <up-client-vsomeip-cpp/routing/SomeipRouter.hpp>
#include <memory>

// using namespace uprotocol::v1;
// using namespace uprotocol::utransport;

// class ListenerContainer {
//     public:
//         std::vector<z_owned_subscriber_t> subVector_;
//         std::vector<z_owned_queryable_t> queryVector_;
//         std::vector<const UListener*> listenerVector_;
// };

class VsomeipUTransport : public uprotocol::utransport::UTransport {
public:

    /**
    * @brief The API provides an instance of the vsomeip session
    * @return instance of VsomeipUTransport
    */
    static VsomeipUTransport& instance(void) noexcept;

    /**
    * @brief Terminates the vsomeip utransport  - the API should be called by any class that called init
    * @return Returns OK on SUCCESS and ERROR on failure
    */
    uprotocol::v1::UStatus terminate() noexcept;

    /**
    * @brief Transmit UPayload to the topic using the attributes defined in UTransportAttributes.
    * @param message UMessage which contains attributes and payload information
    * @return Returns OKSTATUS if the payload has been successfully sent (ACK'ed), otherwise it
    * returns FAILSTATUS with the appropriate failure.
    */
    uprotocol::v1::UStatus send(const uprotocol::utransport::UMessage &message) noexcept;

    /**
    * Register listener to be called when UPayload is received for the specific topic.
    * @param topic Resolved UUri for where the message arrived via the underlying transport technology.
    * @param listener The method to execute to process the date for the topic.
    * @return Returns OKSTATUS if the listener is unregistered correctly, otherwise it returns FAILSTATUS
    * with the appropriate failure.
    */
    uprotocol::v1::UStatus registerListener(const uprotocol::v1::UUri &uri,
                                const uprotocol::utransport::UListener &listener) noexcept;

    /**
    * Unregister a listener for a given topic. Messages arriving on this topic will no longer be processed
    * by this listener.
    * @param topic Resolved UUri for where the listener was registered to receive messages from.
    * @param listener The method to execute to process the date for the topic.
    * @return Returns OKSTATUS if the listener is unregistered correctly, otherwise it returns FAILSTATUS
    * with the appropriate failure.
    */
    uprotocol::v1::UStatus unregisterListener(const uprotocol::v1::UUri &uri,
                                const uprotocol::utransport::UListener &listener) noexcept;

    uprotocol::v1::UStatus receive(const uprotocol::v1::UUri &uri,
                    const uprotocol::utransport::UPayload &payload,
                    const uprotocol::v1::UAttributes &attributes) noexcept;

private:
	VsomeipUTransport() {}
    VsomeipUTransport(const VsomeipUTransport&) = delete;
    VsomeipUTransport& operator=(const VsomeipUTransport&) = delete;
        /**
    * @brief init the VsomeipUTransport
    * @param listener UListener reference
    * @return Returns OK on SUCCESS and ERROR on failure
    */
    uprotocol::v1::UStatus init(const uprotocol::utransport::UListener &listener) noexcept;

    std::unique_ptr<SomeipRouter> router_;
};

#endif /*_VSOMEIP_UTRANSPORT_H_*/

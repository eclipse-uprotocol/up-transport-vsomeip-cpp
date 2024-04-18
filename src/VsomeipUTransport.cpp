#include <up-client-vsomeip-cpp/transport/VsomeipUTransport.hpp>
#include <up-client-vsomeip-cpp/utils/Logger.hpp>

using namespace uprotocol::v1;
using namespace uprotocol::utransport;

VsomeipUTransport& VsomeipUTransport::instance(void) noexcept {
    static VsomeipUTransport instance;
    return instance;
}

/**
  * @brief @return @see @ref VsomeipUTransport::init
  * @param listener @see @ref VsomeipUTransport::init
  */
UStatus VsomeipUTransport::init(const UListener &listener) noexcept {
    UStatus status;
    LogInfo("{}", __FUNCTION__);

    try{
        router_ = std::make_unique<SomeipRouter>(listener);
    } catch (const std::bad_alloc& e) {
        LogErr("Failed to allocate memory for SomeipRouter: {}", e.what());
        status.set_code(UCode::RESOURCE_EXHAUSTED);
        return status;
    }
        if (! router_->init()) {
        LogErr("Failed to initialize SomeipRouter");
        status.set_code(UCode::UNAVAILABLE);
        return status;
    }

    status.set_code(UCode::OK);
    return status;
}

UStatus VsomeipUTransport::terminate() noexcept {
    UStatus status;
    LogInfo("{}", __FUNCTION__);
    router_.reset();
    status.set_code(UCode::OK);
    return status;
}

/**
  * @brief @see @ref VsomeipUTransport::send
  * @param message @see @ref VsomeipUTransport::send
  * @return @see @ref VsomeipUTransport::send
  */
UStatus VsomeipUTransport::send(const UMessage &message) noexcept {
    UStatus status;

    if (! router_->routeOutboundMsg(message)) {
        status.set_code(UCode::UNAVAILABLE);
        return status;
    }

    status.set_code(UCode::OK);
    return status;
}

UStatus VsomeipUTransport::registerListener(const UUri &uri,
                                            const UListener &listener) noexcept {
    LogInfo("{}", __FUNCTION__);
    static_cast<void>(uri);
    UStatus status = init(listener);
    return status;
}

UStatus VsomeipUTransport::unregisterListener(const UUri &uri,
                           const UListener &listener) noexcept {
    static_cast<void>(uri);
    static_cast<void>(listener);
    UStatus status;
    status.set_code(UCode::UNIMPLEMENTED);
    return status;
}

UStatus VsomeipUTransport::receive(const UUri &uri,
                const UPayload &payload,
                const UAttributes &attributes) noexcept {
    static_cast<void>(uri);
    static_cast<void>(payload);
    static_cast<void>(attributes);
    UStatus status;
    status.set_code(UCode::UNIMPLEMENTED);
    return status;
}

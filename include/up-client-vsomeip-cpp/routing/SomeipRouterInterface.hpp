#ifndef SOMEIP_ROUTER_INTERFACE_HPP
#define SOMEIP_ROUTER_INTERFACE_HPP
#include <up-cpp/transport/UTransport.h>
#include <up-client-vsomeip-cpp/translation/MessageTranslator.hpp>
#include <memory>

class SomeipHandler;

/**
 * @brief Represents an interface for the SomeIP router.
 *
 * @note SomeipRouterInterface class is a template for plugins to follow.
 * These are the apis exposed to the Message Router
 * Plugins need to implement them.
 * Supports data flow from Message Router --> plugin
 */
class SomeipRouterInterface {
public:

     /**
     * @brief Route an inbound SomeIP message.
     *
     * @note This pure virtual function is responsible for routing an inbound SomeIP message.
     * It takes a shared pointer to a UMessage object representing the incoming message,
     * and a reference to a SomeipHandler object for handling the message.
     *
     * @param[in] uMessage UMessage object representing the inbound message.
     * @return True if the message was successfully routed, false otherwise.
     */
    virtual bool routeInboundMsg(uprotocol::utransport::UMessage &uMessage)  = 0;

    /**
     * @brief Get the message translator.
     *
     * @note This pure virtual function is responsible for returning a reference to the message translator object.
     * The message translator is used for translating messages between different formats or protocols.
     *
     * @return A reference to the message translator object.
     */
    virtual MessageTranslator& getMessageTranslator() = 0;

    /**
     * @brief Handles inbound subscription for a given topic.
     *
     *
     * @param[in] strTopic The topic for which the subscription request is made.
     * @param[in] isSubscribe A boolean indicating whether it is a subscribe or unsubscribe request.
     *
     * @return True if the subscription request is successfully handled, false otherwise.
     */
    virtual bool routeInboundSubscription(std::string const &strTopic, bool const &isSubscribe) const = 0;

    /**
     * @brief Check if the state is registered.
     *
     * @note This pure virtual function is responsible for checking if the state is registered.
     * It returns a boolean value indicating whether the state is registered or not.
     *
     * @return True if the state is registered, false otherwise.
     */
    virtual bool isStateRegistered() = 0;

};

#endif /* SOMEIP_ROUTER_INTERFACE_HPP */

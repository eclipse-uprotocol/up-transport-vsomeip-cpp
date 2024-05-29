#include "UURIHelper.hpp"
 
/**
 *  @brief Builds a UUri.
 */
std::shared_ptr<UUri> buildUURI() {
    /**
     *  @brief UUri parameters
     */
    uint16_t const uEntityId             = 0x1102; //Service ID
    std::string const uEntityName        = "0x1102";
    uint32_t const uEntityVersionMajor   = 0x1; //Major Version
    uint32_t const uEntityVersionMinor   = 0x0; //Minor Version
    std::string const uAuthorityIp       = "172.17.0.1";
    uint16_t const uResourceId           = 0x0102; //Method ID
    std::string const uResourceName      = "rpc";
    std::string const uResourceInstance  = "0x0102";
    std::shared_ptr<UUri> uriPtr = std::make_shared<UUri>();
    UEntity uEntity;
    UAuthority uAuthority;
    UResource uResource;
 
    uEntity.set_id(uEntityId);
    uEntity.set_name(uEntityName.c_str());
    uEntity.set_version_major(uEntityVersionMajor);
    uEntity.set_version_minor(uEntityVersionMinor);
 
    uriPtr->mutable_entity()->CopyFrom(uEntity);
 
    uResource.set_id(uResourceId);
    uResource.set_name(uResourceName.c_str());
    uResource.set_instance(uResourceInstance);
 
    uriPtr->mutable_resource()->CopyFrom(uResource);
    return uriPtr;
}
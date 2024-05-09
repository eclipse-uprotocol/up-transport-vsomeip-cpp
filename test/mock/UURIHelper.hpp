#ifndef UURIHELPER_HPP
#define UURIHELPER_HPP

#include <VsomeipUTransport.hpp>
#include <up-cpp/transport/builder/UAttributesBuilder.h>
#include <up-core-api/uri.pb.h>
#include <up-cpp/uuid/factory/Uuidv8Factory.h>
#include <up-cpp/uuid/serializer/UuidSerializer.h>
 
using namespace uprotocol::utransport;
using namespace uprotocol::uuid;
using namespace uprotocol::v1;
using namespace vsomeip_v3;

extern UEntity uEntity;
extern UAuthority uAuthority;
extern UResource uResource;
 
std::shared_ptr<UUri> buildUURI();
 
#endif // UURIHELPER_HPP
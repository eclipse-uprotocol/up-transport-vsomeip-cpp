#include <up-cpp/uuid/factory/Uuidv8Factory.h>
#include <up-cpp/uuid/serializer/UuidSerializer.h>
#include <up-cpp/uri/serializer/LongUriSerializer.h>
#include <up-cpp/transport/builder/UAttributesBuilder.h>
#include <up-cpp/transport/UTransport.h>
#include <up-client-vsomeip-cpp/transport/VsomeipUTransport.hpp>
#include <chrono>
#include <csignal>
#include <unistd.h>

using namespace uprotocol::utransport;
using namespace uprotocol::uuid;
using namespace uprotocol::v1;
using namespace uprotocol::uri;
#ifndef SOMEIP_LIB_INCLUDES_HPP
#define SOMEIP_LIB_INCLUDES_HPP


/* TODO: LOCATION OF THIS FILE IS IMPORTANT
 * currently placing it alongside the SomeipInterface.hpp file
 */

/**
 * @brief This file should include all the required headers from the someip library
 * currently being used. This file with the same name shall exist in all the someip
 * implementations. This file is included in the SomeipInterface.hpp file.
 * This is done so that someip libraries can be swapped in and out without having to
 * compile the entire project with the new library, given that the new library is
 * compatible with the interface defined in SomeipInterface.hpp and the libraries are
 * linked dynamically (.so).
 *
 */

/* This version of SomeipLibIncludes is for vsomeip library */
#include "vsomeip/vsomeip.hpp"
#include "vsomeip/handler.hpp"
#include <vsomeip/primitive_types.hpp>

using namespace vsomeip_v3;

/**
 * @brief typecasting stateType as state_type_e
 *
 */
using stateType = state_type_e;

/**
 * @brief typecasting secClientType as vsomeip_sec_client_t
 *
 */
using secClientType = vsomeip_sec_client_t;

/**
 * @brief This function converts the stateType from enum to string
 *
 * @param enVal enum val that is to be converted into string
 * @return enum value in string format
 *
 */
constexpr const char *stateTypeAsStr(stateType const enVal) noexcept {
    const char *ret = "InvalidInput";
    switch (enVal)
    {
    case stateType::ST_REGISTERED:
        ret = "ST_REGISTERED";
        break;
    case stateType::ST_DEREGISTERED:
        ret = "ST_DEREGISTERED";
        break;

    default:
        break;
    }
    return ret;
}

#endif //SOMEIP_LIB_INCLUDES_HPP
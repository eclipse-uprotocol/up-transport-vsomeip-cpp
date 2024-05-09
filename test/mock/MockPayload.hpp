#include <gmock/gmock.h>
#include <vsomeip/primitive_types.hpp>
#include <vsomeip/vsomeip.hpp>
#include <vector>

namespace vsomeip_v3 {

class MockPayload : public payload {
public:
    MOCK_METHOD(bool, isEqualTo, (const payload&), ());
    MOCK_METHOD(byte_t*, get_data, (), (override));
    MOCK_METHOD(const byte_t*, get_data, (), (const, override));
    MOCK_METHOD(void, set_data, (const byte_t*, length_t), (override));
    MOCK_METHOD(void, set_data, (const std::vector<byte_t>&), (override));
    MOCK_METHOD(length_t, get_length, (), (const, override));
    MOCK_METHOD(void, set_capacity, (length_t), (override));
    MOCK_METHOD(void, set_data, (std::vector<byte_t>&&), (override));

    bool operator==(const payload &_other) {
        return isEqualTo(_other);
    }
};

} // namespace vsomeip_v3
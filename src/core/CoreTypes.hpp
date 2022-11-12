#ifndef SRC_CORE_CORETYPES_HPP
#define SRC_CORE_CORETYPES_HPP

namespace core {

enum Result {
    kGood = 0,
    kFail = -1,        // Generic Fail
    kBusFail = -2,     // Data Bus Fail
    kLogicFail = -3,   // Logic Error Fail

};

} // core

#endif // SRC_CORE_CORETYPES_HPP
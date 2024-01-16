#include "QrackWrapper.h"

#include "qrack/wasm_api.hpp"

int QrackWrapper::get_error(int sid) {
    return Qrack::get_error((Qrack::quid)sid);
}

int QrackWrapper::init_general(int length) {
    return (int)Qrack::init_count((bitLenInt)length, false);
}

int QrackWrapper::init_stabilizer(int length) {
    return (int)Qrack::init_count_type((bitLenInt)length, false, false, true, true, false, false, false, false, false);
}

int QrackWrapper::init_qbdd(int length) {
    return (int)Qrack::init_count_type((bitLenInt)length, false, false, true, false, true, false, false, false, false);
}

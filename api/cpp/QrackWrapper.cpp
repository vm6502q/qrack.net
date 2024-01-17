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

int QrackWrapper::init_clone(int sid) {
    return (int)Qrack::init_clone((Qrack::quid)sid);
}

void QrackWrapper::destroy(int sid) {
    Qrack::destroy((Qrack::quid)sid);
}

void QrackWrapper::seed(int sid, int s) {
    Qrack::seed((Qrack::quid)sid, (unsigned)s);
}

double QrackWrapper::Prob(int sid, int q) {
    return (double)Qrack::Prob((Qrack::quid)sid, (bitLenInt)q);
}

double QrackWrapper::ProbRdm(int sid, int q) {
    return (double)Qrack::ProbRdm((Qrack::quid)sid, (bitLenInt)q);
}

void QrackWrapper::ResetAll(int sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}
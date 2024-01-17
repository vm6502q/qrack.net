#include "QrackWrapper.h"

#include "qrack/wasm_api.hpp"

typedef long long int64_t;

int64_t QrackWrapper::get_error(int64_t sid) {
    return Qrack::get_error((Qrack::quid)sid);
}

int64_t QrackWrapper::init_general(int64_t length) {
    return (int64_t)Qrack::init_count((bitLenInt)length, false);
}

int64_t QrackWrapper::init_stabilizer(int64_t length) {
    return (int64_t)Qrack::init_count_type((bitLenInt)length, false, false, true, true, false, false, false, false, false);
}

int64_t QrackWrapper::init_qbdd(int64_t length) {
    return (int64_t)Qrack::init_count_type((bitLenInt)length, false, false, true, false, true, false, false, false, false);
}

int64_t QrackWrapper::init_clone(int64_t sid) {
    return (int64_t)Qrack::init_clone((Qrack::quid)sid);
}

void QrackWrapper::destroy(int64_t sid) {
    Qrack::destroy((Qrack::quid)sid);
}

void QrackWrapper::seed(int64_t sid, int64_t s) {
    Qrack::seed((Qrack::quid)sid, (unsigned)s);
}

double QrackWrapper::Prob(int64_t sid, int64_t q) {
    return (double)Qrack::Prob((Qrack::quid)sid, (bitLenInt)q);
}

double QrackWrapper::ProbRdm(int64_t sid, int64_t q) {
    return (double)Qrack::ProbRdm((Qrack::quid)sid, (bitLenInt)q);
}

double QrackWrapper::PermutationProb(long long sid, std::vector<long long> q, std::vector<char> s) {
    if (q.size() != s.size()) {
        throw std::invalid_argument("QrackWrapper::PermutationProb() 'q' and 's' parameter vectors should have same size!");
    }
    std::vector<Qrack::QubitIndexState> _q;
    _q.reserve(q.size());
    for (long long i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIndexState((int64_t)q[i], (bool)s[i]));
    }
    return (double)Qrack::PermutationProb((Qrack::quid)sid, _q);
}

void QrackWrapper::ResetAll(int64_t sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}

void QrackWrapper::allocateQubit(int64_t sid, int64_t qid) {
    Qrack::allocateQubit((Qrack::quid)sid, (Qrack::quid)qid);
}
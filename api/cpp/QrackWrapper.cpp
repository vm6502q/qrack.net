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

std::vector<Qrack::QubitIndexState> validatePermProb(const std::vector<long long>& q, const std::vector<char>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIndexState> _q;
    _q.reserve(q.size());
    for (long long i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIndexState((int64_t)q[i], (bool)s[i]));
    }
    return _q;
}

double QrackWrapper::PermutationProb(long long sid, std::vector<long long> q, std::vector<char> s) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProb() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProb((Qrack::quid)sid, _q);
}

double QrackWrapper::PermutationProbRdm(long long sid, std::vector<long long> q, std::vector<char> s, bool r) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProbRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProbRdm((Qrack::quid)sid, _q, r);
}

std::vector<Qrack::QubitIntegerExpectation> validateFactProb(const std::vector<long long>& q, const std::vector<long long>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIntegerExpectation> _q;
    _q.reserve(q.size());
    for (long long i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIntegerExpectation((int64_t)q[i], (int64_t)s[i]));
    }
    return _q;
}

double QrackWrapper::FactorizedExpectation(long long sid, std::vector<long long> q, std::vector<long long> s) {
    std::vector<Qrack::QubitIntegerExpectation> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectation() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectation((Qrack::quid)sid, _q);
}

double QrackWrapper::FactorizedExpectationRdm(long long sid, std::vector<long long> q, std::vector<long long> s, bool r) {
    std::vector<Qrack::QubitIntegerExpectation> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectationRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationRdm((Qrack::quid)sid, _q, r);
}

std::vector<Qrack::QubitRealExpectation> validateFactFpProb(const std::vector<long long>& q, const std::vector<double>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitRealExpectation> _q;
    _q.reserve(q.size());
    for (long long i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitRealExpectation((int64_t)q[i], (Qrack::real1)s[i]));
    }
    return _q;
}

double QrackWrapper::FactorizedExpectationFp(long long sid, std::vector<long long> q, std::vector<double> s) {
    std::vector<Qrack::QubitRealExpectation> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFp() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFp((Qrack::quid)sid, _q);
}

double QrackWrapper::FactorizedExpectationFpRdm(long long sid, std::vector<long long> q, std::vector<double> s, bool r) {
    std::vector<Qrack::QubitRealExpectation> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFpRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFpRdm((Qrack::quid)sid, _q, r);
}

void QrackWrapper::ResetAll(int64_t sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}

void QrackWrapper::allocateQubit(int64_t sid, int64_t qid) {
    Qrack::allocateQubit((Qrack::quid)sid, (Qrack::quid)qid);
}
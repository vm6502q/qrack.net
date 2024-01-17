#include "QrackWrapper.h"

#include "qrack/wasm_api.hpp"

typedef long long int64_t;

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
long long QrackWrapper::num_qubits(long long sid) {
    return (long long)Qrack::num_qubits((Qrack::quid)sid);
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

void QrackWrapper::PhaseParity(long long sid, double lambda, std::vector<long long> q) {
    std::vector<bitLenInt>_q;
    _q.reserve(q.size());
    for (size_t i = 0U; i < q.size(); ++i) {
        _q.push_back(q[i]);
    }
    Qrack::PhaseParity((Qrack::quid)sid, (Qrack::real1_f)lambda, _q);
}
double QrackWrapper::JointEnsembleProbability(long long sid, std::vector<long long> q, std::vector<char> b) {
    if (q.size() != b.size()) {
        throw std::invalid_argument("QrackWrapper::JointEnsembleProbability() 'q' and 'b' parameter vectors should have same size!");
    }
    std::vector<Qrack::QubitPauliBasis>_q;
    _q.reserve(q.size());
    for (size_t i = 0U; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitPauliBasis((bitLenInt)q[i], (Qrack::Pauli)b[i]));
    }
    return (double)Qrack::JointEnsembleProbability((Qrack::quid)sid, _q);
}

void QrackWrapper::ResetAll(int64_t sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}
void QrackWrapper::allocateQubit(int64_t sid, int64_t qid) {
    Qrack::allocateQubit((Qrack::quid)sid, (Qrack::quid)qid);
}

void QrackWrapper::X(long long sid, long long q) {
    Qrack::X((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Y(long long sid, long long q) {
    Qrack::Y((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Z(long long sid, long long q) {
    Qrack::Z((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::H(long long sid, long long q) {
    Qrack::H((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::S(long long sid, long long q) {
    Qrack::S((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::T(long long sid, long long q) {
    Qrack::T((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjS(long long sid, long long q) {
    Qrack::AdjS((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjT(long long sid, long long q) {
    Qrack::AdjT((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::U(long long sid, long long q, double theta, double phi, double lambda) {
    Qrack::U((Qrack::quid)sid, (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
std::vector<Qrack::complex> transform_matrix(std::vector<double> m) {
    if (m.size() != 8) {
        throw std::invalid_argument("QrackWrapper::Mtrx m argument must be 8 real components for 4 complex numbers of a 2x2 matrix!");
    }
    std::vector<Qrack::complex> _m;
    _m.reserve(4);
    for (size_t i = 0U; i < 4; ++i) {
        _m.push_back(Qrack::complex(m[2 * i], m[(2 * i) + 1]));
    }

    return _m;
}
void QrackWrapper::Mtrx(long long sid, std::vector<double> m, long long q) {
    Qrack::Mtrx((Qrack::quid)sid, transform_matrix(m), (bitLenInt)q);
}
std::vector<bitLenInt> transform_controls(const std::vector<long long>& c) {
    std::vector<bitLenInt> _c;
    _c.reserve(c.size());
    for (size_t i = 0U; i < c.size(); ++i) {
        _c.push_back((bitLenInt)c[i]);
    }

    return _c;
}
void QrackWrapper::MCX(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCX((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCY(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCY((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCZ(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCZ((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCH(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCH((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCS(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCS((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCT(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCT((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjS(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCAdjS((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjT(long long sid, std::vector<long long> c, long long q) {
    Qrack::MCAdjT((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MCU(long long sid, std::vector<long long> c, long long q, double theta, double phi, double lambda) {
    Qrack::MCU((Qrack::quid)sid, transform_controls(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MCMtrx(long long sid, std::vector<long long> c, std::vector<double> m, long long q) {
    Qrack::MCMtrx((Qrack::quid)sid, transform_controls(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MACX(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACX((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACY(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACY((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACZ(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACZ((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACH(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACH((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACS(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACS((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACT(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACT((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjS(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACAdjS((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjT(long long sid, std::vector<long long> c, long long q) {
    Qrack::MACAdjT((Qrack::quid)sid, transform_controls(c), (bitLenInt)q);
}
void QrackWrapper::MACU(long long sid, std::vector<long long> c, long long q, double theta, double phi, double lambda) {
    Qrack::MACU((Qrack::quid)sid, transform_controls(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MACMtrx(long long sid, std::vector<long long> c, std::vector<double> m, long long q) {
    Qrack::MACMtrx((Qrack::quid)sid, transform_controls(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::UCMtrx(long long sid, std::vector<long long> c, std::vector<double> m, long long q, long long p) {
    Qrack::UCMtrx((Qrack::quid)sid, transform_controls(c), transform_matrix(m), (bitLenInt)q, (bitCapInt)p);
}
void QrackWrapper::Multiplex1Mtrx(long long sid, std::vector<long long> c, long long q, std::vector<double> m) {
    Qrack::Multiplex1Mtrx((Qrack::quid)sid, transform_controls(c), (bitLenInt)q, transform_matrix(m));
}

void QrackWrapper::MX(long long sid, std::vector<long long> q) {
    Qrack::MX((Qrack::quid)sid, transform_controls(q));
}
void QrackWrapper::MY(long long sid, std::vector<long long> q) {
    Qrack::MY((Qrack::quid)sid, transform_controls(q));
}
void QrackWrapper::MZ(long long sid, std::vector<long long> q) {
    Qrack::MZ((Qrack::quid)sid, transform_controls(q));
}

void QrackWrapper::R(long long sid, double phi, long long q, char b) {
    Qrack::R((Qrack::quid)sid, (Qrack::real1_f)phi, Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}
void QrackWrapper::MCR(long long sid, double phi, std::vector<long long> c, long long q, char b) {
    Qrack::MCR((Qrack::quid)sid, (Qrack::real1_f)phi, transform_controls(c), Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}

std::vector<Qrack::QubitPauliBasis> transform_qubit_paulis(const std::vector<long long>& q, const std::vector<char>& b, std::string m) {
    if (q.size() != b.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitPauliBasis> _b;
    _b.reserve(b.size());
    for (size_t i = 0U; i < b.size(); ++i) {
        _b.push_back(Qrack::QubitPauliBasis((bitLenInt)q[i], (Qrack::Pauli)b[i]));
    }

    return _b;
}
void QrackWrapper::Exp(long long sid, double phi, std::vector<long long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Exp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::Exp((Qrack::quid)sid, (Qrack::real1_f)phi, _q);
}
void QrackWrapper::MCExp(long long sid, double phi, std::vector<long long> c, std::vector<long long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::MCExp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::MCExp((Qrack::quid)sid, (Qrack::real1_f)phi, transform_controls(c), _q);
}
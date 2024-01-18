#include "QrackWrapper.h"

#include "qrack/wasm_api.hpp"

typedef long long int64_t;
typedef unsigned long long uint64_t;

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
int64_t QrackWrapper::num_qubits(int64_t sid) {
    return (int64_t)Qrack::num_qubits((Qrack::quid)sid);
}
void QrackWrapper::destroy(int64_t sid) {
    Qrack::destroy((Qrack::quid)sid);
}
void QrackWrapper::seed(int64_t sid, int64_t s) {
    Qrack::seed((Qrack::quid)sid, (unsigned)s);
}
void QrackWrapper::allocateQubit(int64_t sid, int64_t qid) {
    Qrack::allocateQubit((Qrack::quid)sid, (Qrack::quid)qid);
}
bool QrackWrapper::release(int64_t sid, int64_t qid) {
    return Qrack::release((Qrack::quid)sid, (Qrack::quid)qid);
}
bool QrackWrapper::TrySeparate1Qb(int64_t sid, int64_t qi1) {
    return Qrack::TrySeparate1Qb((Qrack::quid)sid, (bitLenInt)qi1);
}
bool QrackWrapper::TrySeparate2Qb(int64_t sid, int64_t qi1, int64_t qi2) {
    return Qrack::TrySeparate2Qb((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
std::vector<bitLenInt> transform_qbids(const std::vector<int64_t>& c) {
    std::vector<bitLenInt> _c;
    _c.reserve(c.size());
    for (size_t i = 0U; i < c.size(); ++i) {
        _c.push_back((bitLenInt)c[i]);
    }

    return _c;
}
bool QrackWrapper::TrySeparateTol(int64_t sid, std::vector<int64_t> q, double tol) {
    return Qrack::TrySeparateTol((Qrack::quid)sid, transform_qbids(q), (Qrack::real1_f)tol);
}
double QrackWrapper::GetUnitaryFidelity(int64_t sid) {
    return (double)Qrack::GetUnitaryFidelity((Qrack::quid)sid);
}
void QrackWrapper::ResetUnitaryFidelity(int64_t sid) {
    Qrack::ResetUnitaryFidelity((Qrack::quid)sid);
}
void QrackWrapper::SetSdrp(int64_t sid, double sdrp) {
    Qrack::SetSdrp((Qrack::quid)sid, (Qrack::real1_f)sdrp);
}
void QrackWrapper::SetReactiveSeparate(int64_t sid, bool irs) {
    Qrack::SetReactiveSeparate((Qrack::quid)sid, irs);
}
void QrackWrapper::SetTInjection(int64_t sid, bool iti) {
    Qrack::SetTInjection((Qrack::quid)sid, iti);
}

double QrackWrapper::Prob(int64_t sid, int64_t q) {
    return (double)Qrack::Prob((Qrack::quid)sid, (bitLenInt)q);
}
double QrackWrapper::ProbRdm(int64_t sid, int64_t q) {
    return (double)Qrack::ProbRdm((Qrack::quid)sid, (bitLenInt)q);
}
std::vector<Qrack::QubitIndexState> validatePermProb(const std::vector<int64_t>& q, const std::vector<char>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIndexState> _q;
    _q.reserve(q.size());
    for (int64_t i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIndexState((int64_t)q[i], (bool)s[i]));
    }
    return _q;
}
double QrackWrapper::PermutationProb(int64_t sid, std::vector<int64_t> q, std::vector<char> s) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProb() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProb((Qrack::quid)sid, _q);
}
double QrackWrapper::PermutationProbRdm(int64_t sid, std::vector<int64_t> q, std::vector<char> s, bool r) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProbRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProbRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitIntegerExpectation> validateFactProb(const std::vector<int64_t>& q, const std::vector<int64_t>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIntegerExpectation> _q;
    _q.reserve(q.size());
    for (int64_t i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIntegerExpectation((int64_t)q[i], (int64_t)s[i]));
    }
    return _q;
}
double QrackWrapper::FactorizedExpectation(int64_t sid, std::vector<int64_t> q, std::vector<int64_t> s) {
    std::vector<Qrack::QubitIntegerExpectation> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectation() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectation((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedExpectationRdm(int64_t sid, std::vector<int64_t> q, std::vector<int64_t> s, bool r) {
    std::vector<Qrack::QubitIntegerExpectation> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectationRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitRealExpectation> validateFactFpProb(const std::vector<int64_t>& q, const std::vector<double>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitRealExpectation> _q;
    _q.reserve(q.size());
    for (int64_t i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitRealExpectation((int64_t)q[i], (Qrack::real1)s[i]));
    }
    return _q;
}
double QrackWrapper::FactorizedExpectationFp(int64_t sid, std::vector<int64_t> q, std::vector<double> s) {
    std::vector<Qrack::QubitRealExpectation> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFp() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFp((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedExpectationFpRdm(int64_t sid, std::vector<int64_t> q, std::vector<double> s, bool r) {
    std::vector<Qrack::QubitRealExpectation> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFpRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFpRdm((Qrack::quid)sid, _q, r);
}

void QrackWrapper::PhaseParity(int64_t sid, double lambda, std::vector<int64_t> q) {
    std::vector<bitLenInt>_q;
    _q.reserve(q.size());
    for (size_t i = 0U; i < q.size(); ++i) {
        _q.push_back(q[i]);
    }
    Qrack::PhaseParity((Qrack::quid)sid, (Qrack::real1_f)lambda, _q);
}
double QrackWrapper::JointEnsembleProbability(int64_t sid, std::vector<int64_t> q, std::vector<char> b) {
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

void QrackWrapper::Compose(int64_t sid1, int64_t sid2, std::vector<int64_t> q) {
    Qrack::Compose((Qrack::quid)sid1, (Qrack::quid)sid2, transform_qbids(q));
}
int64_t QrackWrapper::Decompose(int64_t sid, std::vector<int64_t> q) {
    return (int64_t)Qrack::Decompose((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::Dispose(int64_t sid, std::vector<int64_t> q) {
    Qrack::Dispose((Qrack::quid)sid, transform_qbids(q));
}

bool QrackWrapper::M(int64_t sid, int64_t q) {
    return Qrack::M((Qrack::quid)sid, (bitLenInt)q);
}
bool QrackWrapper::ForceM(int64_t sid, int64_t q, bool r) {
    return Qrack::ForceM((Qrack::quid)sid, (bitLenInt)q, r);
}
std::vector<Qrack::QubitPauliBasis> transform_qubit_paulis(const std::vector<int64_t>& q, const std::vector<char>& b, std::string m) {
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
bool QrackWrapper::Measure(int64_t sid, std::vector<int64_t> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Measure() 'q' and 'b' parameter vectors should have same size!");
    return Qrack::Measure((Qrack::quid)sid, _q);
}
uint64_t QrackWrapper::MAll(int64_t sid) {
    return (uint64_t)Qrack::MAll((Qrack::quid)sid);
}
std::vector<unsigned long long> QrackWrapper::MeasureShots(int64_t sid, std::vector<int64_t> q, unsigned s) {
    return Qrack::MeasureShots((Qrack::quid)sid, transform_qbids(q), s);
}
void QrackWrapper::ResetAll(int64_t sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}

void QrackWrapper::X(int64_t sid, int64_t q) {
    Qrack::X((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Y(int64_t sid, int64_t q) {
    Qrack::Y((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Z(int64_t sid, int64_t q) {
    Qrack::Z((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::H(int64_t sid, int64_t q) {
    Qrack::H((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::S(int64_t sid, int64_t q) {
    Qrack::S((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::T(int64_t sid, int64_t q) {
    Qrack::T((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjS(int64_t sid, int64_t q) {
    Qrack::AdjS((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjT(int64_t sid, int64_t q) {
    Qrack::AdjT((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::U(int64_t sid, int64_t q, double theta, double phi, double lambda) {
    Qrack::U((Qrack::quid)sid, (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
std::vector<Qrack::complex> transform_matrix(const std::vector<double>& m) {
    if (m.size() != 8) {
        throw std::invalid_argument("QrackWrapper::Mtrx m argument must be 8 real components for 4 complex numbers of a 2x2 matrix!");
    }
    std::vector<Qrack::complex> _m;
    _m.reserve(4);
    for (size_t i = 0U; i < 4; i+=2) {
        _m.push_back(Qrack::complex(m[2 * i], m[(2 * i) + 1]));
    }

    return _m;
}
void QrackWrapper::Mtrx(int64_t sid, std::vector<double> m, int64_t q) {
    Qrack::Mtrx((Qrack::quid)sid, transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MCX(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCX((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCY(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCY((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCZ(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCZ((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCH(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCH((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCS(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCT(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjS(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCAdjS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjT(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MCAdjT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCU(int64_t sid, std::vector<int64_t> c, int64_t q, double theta, double phi, double lambda) {
    Qrack::MCU((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MCMtrx(int64_t sid, std::vector<int64_t> c, std::vector<double> m, int64_t q) {
    Qrack::MCMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MACX(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACX((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACY(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACY((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACZ(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACZ((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACH(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACH((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACS(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACT(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjS(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACAdjS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjT(int64_t sid, std::vector<int64_t> c, int64_t q) {
    Qrack::MACAdjT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACU(int64_t sid, std::vector<int64_t> c, int64_t q, double theta, double phi, double lambda) {
    Qrack::MACU((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MACMtrx(int64_t sid, std::vector<int64_t> c, std::vector<double> m, int64_t q) {
    Qrack::MACMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::UCMtrx(int64_t sid, std::vector<int64_t> c, std::vector<double> m, int64_t q, int64_t p) {
    Qrack::UCMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q, (bitCapInt)p);
}
void QrackWrapper::Multiplex1Mtrx(int64_t sid, std::vector<int64_t> c, int64_t q, std::vector<double> m) {
    Qrack::Multiplex1Mtrx((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, transform_matrix(m));
}

void QrackWrapper::MX(int64_t sid, std::vector<int64_t> q) {
    Qrack::MX((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::MY(int64_t sid, std::vector<int64_t> q) {
    Qrack::MY((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::MZ(int64_t sid, std::vector<int64_t> q) {
    Qrack::MZ((Qrack::quid)sid, transform_qbids(q));
}

void QrackWrapper::R(int64_t sid, double phi, int64_t q, char b) {
    Qrack::R((Qrack::quid)sid, (Qrack::real1_f)phi, Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}
void QrackWrapper::MCR(int64_t sid, double phi, std::vector<int64_t> c, int64_t q, char b) {
    Qrack::MCR((Qrack::quid)sid, (Qrack::real1_f)phi, transform_qbids(c), Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}

void QrackWrapper::Exp(int64_t sid, double phi, std::vector<int64_t> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Exp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::Exp((Qrack::quid)sid, (Qrack::real1_f)phi, _q);
}
void QrackWrapper::MCExp(int64_t sid, double phi, std::vector<int64_t> c, std::vector<int64_t> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::MCExp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::MCExp((Qrack::quid)sid, (Qrack::real1_f)phi, transform_qbids(c), _q);
}

void QrackWrapper::SWAP(int64_t sid, int64_t qi1, int64_t qi2) {
    Qrack::SWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::ISWAP(int64_t sid, int64_t qi1, int64_t qi2) {
    Qrack::ISWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::AdjISWAP(int64_t sid, int64_t qi1, int64_t qi2) {
    Qrack::AdjISWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::FSim(int64_t sid, double theta, double phi, int64_t qi1, int64_t qi2) {
    Qrack::FSim((Qrack::quid)sid, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::CSWAP(int64_t sid, std::vector<int64_t> c, int64_t qi1, int64_t qi2) {
    Qrack::CSWAP((Qrack::quid)sid, transform_qbids(c), (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::ACSWAP(int64_t sid, std::vector<int64_t> c, int64_t qi1, int64_t qi2) {
    Qrack::ACSWAP((Qrack::quid)sid, transform_qbids(c), (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::AND(int64_t sid, int64_t qi1, int64_t qi2, int64_t qo) {
    Qrack::AND((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::OR(int64_t sid, int64_t qi1, int64_t qi2, int64_t qo) {
    Qrack::OR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::XOR(int64_t sid, int64_t qi1, int64_t qi2, int64_t qo) {
    Qrack::XOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::NAND(int64_t sid, int64_t qi1, int64_t qi2, int64_t qo) {
    Qrack::NAND((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::NOR(int64_t sid, int64_t qi1, int64_t qi2, int64_t qo) {
    Qrack::NOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::XNOR(int64_t sid, int64_t qi1, int64_t qi2, int64_t qo) {
    Qrack::XNOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::CLAND(int64_t sid, bool ci, int64_t qi, int64_t qo) {
    Qrack::CLAND((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLOR(int64_t sid, bool ci, int64_t qi, int64_t qo) {
    Qrack::CLOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLXOR(int64_t sid, bool ci, int64_t qi, int64_t qo) {
    Qrack::CLXOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLNAND(int64_t sid, bool ci, int64_t qi, int64_t qo) {
    Qrack::CLNAND((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLNOR(int64_t sid, bool ci, int64_t qi, int64_t qo) {
    Qrack::CLNOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLXNOR(int64_t sid, bool ci, int64_t qi, int64_t qo) {
    Qrack::CLXNOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}

void QrackWrapper::QFT(int64_t sid, std::vector<int64_t> q) {
    Qrack::QFT((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::IQFT(int64_t sid, std::vector<int64_t> q) {
    Qrack::IQFT((Qrack::quid)sid, transform_qbids(q));
}

void QrackWrapper::ADD(int64_t sid, unsigned long long a, std::vector<int64_t> q) {
    Qrack::ADD((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q));
}
void QrackWrapper::SUB(int64_t sid, unsigned long long a, std::vector<int64_t> q) {
    Qrack::SUB((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q));
}
void QrackWrapper::ADDS(int64_t sid, unsigned long long a, int64_t s, std::vector<int64_t> q) {
    Qrack::ADDS((Qrack::quid)sid, (bitCapInt)a, (bitLenInt)s, transform_qbids(q));
}
void QrackWrapper::SUBS(int64_t sid, unsigned long long a, int64_t s, std::vector<int64_t> q) {
    Qrack::SUBS((Qrack::quid)sid, (bitCapInt)a, (bitLenInt)s, transform_qbids(q));
}
void QrackWrapper::MCADD(int64_t sid, unsigned long long a, std::vector<int64_t> c, std::vector<int64_t> q) {
    Qrack::MCADD((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q));
}
void QrackWrapper::MCSUB(int64_t sid, unsigned long long a, std::vector<int64_t> c, std::vector<int64_t> q) {
    Qrack::MCSUB((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q));
}
void QrackWrapper::MUL(int64_t sid, unsigned long long a, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MUL((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::DIV(int64_t sid, unsigned long long a, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::DIV((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MULN(int64_t sid, unsigned long long a, unsigned long long m, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MULN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::DIVN(int64_t sid, unsigned long long a, unsigned long long m, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::DIVN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::POWN(int64_t sid, unsigned long long a, unsigned long long m, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::POWN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCMUL(int64_t sid, unsigned long long a, std::vector<int64_t> c, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MCMUL((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCDIV(int64_t sid, unsigned long long a, std::vector<int64_t> c, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MCDIV((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCMULN(int64_t sid, unsigned long long a, std::vector<int64_t> c, unsigned long long m, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MCMULN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCDIVN(int64_t sid, unsigned long long a, std::vector<int64_t> c, unsigned long long m, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MCDIVN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCPOWN(int64_t sid, unsigned long long a, std::vector<int64_t> c, unsigned long long m, std::vector<int64_t> q, std::vector<int64_t> o) {
    Qrack::MCPOWN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}

int64_t QrackWrapper::init_qneuron(int64_t sid, std::vector<int64_t> c, int64_t q, char f, double a, double tol) {
    return Qrack::init_qneuron((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::QNeuronActivationFn)f, (Qrack::real1_f)a, (Qrack::real1_f)tol);
}
int64_t QrackWrapper::clone_qneuron(int64_t nid) {
    return Qrack::clone_qneuron((Qrack::quid)nid);
}
void QrackWrapper::destroy_qneuron(int64_t nid) {
    return Qrack::destroy_qneuron((Qrack::quid)nid);
}
std::vector<Qrack::real1> transform_to_real1(const std::vector<double>& m) {
    std::vector<Qrack::real1> _m;
    _m.reserve(m.size());
    for (size_t i = 0U; i < m.size(); ++i) {
        _m.push_back(m[i]);
    }

    return _m;
}
void QrackWrapper::set_qneuron_angles(int64_t nid, std::vector<double> angles) {
    Qrack::set_qneuron_angles((Qrack::quid)nid, transform_to_real1(angles));
}
std::vector<double> transform_to_double(const std::vector<Qrack::real1>& m) {
    std::vector<double> _m;
    _m.reserve(m.size());
    for (size_t i = 0U; i < m.size(); ++i) {
        _m.push_back(m[i]);
    }

    return _m;
}
std::vector<double> QrackWrapper::get_qneuron_angles(int64_t nid) {
    return transform_to_double(Qrack::get_qneuron_angles((Qrack::quid)nid));
}
void QrackWrapper::set_qneuron_alpha(int64_t nid, double alpha) {
    Qrack::set_qneuron_alpha((Qrack::quid)nid, (Qrack::real1_f)alpha);
}
double QrackWrapper::get_qneuron_alpha(int64_t nid) {
    return (double)Qrack::get_qneuron_alpha((Qrack::quid)nid);
}
void QrackWrapper::set_qneuron_activation_fn(int64_t nid, char f) {
    Qrack::set_qneuron_activation_fn((Qrack::quid)nid, (Qrack::QNeuronActivationFn)f);
}
char QrackWrapper::get_qneuron_activation_fn(int64_t nid) {
    return (char)Qrack::get_qneuron_activation_fn((Qrack::quid)nid);
}
double QrackWrapper::qneuron_predict(int64_t nid, bool e, bool r) {
    return (double)Qrack::qneuron_predict((Qrack::quid)nid, e, r);
}
double QrackWrapper::qneuron_unpredict(int64_t nid, bool e) {
    return (double)Qrack::qneuron_unpredict((Qrack::quid)nid, e);
}
double QrackWrapper::qneuron_learn_cycle(int64_t nid, bool e) {
    return (double)Qrack::qneuron_learn_cycle((Qrack::quid)nid, e);
}
void QrackWrapper::qneuron_learn(int64_t nid, double eta, bool e, bool r) {
    Qrack::qneuron_learn((Qrack::quid)nid, (Qrack::real1_f)eta, e, r);
}
void QrackWrapper::qneuron_learn_permutation(long long nid, double eta, bool e, bool r) {
    Qrack::qneuron_learn_permutation((Qrack::quid)nid, (Qrack::real1_f)eta, e, r);
}
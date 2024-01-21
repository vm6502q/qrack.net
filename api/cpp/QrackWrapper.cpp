#include "QrackWrapper.h"

#include "qrack/wasm_api.hpp"

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
int QrackWrapper::num_qubits(int sid) {
    return (int)Qrack::num_qubits((Qrack::quid)sid);
}
void QrackWrapper::destroy(int sid) {
    Qrack::destroy((Qrack::quid)sid);
}
void QrackWrapper::seed(int sid, int s) {
    Qrack::seed((Qrack::quid)sid, (unsigned)s);
}
void QrackWrapper::allocateQubit(int sid, int qid) {
    Qrack::allocateQubit((Qrack::quid)sid, (Qrack::quid)qid);
}
bool QrackWrapper::release(int sid, int qid) {
    return Qrack::release((Qrack::quid)sid, (Qrack::quid)qid);
}
bool QrackWrapper::TrySeparate1Qb(int sid, int qi1) {
    return Qrack::TrySeparate1Qb((Qrack::quid)sid, (bitLenInt)qi1);
}
bool QrackWrapper::TrySeparate2Qb(int sid, int qi1, int qi2) {
    return Qrack::TrySeparate2Qb((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
std::vector<bitLenInt> transform_qbids(const std::vector<int>& c) {
    std::vector<bitLenInt> _c;
    _c.reserve(c.size());
    for (size_t i = 0U; i < c.size(); ++i) {
        _c.push_back((bitLenInt)c[i]);
    }

    return _c;
}
bool QrackWrapper::TrySeparateTol(int sid, std::vector<int> q, double tol) {
    return Qrack::TrySeparateTol((Qrack::quid)sid, transform_qbids(q), (Qrack::real1_f)tol);
}
double QrackWrapper::GetUnitaryFidelity(int sid) {
    return (double)Qrack::GetUnitaryFidelity((Qrack::quid)sid);
}
void QrackWrapper::ResetUnitaryFidelity(int sid) {
    Qrack::ResetUnitaryFidelity((Qrack::quid)sid);
}
void QrackWrapper::SetSdrp(int sid, double sdrp) {
    Qrack::SetSdrp((Qrack::quid)sid, (Qrack::real1_f)sdrp);
}
void QrackWrapper::SetReactiveSeparate(int sid, bool irs) {
    Qrack::SetReactiveSeparate((Qrack::quid)sid, irs);
}
void QrackWrapper::SetTInjection(int sid, bool iti) {
    Qrack::SetTInjection((Qrack::quid)sid, iti);
}

double QrackWrapper::Prob(int sid, int q) {
    return (double)Qrack::Prob((Qrack::quid)sid, (bitLenInt)q);
}
double QrackWrapper::ProbRdm(int sid, int q) {
    return (double)Qrack::ProbRdm((Qrack::quid)sid, (bitLenInt)q);
}
std::vector<Qrack::QubitIndexState> validatePermProb(const std::vector<int>& q, const std::vector<char>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIndexState> _q;
    _q.reserve(q.size());
    for (int i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIndexState((int)q[i], (bool)s[i]));
    }
    return _q;
}
double QrackWrapper::PermutationProb(int sid, std::vector<int> q, std::vector<char> s) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProb() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProb((Qrack::quid)sid, _q);
}
double QrackWrapper::PermutationProbRdm(int sid, std::vector<int> q, std::vector<char> s, bool r) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProbRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProbRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitIntegerExpectation> validateFactProb(const std::vector<int>& q, const std::vector<int>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIntegerExpectation> _q;
    _q.reserve(q.size());
    for (int i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitIntegerExpectation((int)q[i], (int)s[i]));
    }
    return _q;
}
double QrackWrapper::FactorizedExpectation(int sid, std::vector<int> q, std::vector<int> s) {
    std::vector<Qrack::QubitIntegerExpectation> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectation() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectation((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedExpectationRdm(int sid, std::vector<int> q, std::vector<int> s, bool r) {
    std::vector<Qrack::QubitIntegerExpectation> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectationRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitRealExpectation> validateFactFpProb(const std::vector<int>& q, const std::vector<double>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitRealExpectation> _q;
    _q.reserve(q.size());
    for (int i = 0; i < q.size(); ++i) {
        _q.push_back(Qrack::QubitRealExpectation((int)q[i], (Qrack::real1)s[i]));
    }
    return _q;
}
double QrackWrapper::FactorizedExpectationFp(int sid, std::vector<int> q, std::vector<double> s) {
    std::vector<Qrack::QubitRealExpectation> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFp() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFp((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedExpectationFpRdm(int sid, std::vector<int> q, std::vector<double> s, bool r) {
    std::vector<Qrack::QubitRealExpectation> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFpRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFpRdm((Qrack::quid)sid, _q, r);
}

void QrackWrapper::PhaseParity(int sid, double lambda, std::vector<int> q) {
    std::vector<bitLenInt>_q;
    _q.reserve(q.size());
    for (size_t i = 0U; i < q.size(); ++i) {
        _q.push_back(q[i]);
    }
    Qrack::PhaseParity((Qrack::quid)sid, (Qrack::real1_f)lambda, _q);
}
double QrackWrapper::JointEnsembleProbability(int sid, std::vector<int> q, std::vector<char> b) {
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

void QrackWrapper::Compose(int sid1, int sid2, std::vector<int> q) {
    Qrack::Compose((Qrack::quid)sid1, (Qrack::quid)sid2, transform_qbids(q));
}
int QrackWrapper::Decompose(int sid, std::vector<int> q) {
    return (int)Qrack::Decompose((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::Dispose(int sid, std::vector<int> q) {
    Qrack::Dispose((Qrack::quid)sid, transform_qbids(q));
}

bool QrackWrapper::M(int sid, int q) {
    return Qrack::M((Qrack::quid)sid, (bitLenInt)q);
}
bool QrackWrapper::ForceM(int sid, int q, bool r) {
    return Qrack::ForceM((Qrack::quid)sid, (bitLenInt)q, r);
}
std::vector<Qrack::QubitPauliBasis> transform_qubit_paulis(const std::vector<int>& q, const std::vector<char>& b, std::string m) {
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
bool QrackWrapper::Measure(int sid, std::vector<int> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Measure() 'q' and 'b' parameter vectors should have same size!");
    return Qrack::Measure((Qrack::quid)sid, _q);
}
unsigned int QrackWrapper::MAll(int sid) {
    return (unsigned int)Qrack::MAll((Qrack::quid)sid);
}
std::vector<unsigned int> QrackWrapper::MeasureShots(int sid, std::vector<int> q, unsigned s) {
    std::vector<unsigned long long> o = Qrack::MeasureShots((Qrack::quid)sid, transform_qbids(q), s);
    std::vector<unsigned int> toRet;
    toRet.reserve(o.size());
    for (size_t i = 0U; i < o.size(); ++i) {
        toRet.push_back((unsigned int)o[i]);
    }
    return toRet;
}
void QrackWrapper::ResetAll(int sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}

void QrackWrapper::X(int sid, int q) {
    Qrack::X((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Y(int sid, int q) {
    Qrack::Y((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Z(int sid, int q) {
    Qrack::Z((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::H(int sid, int q) {
    Qrack::H((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::S(int sid, int q) {
    Qrack::S((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::T(int sid, int q) {
    Qrack::T((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjS(int sid, int q) {
    Qrack::AdjS((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjT(int sid, int q) {
    Qrack::AdjT((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::U(int sid, int q, double theta, double phi, double lambda) {
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
void QrackWrapper::Mtrx(int sid, std::vector<double> m, int q) {
    Qrack::Mtrx((Qrack::quid)sid, transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MCX(int sid, std::vector<int> c, int q) {
    Qrack::MCX((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCY(int sid, std::vector<int> c, int q) {
    Qrack::MCY((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCZ(int sid, std::vector<int> c, int q) {
    Qrack::MCZ((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCH(int sid, std::vector<int> c, int q) {
    Qrack::MCH((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCS(int sid, std::vector<int> c, int q) {
    Qrack::MCS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCT(int sid, std::vector<int> c, int q) {
    Qrack::MCT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjS(int sid, std::vector<int> c, int q) {
    Qrack::MCAdjS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjT(int sid, std::vector<int> c, int q) {
    Qrack::MCAdjT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCU(int sid, std::vector<int> c, int q, double theta, double phi, double lambda) {
    Qrack::MCU((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MCMtrx(int sid, std::vector<int> c, std::vector<double> m, int q) {
    Qrack::MCMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MACX(int sid, std::vector<int> c, int q) {
    Qrack::MACX((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACY(int sid, std::vector<int> c, int q) {
    Qrack::MACY((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACZ(int sid, std::vector<int> c, int q) {
    Qrack::MACZ((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACH(int sid, std::vector<int> c, int q) {
    Qrack::MACH((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACS(int sid, std::vector<int> c, int q) {
    Qrack::MACS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACT(int sid, std::vector<int> c, int q) {
    Qrack::MACT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjS(int sid, std::vector<int> c, int q) {
    Qrack::MACAdjS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjT(int sid, std::vector<int> c, int q) {
    Qrack::MACAdjT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACU(int sid, std::vector<int> c, int q, double theta, double phi, double lambda) {
    Qrack::MACU((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MACMtrx(int sid, std::vector<int> c, std::vector<double> m, int q) {
    Qrack::MACMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::UCMtrx(int sid, std::vector<int> c, std::vector<double> m, int q, int p) {
    Qrack::UCMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q, (bitCapInt)p);
}
void QrackWrapper::Multiplex1Mtrx(int sid, std::vector<int> c, int q, std::vector<double> m) {
    Qrack::Multiplex1Mtrx((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, transform_matrix(m));
}

void QrackWrapper::MX(int sid, std::vector<int> q) {
    Qrack::MX((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::MY(int sid, std::vector<int> q) {
    Qrack::MY((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::MZ(int sid, std::vector<int> q) {
    Qrack::MZ((Qrack::quid)sid, transform_qbids(q));
}

void QrackWrapper::R(int sid, double phi, int q, char b) {
    Qrack::R((Qrack::quid)sid, (Qrack::real1_f)phi, Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}
void QrackWrapper::MCR(int sid, double phi, std::vector<int> c, int q, char b) {
    Qrack::MCR((Qrack::quid)sid, (Qrack::real1_f)phi, transform_qbids(c), Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}

void QrackWrapper::Exp(int sid, double phi, std::vector<int> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Exp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::Exp((Qrack::quid)sid, (Qrack::real1_f)phi, _q);
}
void QrackWrapper::MCExp(int sid, double phi, std::vector<int> c, std::vector<int> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::MCExp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::MCExp((Qrack::quid)sid, (Qrack::real1_f)phi, transform_qbids(c), _q);
}

void QrackWrapper::SWAP(int sid, int qi1, int qi2) {
    Qrack::SWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::ISWAP(int sid, int qi1, int qi2) {
    Qrack::ISWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::AdjISWAP(int sid, int qi1, int qi2) {
    Qrack::AdjISWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::FSim(int sid, double theta, double phi, int qi1, int qi2) {
    Qrack::FSim((Qrack::quid)sid, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::CSWAP(int sid, std::vector<int> c, int qi1, int qi2) {
    Qrack::CSWAP((Qrack::quid)sid, transform_qbids(c), (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::ACSWAP(int sid, std::vector<int> c, int qi1, int qi2) {
    Qrack::ACSWAP((Qrack::quid)sid, transform_qbids(c), (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::AND(int sid, int qi1, int qi2, int qo) {
    Qrack::AND((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::OR(int sid, int qi1, int qi2, int qo) {
    Qrack::OR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::XOR(int sid, int qi1, int qi2, int qo) {
    Qrack::XOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::NAND(int sid, int qi1, int qi2, int qo) {
    Qrack::NAND((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::NOR(int sid, int qi1, int qi2, int qo) {
    Qrack::NOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::XNOR(int sid, int qi1, int qi2, int qo) {
    Qrack::XNOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::CLAND(int sid, bool ci, int qi, int qo) {
    Qrack::CLAND((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLOR(int sid, bool ci, int qi, int qo) {
    Qrack::CLOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLXOR(int sid, bool ci, int qi, int qo) {
    Qrack::CLXOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLNAND(int sid, bool ci, int qi, int qo) {
    Qrack::CLNAND((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLNOR(int sid, bool ci, int qi, int qo) {
    Qrack::CLNOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLXNOR(int sid, bool ci, int qi, int qo) {
    Qrack::CLXNOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}

void QrackWrapper::QFT(int sid, std::vector<int> q) {
    Qrack::QFT((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::IQFT(int sid, std::vector<int> q) {
    Qrack::IQFT((Qrack::quid)sid, transform_qbids(q));
}

void QrackWrapper::ADD(int sid, unsigned int a, std::vector<int> q) {
    Qrack::ADD((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q));
}
void QrackWrapper::SUB(int sid, unsigned int a, std::vector<int> q) {
    Qrack::SUB((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q));
}
void QrackWrapper::ADDS(int sid, unsigned int a, int s, std::vector<int> q) {
    Qrack::ADDS((Qrack::quid)sid, (bitCapInt)a, (bitLenInt)s, transform_qbids(q));
}
void QrackWrapper::SUBS(int sid, unsigned int a, int s, std::vector<int> q) {
    Qrack::SUBS((Qrack::quid)sid, (bitCapInt)a, (bitLenInt)s, transform_qbids(q));
}
void QrackWrapper::MCADD(int sid, unsigned int a, std::vector<int> c, std::vector<int> q) {
    Qrack::MCADD((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q));
}
void QrackWrapper::MCSUB(int sid, unsigned int a, std::vector<int> c, std::vector<int> q) {
    Qrack::MCSUB((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q));
}
void QrackWrapper::MUL(int sid, unsigned int a, std::vector<int> q, std::vector<int> o) {
    Qrack::MUL((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::DIV(int sid, unsigned int a, std::vector<int> q, std::vector<int> o) {
    Qrack::DIV((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MULN(int sid, unsigned int a, unsigned int m, std::vector<int> q, std::vector<int> o) {
    Qrack::MULN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::DIVN(int sid, unsigned int a, unsigned int m, std::vector<int> q, std::vector<int> o) {
    Qrack::DIVN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::POWN(int sid, unsigned int a, unsigned int m, std::vector<int> q, std::vector<int> o) {
    Qrack::POWN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCMUL(int sid, unsigned int a, std::vector<int> c, std::vector<int> q, std::vector<int> o) {
    Qrack::MCMUL((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCDIV(int sid, unsigned int a, std::vector<int> c, std::vector<int> q, std::vector<int> o) {
    Qrack::MCDIV((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCMULN(int sid, unsigned int a, std::vector<int> c, unsigned int m, std::vector<int> q, std::vector<int> o) {
    Qrack::MCMULN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCDIVN(int sid, unsigned int a, std::vector<int> c, unsigned int m, std::vector<int> q, std::vector<int> o) {
    Qrack::MCDIVN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCPOWN(int sid, unsigned int a, std::vector<int> c, unsigned int m, std::vector<int> q, std::vector<int> o) {
    Qrack::MCPOWN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}

int QrackWrapper::init_qneuron(int sid, std::vector<int> c, int q, char f, double a, double tol) {
    return Qrack::init_qneuron((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::QNeuronActivationFn)f, (Qrack::real1_f)a, (Qrack::real1_f)tol);
}
int QrackWrapper::clone_qneuron(int nid) {
    return Qrack::clone_qneuron((Qrack::quid)nid);
}
void QrackWrapper::destroy_qneuron(int nid) {
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
void QrackWrapper::set_qneuron_angles(int nid, std::vector<double> angles) {
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
std::vector<double> QrackWrapper::get_qneuron_angles(int nid) {
    return transform_to_double(Qrack::get_qneuron_angles((Qrack::quid)nid));
}
void QrackWrapper::set_qneuron_alpha(int nid, double alpha) {
    Qrack::set_qneuron_alpha((Qrack::quid)nid, (Qrack::real1_f)alpha);
}
double QrackWrapper::get_qneuron_alpha(int nid) {
    return (double)Qrack::get_qneuron_alpha((Qrack::quid)nid);
}
void QrackWrapper::set_qneuron_activation_fn(int nid, char f) {
    Qrack::set_qneuron_activation_fn((Qrack::quid)nid, (Qrack::QNeuronActivationFn)f);
}
char QrackWrapper::get_qneuron_activation_fn(int nid) {
    return (char)Qrack::get_qneuron_activation_fn((Qrack::quid)nid);
}
double QrackWrapper::qneuron_predict(int nid, bool e, bool r) {
    return (double)Qrack::qneuron_predict((Qrack::quid)nid, e, r);
}
double QrackWrapper::qneuron_unpredict(int nid, bool e) {
    return (double)Qrack::qneuron_unpredict((Qrack::quid)nid, e);
}
double QrackWrapper::qneuron_learn_cycle(int nid, bool e) {
    return (double)Qrack::qneuron_learn_cycle((Qrack::quid)nid, e);
}
void QrackWrapper::qneuron_learn(int nid, double eta, bool e, bool r) {
    Qrack::qneuron_learn((Qrack::quid)nid, (Qrack::real1_f)eta, e, r);
}
void QrackWrapper::qneuron_learn_permutation(int nid, double eta, bool e, bool r) {
    Qrack::qneuron_learn_permutation((Qrack::quid)nid, (Qrack::real1_f)eta, e, r);
}
#include "QrackWrapper.h"

#include "qrack/wasm_api.hpp"

long QrackWrapper::init_general(long length) {
    return (long)Qrack::init_count((bitLenInt)length, false);
}
long QrackWrapper::init_stabilizer(long length) {
    return (long)Qrack::init_count_type((bitLenInt)length, false, false, false, true, false, false, false, false, false, false);
}
long QrackWrapper::init_qbdd(long length) {
    return (long)Qrack::init_qbdd_count((bitLenInt)length);
}
long QrackWrapper::init_clone(long sid) {
    return (long)Qrack::init_clone((Qrack::quid)sid);
}
long QrackWrapper::num_qubits(long sid) {
    return (long)Qrack::num_qubits((Qrack::quid)sid);
}
void QrackWrapper::destroy(long sid) {
    Qrack::destroy((Qrack::quid)sid);
}
void QrackWrapper::allocateQubit(long sid, long qid) {
    Qrack::allocateQubit((Qrack::quid)sid, (Qrack::quid)qid);
}
bool QrackWrapper::release(long sid, long qid) {
    return Qrack::release((Qrack::quid)sid, (Qrack::quid)qid);
}
void QrackWrapper::SetPermutation(long sid, long p) {
    Qrack::SetPermutation((Qrack::quid)sid, p);
}
void QrackWrapper::seed(long sid, long s) {
    Qrack::seed((Qrack::quid)sid, (unsigned)s);
}
bool QrackWrapper::TrySeparate1Qb(long sid, long qi1) {
    return Qrack::TrySeparate1Qb((Qrack::quid)sid, (bitLenInt)qi1);
}
bool QrackWrapper::TrySeparate2Qb(long sid, long qi1, long qi2) {
    return Qrack::TrySeparate2Qb((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
std::vector<bitLenInt> transform_qbids(const std::vector<long>& c) {
    std::vector<bitLenInt> _c;
    _c.reserve(c.size());
    for (size_t i = 0U; i < c.size(); ++i) {
        _c.push_back((bitLenInt)c[i]);
    }

    return _c;
}
bool QrackWrapper::TrySeparateTol(long sid, std::vector<long> q, double tol) {
    return Qrack::TrySeparateTol((Qrack::quid)sid, transform_qbids(q), (Qrack::real1_f)tol);
}
double QrackWrapper::GetUnitaryFidelity(long sid) {
    return (double)Qrack::GetUnitaryFidelity((Qrack::quid)sid);
}
void QrackWrapper::ResetUnitaryFidelity(long sid) {
    Qrack::ResetUnitaryFidelity((Qrack::quid)sid);
}
void QrackWrapper::SetSdrp(long sid, double sdrp) {
    Qrack::SetSdrp((Qrack::quid)sid, (Qrack::real1_f)sdrp);
}
void QrackWrapper::SetNcrp(long sid, double sdrp) {
    Qrack::SetNcrp((Qrack::quid)sid, (Qrack::real1_f)sdrp);
}
void QrackWrapper::SetReactiveSeparate(long sid, bool irs) {
    Qrack::SetReactiveSeparate((Qrack::quid)sid, irs);
}
void QrackWrapper::SetTInjection(long sid, bool iti) {
    Qrack::SetTInjection((Qrack::quid)sid, iti);
}

double QrackWrapper::Prob(long sid, long q) {
    return (double)Qrack::Prob((Qrack::quid)sid, (bitLenInt)q);
}
double QrackWrapper::ProbRdm(long sid, long q) {
    return (double)Qrack::ProbRdm((Qrack::quid)sid, (bitLenInt)q);
}
std::vector<Qrack::QubitIndexState> validatePermProb(const std::vector<long>& q, const std::vector<char>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIndexState> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        _q.emplace_back((long)q[i], (bool)s[i]);
    }
    return _q;
}
double QrackWrapper::PermutationProb(long sid, std::vector<long> q, std::vector<char> s) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProb() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProb((Qrack::quid)sid, _q);
}
double QrackWrapper::PermutationProbRdm(long sid, std::vector<long> q, std::vector<char> s, bool r) {
    std::vector<Qrack::QubitIndexState> _q = validatePermProb(q, s,
        "QrackWrapper::PermutationProbRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::PermutationProbRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitIntegerExpVar> validateFactProb(const std::vector<long>& q, const std::vector<long>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitIntegerExpVar> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        _q.emplace_back((long)q[i], (long)s[i]);
    }
    return _q;
}
double QrackWrapper::FactorizedExpectation(long sid, std::vector<long> q, std::vector<long> s) {
    std::vector<Qrack::QubitIntegerExpVar> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectation() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectation((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedExpectationRdm(long sid, std::vector<long> q, std::vector<long> s, bool r) {
    std::vector<Qrack::QubitIntegerExpVar> _q = validateFactProb(q, s,
        "QrackWrapper::FactorizedExpectationRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitRealExpVar> validateFactFpProb(const std::vector<long>& q, const std::vector<double>& s, std::string m) {
    if (q.size() != s.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitRealExpVar> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        _q.emplace_back((long)q[i], (Qrack::real1)s[i]);
    }
    return _q;
}
double QrackWrapper::FactorizedExpectationFp(long sid, std::vector<long> q, std::vector<double> s) {
    std::vector<Qrack::QubitRealExpVar> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFp() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFp((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedExpectationFpRdm(long sid, std::vector<long> q, std::vector<double> s, bool r) {
    std::vector<Qrack::QubitRealExpVar> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedExpectationFpRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedExpectationFpRdm((Qrack::quid)sid, _q, r);
}
std::vector<Qrack::QubitU3Basis> validateUnitaryExpVar(const std::vector<long>& q, const std::vector<double>& b, std::string m) {
    if ((3U * q.size()) != b.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitU3Basis> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        const long j = 3 * i;
        _q.emplace_back((bitLenInt)q[i], std::vector<Qrack::real1_f>{ (Qrack::real1_f)b[j], (Qrack::real1_f)b[j + 1], (Qrack::real1_f)b[j + 2] });
    }
    return _q;
}
double QrackWrapper::UnitaryExpectation(long sid, std::vector<long> q, std::vector<double> b) {
    std::vector<Qrack::QubitU3Basis> _q = validateUnitaryExpVar(q, b,
        "QrackWrapper::UnitaryExpectation() 'b' parameter vector should be 3 times size of 'q'!");
    return (double)Qrack::UnitaryExpectation((Qrack::quid)sid, _q);
}
std::vector<Qrack::QubitMatrixBasis> validateMatrixExpVar(const std::vector<long>& q, const std::vector<double>& b, std::string m) {
    if ((q.size() << 3U) != b.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitMatrixBasis> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        const long j = 8 * i;
        std::vector<Qrack::complex> mtrx {
            Qrack::complex(b[j + 0], b[j + 1]), Qrack::complex(b[j + 2], b[j + 3]),
            Qrack::complex(b[j + 4], b[j + 5]), Qrack::complex(b[j + 6], b[j + 7])
        };
        _q.emplace_back((bitLenInt)q[i], mtrx);
    }
    return _q;
}
double QrackWrapper::MatrixExpectation(long sid, std::vector<long> q, std::vector<double> b) {
    std::vector<Qrack::QubitMatrixBasis> _q = validateMatrixExpVar(q, b,
        "QrackWrapper::MatrixExpectation() 'b' parameter vector should be 8 times size of 'q'!");
    return (double)Qrack::MatrixExpectation((Qrack::quid)sid, _q);
}
std::vector<Qrack::QubitU3BasisEigenVal> validateUnitaryExpVarEigenVal(const std::vector<long>& q, const std::vector<double>& b, const std::vector<double>& e, std::string m) {
    if (((3U * q.size()) != b.size()) || ((q.size() << 1U) != e.size())) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitU3BasisEigenVal> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        const long j = 3 * i;
        const long k = i << 1;
        _q.emplace_back((bitLenInt)q[i], std::vector<Qrack::real1_f>{ (Qrack::real1_f)b[j], (Qrack::real1_f)b[j + 1], (Qrack::real1_f)b[j + 2] }, std::vector<Qrack::real1_f>{ (Qrack::real1_f)e[k], (Qrack::real1_f)e[k + 1] });
    }
    return _q;
}
double QrackWrapper::UnitaryExpectationEigenVal(long sid, std::vector<long> q, std::vector<double> b, std::vector<double> e) {
    std::vector<Qrack::QubitU3BasisEigenVal> _q = validateUnitaryExpVarEigenVal(q, b, e,
        "QrackWrapper::UnitaryExpectationEigenVal() 'b' parameter vector should be 3 times size of 'q', and 'e' should be 2 times size of 'q'!");
    return (double)Qrack::UnitaryExpectationEigenVal((Qrack::quid)sid, _q);
}
std::vector<Qrack::QubitMatrixBasisEigenVal> validateMatrixExpVarEigenVal(const std::vector<long>& q, const std::vector<double>& b, const std::vector<double>& e, std::string m) {
    if (((q.size() << 3U) != b.size()) || ((q.size() << 1U) != e.size())) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitMatrixBasisEigenVal> _q;
    _q.reserve(q.size());
    for (long i = 0; i < q.size(); ++i) {
        const long j = 8 * i;
        const long k = i << 1;
        std::vector<Qrack::complex> mtrx {
            Qrack::complex(b[j + 0], b[j + 1]), Qrack::complex(b[j + 2], b[j + 3]),
            Qrack::complex(b[j + 4], b[j + 5]), Qrack::complex(b[j + 6], b[j + 7])
        };
        _q.emplace_back((bitLenInt)q[i], mtrx, std::vector<Qrack::real1_f>{ (Qrack::real1_f)e[k], (Qrack::real1_f)e[k + 1] });
    }
    return _q;
}
double QrackWrapper::MatrixExpectationEigenVal(long sid, std::vector<long> q, std::vector<double> b, std::vector<double> e) {
    std::vector<Qrack::QubitMatrixBasisEigenVal> _q = validateMatrixExpVarEigenVal(q, b, e,
        "QrackWrapper::MatrixExpectationEigenVal() 'b' parameter vector should be 8 times size of 'q', and 'e' should be 2 times size of 'q'!");
    return (double)Qrack::MatrixExpectationEigenVal((Qrack::quid)sid, _q);
}
std::vector<Qrack::QubitPauliBasis> transform_qubit_paulis(const std::vector<long>& q, const std::vector<char>& b, std::string m) {
    if (q.size() != b.size()) {
        throw std::invalid_argument(m);
    }
    std::vector<Qrack::QubitPauliBasis> _b;
    _b.reserve(b.size());
    for (size_t i = 0U; i < b.size(); ++i) {
        _b.emplace_back((bitLenInt)q[i], (Qrack::Pauli)b[i]);
    }

    return _b;
}
double QrackWrapper::PauliExpectation(long sid, std::vector<long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis>_q = transform_qubit_paulis(q, b,
        "QrackWrapper::PauliExpectation() 'q' and 'b' parameter vectors should have same size!");
    return (double)Qrack::PauliExpectation((Qrack::quid)sid, _q);
}
double QrackWrapper::Variance(long sid, std::vector<long> q) {
    return (double)Qrack::Variance((Qrack::quid)sid, transform_qbids(q));
}
double QrackWrapper::VarianceRdm(long sid, std::vector<long> q, bool r) {
    return (double)Qrack::VarianceRdm((Qrack::quid)sid, transform_qbids(q), r);
}
double QrackWrapper::FactorizedVariance(long sid, std::vector<long> q, std::vector<long> c) {
    std::vector<Qrack::QubitIntegerExpVar> _q = validateFactProb(q, c,
        "QrackWrapper::FactorizedVariance() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedVariance((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedVarianceRdm(long sid, std::vector<long> q, std::vector<long> c, bool r) {
    std::vector<Qrack::QubitIntegerExpVar> _q = validateFactProb(q, c,
        "QrackWrapper::FactorizedVarianceRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedVarianceRdm((Qrack::quid)sid, _q, r);
}
double QrackWrapper::FactorizedVarianceFp(long sid, std::vector<long> q, std::vector<double> s) {
    std::vector<Qrack::QubitRealExpVar> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedVarianceFp() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedVarianceFp((Qrack::quid)sid, _q);
}
double QrackWrapper::FactorizedVarianceFpRdm(long sid, std::vector<long> q, std::vector<double> s, bool r) {
    std::vector<Qrack::QubitRealExpVar> _q = validateFactFpProb(q, s,
        "QrackWrapper::FactorizedVarianceFpRdm() 'q' and 's' parameter vectors should have same size!");
    return (double)Qrack::FactorizedVarianceFpRdm((Qrack::quid)sid, _q, r);
}
double QrackWrapper::UnitaryVariance(long sid, std::vector<long> q, std::vector<double> b) {
    std::vector<Qrack::QubitU3Basis> _q = validateUnitaryExpVar(q, b,
        "QrackWrapper::UnitaryVariance() 'b' parameter vector should be 3 times size of 'q'!");
    return (double)Qrack::UnitaryVariance((Qrack::quid)sid, _q);
}
double QrackWrapper::MatrixVariance(long sid, std::vector<long> q, std::vector<double> b) {
    std::vector<Qrack::QubitMatrixBasis> _q = validateMatrixExpVar(q, b,
        "QrackWrapper::MatrixVariance() 'b' parameter vector should be 8 times size of 'q'!");
    return (double)Qrack::MatrixVariance((Qrack::quid)sid, _q);
}
double QrackWrapper::UnitaryVarianceEigenVal(long sid, std::vector<long> q, std::vector<double> b, std::vector<double> e) {
    std::vector<Qrack::QubitU3BasisEigenVal> _q = validateUnitaryExpVarEigenVal(q, b, e,
        "QrackWrapper::UnitaryVarianceEigenVal() 'b' parameter vector should be 3 times size of 'q', and 'e' should be 2 times size of 'q'!");
    return (double)Qrack::UnitaryVarianceEigenVal((Qrack::quid)sid, _q);
}
double QrackWrapper::MatrixVarianceEigenVal(long sid, std::vector<long> q, std::vector<double> b, std::vector<double> e) {
    std::vector<Qrack::QubitMatrixBasisEigenVal> _q = validateMatrixExpVarEigenVal(q, b, e,
        "QrackWrapper::MatrixVarianceEigenVal() 'b' parameter vector should be 8 times size of 'q', and 'e' should be 2 times size of 'q'!");
    return (double)Qrack::MatrixVarianceEigenVal((Qrack::quid)sid, _q);
}
double QrackWrapper::PauliVariance(long sid, std::vector<long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis>_q = transform_qubit_paulis(q, b,
        "QrackWrapper::PauliVariance() 'q' and 'b' parameter vectors should have same size!");
    return (double)Qrack::PauliVariance((Qrack::quid)sid, _q);
}

void QrackWrapper::PhaseParity(long sid, double lambda, std::vector<long> q) {
    std::vector<bitLenInt>_q;
    _q.reserve(q.size());
    for (size_t i = 0U; i < q.size(); ++i) {
        _q.push_back(q[i]);
    }
    Qrack::PhaseParity((Qrack::quid)sid, (Qrack::real1_f)lambda, _q);
}
double QrackWrapper::JointEnsembleProbability(long sid, std::vector<long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis>_q = transform_qubit_paulis(q, b,
        "QrackWrapper::JointEnsembleProbability() 'q' and 'b' parameter vectors should have same size!");
    return (double)Qrack::JointEnsembleProbability((Qrack::quid)sid, _q);
}

void QrackWrapper::Compose(long sid1, long sid2, std::vector<long> q) {
    Qrack::Compose((Qrack::quid)sid1, (Qrack::quid)sid2, transform_qbids(q));
}
long QrackWrapper::Decompose(long sid, std::vector<long> q) {
    return (long)Qrack::Decompose((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::Dispose(long sid, std::vector<long> q) {
    Qrack::Dispose((Qrack::quid)sid, transform_qbids(q));
}

bool QrackWrapper::M(long sid, long q) {
    return Qrack::M((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::ForceM(long sid, long q, bool r) {
    // This returns "r," but there's no need to save the input argument.
    Qrack::ForceM((Qrack::quid)sid, (bitLenInt)q, r);
}
bool QrackWrapper::Measure(long sid, std::vector<long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Measure() 'q' and 'b' parameter vectors should have same size!");
    return Qrack::Measure((Qrack::quid)sid, _q);
}
long QrackWrapper::MAll(long sid) {
    return (long)((bitCapIntOcl)Qrack::MAll((Qrack::quid)sid));
}
std::vector<long> QrackWrapper::MeasureShots(long sid, std::vector<long> q, long s) {
    std::vector<long long unsigned int> o = Qrack::MeasureShots((Qrack::quid)sid, transform_qbids(q), (unsigned)s);
    std::vector<long> toRet;
    toRet.reserve(o.size());
    for (size_t i = 0U; i < o.size(); ++i) {
        toRet.push_back((long)o[i]);
    }
    return toRet;
}
void QrackWrapper::ResetAll(long sid) {
    Qrack::ResetAll((Qrack::quid)sid);
}

void QrackWrapper::X(long sid, long q) {
    Qrack::X((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Y(long sid, long q) {
    Qrack::Y((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::Z(long sid, long q) {
    Qrack::Z((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::H(long sid, long q) {
    Qrack::H((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::S(long sid, long q) {
    Qrack::S((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::SX(long sid, long q) {
    Qrack::SX((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjSX(long sid, long q) {
    Qrack::AdjSX((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::SY(long sid, long q) {
    Qrack::SY((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjSY(long sid, long q) {
    Qrack::AdjSY((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::T(long sid, long q) {
    Qrack::T((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjS(long sid, long q) {
    Qrack::AdjS((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::AdjT(long sid, long q) {
    Qrack::AdjT((Qrack::quid)sid, (bitLenInt)q);
}
void QrackWrapper::U(long sid, long q, double theta, double phi, double lambda) {
    Qrack::U((Qrack::quid)sid, (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
std::vector<Qrack::complex> transform_matrix(const std::vector<double>& m) {
    if (m.size() != 8) {
        throw std::invalid_argument("QrackWrapper::Mtrx m argument must be 8 real components for 4 complex numbers of a 2x2 matrix!");
    }
    std::vector<Qrack::complex> _m;
    _m.reserve(4);
    for (size_t i = 0U; i < 4; i+=2) {
        _m.emplace_back(m[2 * i], m[(2 * i) + 1]);
    }

    return _m;
}
void QrackWrapper::Mtrx(long sid, std::vector<double> m, long q) {
    Qrack::Mtrx((Qrack::quid)sid, transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MCX(long sid, std::vector<long> c, long q) {
    Qrack::MCX((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCY(long sid, std::vector<long> c, long q) {
    Qrack::MCY((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCZ(long sid, std::vector<long> c, long q) {
    Qrack::MCZ((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCH(long sid, std::vector<long> c, long q) {
    Qrack::MCH((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCS(long sid, std::vector<long> c, long q) {
    Qrack::MCS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCT(long sid, std::vector<long> c, long q) {
    Qrack::MCT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjS(long sid, std::vector<long> c, long q) {
    Qrack::MCAdjS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCAdjT(long sid, std::vector<long> c, long q) {
    Qrack::MCAdjT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MCU(long sid, std::vector<long> c, long q, double theta, double phi, double lambda) {
    Qrack::MCU((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MCMtrx(long sid, std::vector<long> c, std::vector<double> m, long q) {
    Qrack::MCMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::MACX(long sid, std::vector<long> c, long q) {
    Qrack::MACX((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACY(long sid, std::vector<long> c, long q) {
    Qrack::MACY((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACZ(long sid, std::vector<long> c, long q) {
    Qrack::MACZ((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACH(long sid, std::vector<long> c, long q) {
    Qrack::MACH((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACS(long sid, std::vector<long> c, long q) {
    Qrack::MACS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACT(long sid, std::vector<long> c, long q) {
    Qrack::MACT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjS(long sid, std::vector<long> c, long q) {
    Qrack::MACAdjS((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACAdjT(long sid, std::vector<long> c, long q) {
    Qrack::MACAdjT((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q);
}
void QrackWrapper::MACU(long sid, std::vector<long> c, long q, double theta, double phi, double lambda) {
    Qrack::MACU((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (Qrack::real1_f)lambda);
}
void QrackWrapper::MACMtrx(long sid, std::vector<long> c, std::vector<double> m, long q) {
    Qrack::MACMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q);
}
void QrackWrapper::UCMtrx(long sid, std::vector<long> c, std::vector<double> m, long q, long p) {
    Qrack::UCMtrx((Qrack::quid)sid, transform_qbids(c), transform_matrix(m), (bitLenInt)q, (unsigned)p);
}
void QrackWrapper::Multiplex1Mtrx(long sid, std::vector<long> c, long q, std::vector<double> m) {
    Qrack::Multiplex1Mtrx((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, transform_matrix(m));
}

void QrackWrapper::MX(long sid, std::vector<long> q) {
    Qrack::MX((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::MY(long sid, std::vector<long> q) {
    Qrack::MY((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::MZ(long sid, std::vector<long> q) {
    Qrack::MZ((Qrack::quid)sid, transform_qbids(q));
}

void QrackWrapper::R(long sid, double phi, long q, char b) {
    Qrack::R((Qrack::quid)sid, (Qrack::real1_f)phi, Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}
void QrackWrapper::MCR(long sid, double phi, std::vector<long> c, long q, char b) {
    Qrack::MCR((Qrack::quid)sid, (Qrack::real1_f)phi, transform_qbids(c), Qrack::QubitPauliBasis((bitLenInt)q, (Qrack::Pauli)b));
}

void QrackWrapper::Exp(long sid, double phi, std::vector<long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::Exp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::Exp((Qrack::quid)sid, (Qrack::real1_f)phi, _q);
}
void QrackWrapper::MCExp(long sid, double phi, std::vector<long> c, std::vector<long> q, std::vector<char> b) {
    std::vector<Qrack::QubitPauliBasis> _q = transform_qubit_paulis(q, b,
        "QrackWrapper::MCExp() 'q' and 'b' parameter vectors should have same size!");
    Qrack::MCExp((Qrack::quid)sid, (Qrack::real1_f)phi, transform_qbids(c), _q);
}

void QrackWrapper::SWAP(long sid, long qi1, long qi2) {
    Qrack::SWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::ISWAP(long sid, long qi1, long qi2) {
    Qrack::ISWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::AdjISWAP(long sid, long qi1, long qi2) {
    Qrack::AdjISWAP((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::FSim(long sid, double theta, double phi, long qi1, long qi2) {
    Qrack::FSim((Qrack::quid)sid, (Qrack::real1_f)theta, (Qrack::real1_f)phi, (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::CSWAP(long sid, std::vector<long> c, long qi1, long qi2) {
    Qrack::CSWAP((Qrack::quid)sid, transform_qbids(c), (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::ACSWAP(long sid, std::vector<long> c, long qi1, long qi2) {
    Qrack::ACSWAP((Qrack::quid)sid, transform_qbids(c), (bitLenInt)qi1, (bitLenInt)qi2);
}
void QrackWrapper::AND(long sid, long qi1, long qi2, long qo) {
    Qrack::AND((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::OR(long sid, long qi1, long qi2, long qo) {
    Qrack::OR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::XOR(long sid, long qi1, long qi2, long qo) {
    Qrack::XOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::NAND(long sid, long qi1, long qi2, long qo) {
    Qrack::NAND((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::NOR(long sid, long qi1, long qi2, long qo) {
    Qrack::NOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::XNOR(long sid, long qi1, long qi2, long qo) {
    Qrack::XNOR((Qrack::quid)sid, (bitLenInt)qi1, (bitLenInt)qi2, (bitLenInt)qo);
}
void QrackWrapper::CLAND(long sid, bool ci, long qi, long qo) {
    Qrack::CLAND((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLOR(long sid, bool ci, long qi, long qo) {
    Qrack::CLOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLXOR(long sid, bool ci, long qi, long qo) {
    Qrack::CLXOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLNAND(long sid, bool ci, long qi, long qo) {
    Qrack::CLNAND((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLNOR(long sid, bool ci, long qi, long qo) {
    Qrack::CLNOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}
void QrackWrapper::CLXNOR(long sid, bool ci, long qi, long qo) {
    Qrack::CLXNOR((Qrack::quid)sid, ci, (bitLenInt)qi, (bitLenInt)qo);
}

void QrackWrapper::QFT(long sid, std::vector<long> q) {
    Qrack::QFT((Qrack::quid)sid, transform_qbids(q));
}
void QrackWrapper::IQFT(long sid, std::vector<long> q) {
    Qrack::IQFT((Qrack::quid)sid, transform_qbids(q));
}

void QrackWrapper::ADD(long sid, long a, std::vector<long> q) {
    Qrack::ADD((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q));
}
void QrackWrapper::SUB(long sid, long a, std::vector<long> q) {
    Qrack::SUB((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q));
}
void QrackWrapper::ADDS(long sid, long a, long s, std::vector<long> q) {
    Qrack::ADDS((Qrack::quid)sid, (bitCapInt)a, (bitLenInt)s, transform_qbids(q));
}
void QrackWrapper::SUBS(long sid, long a, long s, std::vector<long> q) {
    Qrack::SUBS((Qrack::quid)sid, (bitCapInt)a, (bitLenInt)s, transform_qbids(q));
}
void QrackWrapper::MCADD(long sid, long a, std::vector<long> c, std::vector<long> q) {
    Qrack::MCADD((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q));
}
void QrackWrapper::MCSUB(long sid, long a, std::vector<long> c, std::vector<long> q) {
    Qrack::MCSUB((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q));
}
void QrackWrapper::MUL(long sid, long a, std::vector<long> q, std::vector<long> o) {
    Qrack::MUL((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::DIV(long sid, long a, std::vector<long> q, std::vector<long> o) {
    Qrack::DIV((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MULN(long sid, long a, long m, std::vector<long> q, std::vector<long> o) {
    Qrack::MULN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::DIVN(long sid, long a, long m, std::vector<long> q, std::vector<long> o) {
    Qrack::DIVN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::POWN(long sid, long a, long m, std::vector<long> q, std::vector<long> o) {
    Qrack::POWN((Qrack::quid)sid, (bitCapInt)a, (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCMUL(long sid, long a, std::vector<long> c, std::vector<long> q, std::vector<long> o) {
    Qrack::MCMUL((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCDIV(long sid, long a, std::vector<long> c, std::vector<long> q, std::vector<long> o) {
    Qrack::MCDIV((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCMULN(long sid, long a, std::vector<long> c, long m, std::vector<long> q, std::vector<long> o) {
    Qrack::MCMULN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCDIVN(long sid, long a, std::vector<long> c, long m, std::vector<long> q, std::vector<long> o) {
    Qrack::MCDIVN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(q), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}
void QrackWrapper::MCPOWN(long sid, long a, std::vector<long> c, long m, std::vector<long> q, std::vector<long> o) {
    Qrack::MCPOWN((Qrack::quid)sid, (bitCapInt)a, transform_qbids(c), (bitCapInt)m, transform_qbids(q), transform_qbids(o));
}

long QrackWrapper::init_qneuron(long sid, std::vector<long> c, long q, char f, double a, double tol) {
    return Qrack::init_qneuron((Qrack::quid)sid, transform_qbids(c), (bitLenInt)q, (Qrack::QNeuronActivationFn)f, (Qrack::real1_f)a, (Qrack::real1_f)tol);
}
long QrackWrapper::clone_qneuron(long nid) {
    return Qrack::clone_qneuron((Qrack::quid)nid);
}
void QrackWrapper::destroy_qneuron(long nid) {
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
void QrackWrapper::set_qneuron_angles(long nid, std::vector<double> angles) {
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
std::vector<double> QrackWrapper::get_qneuron_angles(long nid) {
    return transform_to_double(Qrack::get_qneuron_angles((Qrack::quid)nid));
}
void QrackWrapper::set_qneuron_alpha(long nid, double alpha) {
    Qrack::set_qneuron_alpha((Qrack::quid)nid, (Qrack::real1_f)alpha);
}
double QrackWrapper::get_qneuron_alpha(long nid) {
    return (double)Qrack::get_qneuron_alpha((Qrack::quid)nid);
}
void QrackWrapper::set_qneuron_activation_fn(long nid, char f) {
    Qrack::set_qneuron_activation_fn((Qrack::quid)nid, (Qrack::QNeuronActivationFn)f);
}
char QrackWrapper::get_qneuron_activation_fn(long nid) {
    return (char)Qrack::get_qneuron_activation_fn((Qrack::quid)nid);
}
double QrackWrapper::qneuron_predict(long nid, bool e, bool r) {
    return (double)Qrack::qneuron_predict((Qrack::quid)nid, e, r);
}
double QrackWrapper::qneuron_unpredict(long nid, bool e) {
    return (double)Qrack::qneuron_unpredict((Qrack::quid)nid, e);
}
double QrackWrapper::qneuron_learn_cycle(long nid, bool e) {
    return (double)Qrack::qneuron_learn_cycle((Qrack::quid)nid, e);
}
void QrackWrapper::qneuron_learn(long nid, double eta, bool e, bool r) {
    Qrack::qneuron_learn((Qrack::quid)nid, (Qrack::real1_f)eta, e, r);
}
void QrackWrapper::qneuron_learn_permutation(long nid, double eta, bool e, bool r) {
    Qrack::qneuron_learn_permutation((Qrack::quid)nid, (Qrack::real1_f)eta, e, r);
}
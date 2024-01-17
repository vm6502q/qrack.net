#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    emscripten::register_vector<long long>("VectorLong");
    emscripten::register_vector<double>("VectorDouble");
    emscripten::register_vector<char>("VectorChar");

    // Utility
    function("init_general", optional_override([](long long length) -> long long {
        return QrackWrapper::init_general(length);
    }));
    function("init_stabilizer", optional_override([](long long length) -> long long {
        return QrackWrapper::init_stabilizer(length);
    }));
    function("init_qbdd", optional_override([](long long length) -> long long {
        return QrackWrapper::init_qbdd(length);
    }));
    function("init_clone", optional_override([](long long sid) -> long long {
        return QrackWrapper::init_clone(sid);
    }));
    function("num_qubits", optional_override([](long long sid) -> long long {
        return QrackWrapper::num_qubits(sid);
    }));
    function("allocate_qubit", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::allocateQubit(sid, q);
    }));
    function("release_qubit", optional_override([](long long sid, long long q) -> bool {
        return QrackWrapper::release(sid, q);
    }));
    function("destroy", optional_override([](long long sid) -> void {
        QrackWrapper::destroy(sid);
    }));
    function("seed", optional_override([](long long sid, long long s) -> void {
        QrackWrapper::seed(sid, s);
    }));

    // Expectation value output
    function("prob", optional_override([](long long sid, long long q) -> double {
        return QrackWrapper::Prob(sid, q);
    }));
    function("prob_rdm", optional_override([](long long sid, long long q) -> double {
        return QrackWrapper::ProbRdm(sid, q);
    }));
    function("perm_prob", optional_override([](long long sid, std::vector<long long> q, std::vector<char> s) -> double {
        return QrackWrapper::PermutationProb(sid, q, s);
    }));
    function("perm_prob_rdm", optional_override([](long long sid, std::vector<long long> q, std::vector<char> s, bool r) -> double {
        return QrackWrapper::PermutationProbRdm(sid, q, s, r);
    }));
    function("fact_exp", optional_override([](long long sid, std::vector<long long> q, std::vector<long long> s) -> double {
        return QrackWrapper::FactorizedExpectation(sid, q, s);
    }));
    function("fact_exp_rdm", optional_override([](long long sid, std::vector<long long> q, std::vector<long long> s, bool r) -> double {
        return QrackWrapper::FactorizedExpectationRdm(sid, q, s, r);
    }));
    function("fact_exp_fp", optional_override([](long long sid, std::vector<long long> q, std::vector<double> s) -> double {
        return QrackWrapper::FactorizedExpectationFp(sid, q, s);
    }));
    function("fact_exp_fp_rdm", optional_override([](long long sid, std::vector<long long> q, std::vector<double> s, bool r) -> double {
        return QrackWrapper::FactorizedExpectationFpRdm(sid, q, s, r);
    }));

    // Parity
    function("phase_parity", optional_override([](long long sid, double lambda, std::vector<long long> q) -> void {
        QrackWrapper::PhaseParity(sid, lambda, q);
    }));
    function("joint_ensemble_prob", optional_override([](long long sid, std::vector<long long> q, std::vector<char> b) -> double {
        return QrackWrapper::JointEnsembleProbability(sid, q, b);
    }));

    // Schmidt decomposition
    function("compose", optional_override([](long long sid1, long long sid2, std::vector<long long> q) -> void {
        QrackWrapper::Compose(sid1, sid2, q);
    }));
    function("decompose", optional_override([](long long sid, std::vector<long long> q) -> long long {
        return QrackWrapper::Decompose(sid, q);
    }));
    function("dispose", optional_override([](long long sid, std::vector<long long> q) -> void {
        QrackWrapper::Dispose(sid, q);
    }));

    // SPAM and non-unitary
    function("reset_all", optional_override([](long long sid) -> void {
        QrackWrapper::ResetAll(sid);
    }));
    function("measure", optional_override([](long long sid, long long q) -> bool {
        return QrackWrapper::M(sid, q);
    }));
    function("force_measure", optional_override([](long long sid, long long q, bool v) -> bool {
        return QrackWrapper::ForceM(sid, q, v);
    }));
    function("measure_basis", optional_override([](long long sid, std::vector<long long> q, std::vector<char> b) -> bool {
        return QrackWrapper::Measure(sid, q, b);
    }));
    function("measure_all", optional_override([](long long sid) -> unsigned long long {
        return QrackWrapper::MAll(sid);
    }));
    function("measure_shots", optional_override([](long long sid, std::vector<long long> q, unsigned s) -> std::vector<unsigned long long> {
        return QrackWrapper::MeasureShots(sid, q, s);
    }));

    // single-qubit gates
    function("x", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::X(sid, q);
    }));
    function("y", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::Y(sid, q);
    }));
    function("z", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::Z(sid, q);
    }));
    function("h", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::H(sid, q);
    }));
    function("s", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::S(sid, q);
    }));
    function("t", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::T(sid, q);
    }));
    function("adjs", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::AdjS(sid, q);
    }));
    function("adjt", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::AdjT(sid, q);
    }));
    function("U", optional_override([](long long sid, long long q, double theta, double phi, double lambda) -> void {
        QrackWrapper::U(sid, q, theta, phi, lambda);
    }));
    function("mtrx", optional_override([](long long sid, std::vector<double> m, long long q) -> void {
        QrackWrapper::Mtrx(sid, m, q);
    }));

    // multi-controlled single-qubit gates
    function("mcx", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCX(sid, c, q);
    }));
    function("mcy", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCY(sid, c, q);
    }));
    function("mcz", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCZ(sid, c, q);
    }));
    function("mch", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCH(sid, c, q);
    }));
    function("mcs", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCS(sid, c, q);
    }));
    function("mct", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCT(sid, c, q);
    }));
    function("mcadjs", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCAdjS(sid, c, q);
    }));
    function("mcadjt", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MCAdjT(sid, c, q);
    }));
    function("mcu", optional_override([](long long sid, std::vector<long long> c, long long q, double theta, double phi, double lambda) -> void {
        QrackWrapper::MCU(sid, c, q, theta, phi, lambda);
    }));
    function("mcmtrx", optional_override([](long long sid, std::vector<long long> c, std::vector<double> m, long long q) -> void {
        QrackWrapper::MCMtrx(sid, c, m, q);
    }));
    function("macx", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACX(sid, c, q);
    }));
    function("macy", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACY(sid, c, q);
    }));
    function("macz", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACZ(sid, c, q);
    }));
    function("mach", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACH(sid, c, q);
    }));
    function("macs", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACS(sid, c, q);
    }));
    function("mact", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACT(sid, c, q);
    }));
    function("macadjs", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACAdjS(sid, c, q);
    }));
    function("macadjt", optional_override([](long long sid, std::vector<long long> c, long long q) -> void {
        QrackWrapper::MACAdjT(sid, c, q);
    }));
    function("macu", optional_override([](long long sid, std::vector<long long> c, long long q, double theta, double phi, double lambda) -> void {
        QrackWrapper::MACU(sid, c, q, theta, phi, lambda);
    }));
    function("macmtrx", optional_override([](long long sid, std::vector<long long> c, std::vector<double> m, long long q) -> void {
        QrackWrapper::MACMtrx(sid, c, m, q);
    }));
    function("ucmtrx", optional_override([](long long sid, std::vector<long long> c, std::vector<double> m, long long q, long long p) -> void {
        QrackWrapper::UCMtrx(sid, c, m, q, p);
    }));
    function("multiplex_1qb_mtrx", optional_override([](long long sid, std::vector<long long> c, long long q, std::vector<double> m) -> void {
        QrackWrapper::Multiplex1Mtrx(sid, c, q, m);
    }));

    // coalesced single-qubit gates
    function("mx", optional_override([](long long sid, std::vector<long long> q) -> void {
        QrackWrapper::MX(sid, q);
    }));
    function("my", optional_override([](long long sid, std::vector<long long> q) -> void {
        QrackWrapper::MY(sid, q);
    }));
    function("mz", optional_override([](long long sid, std::vector<long long> q) -> void {
        QrackWrapper::MZ(sid, q);
    }));

    // single-qubit rotations
    function("r", optional_override([](long long sid, double phi, long long q, char b) -> void {
        QrackWrapper::R(sid, phi, q, b);
    }));
    // multi-controlled single-qubit rotations
    function("mcr", optional_override([](long long sid, double phi, std::vector<long long> c, long long q, char b) -> void {
        QrackWrapper::MCR(sid, phi, c, q, b);
    }));

    // exponential of Pauli operators
    function("exp", optional_override([](long long sid, double phi, std::vector<long long> q, std::vector<char> b) -> void {
        QrackWrapper::Exp(sid, phi, q, b);
    }));
    // multi-controlled exponential of Pauli operators
    function("mcexp", optional_override([](long long sid, double phi, std::vector<long long> c, std::vector<long long> q, std::vector<char> b) -> void {
        QrackWrapper::MCExp(sid, phi, c, q, b);
    }));

    // swap variants
    function("swap", optional_override([](long long sid, long long q1, long long q2) -> void {
        QrackWrapper::SWAP(sid, q1, q2);
    }));
    function("iswap", optional_override([](long long sid, long long q1, long long q2) -> void {
        QrackWrapper::ISWAP(sid, q1, q2);
    }));
    function("adjiswap", optional_override([](long long sid, long long q1, long long q2) -> void {
        QrackWrapper::AdjISWAP(sid, q1, q2);
    }));
    function("fsim", optional_override([](long long sid, double theta, double phi, long long q1, long long q2) -> void {
        QrackWrapper::FSim(sid, theta, phi, q1, q2);
    }));
    function("mcswap", optional_override([](long long sid, std::vector<long long> c, long long q1, long long q2) -> void {
        QrackWrapper::CSWAP(sid, c, q1, q2);
    }));
    function("macswap", optional_override([](long long sid, std::vector<long long> c, long long q1, long long q2) -> void {
        QrackWrapper::ACSWAP(sid, c, q1, q2);
    }));

    // Quantum boolean (Toffoli) operations
    function("and", optional_override([](long long sid, long long qi1, long long qi2, long long qo) -> void {
        QrackWrapper::AND(sid, qi1, qi2, qo);
    }));
    function("or", optional_override([](long long sid, long long qi1, long long qi2, long long qo) -> void {
        QrackWrapper::OR(sid, qi1, qi2, qo);
    }));
    function("xor", optional_override([](long long sid, long long qi1, long long qi2, long long qo) -> void {
        QrackWrapper::XOR(sid, qi1, qi2, qo);
    }));
    function("nand", optional_override([](long long sid, long long qi1, long long qi2, long long qo) -> void {
        QrackWrapper::NAND(sid, qi1, qi2, qo);
    }));
    function("nor", optional_override([](long long sid, long long qi1, long long qi2, long long qo) -> void {
        QrackWrapper::NOR(sid, qi1, qi2, qo);
    }));
    function("xnor", optional_override([](long long sid, long long qi1, long long qi2, long long qo) -> void {
        QrackWrapper::XNOR(sid, qi1, qi2, qo);
    }));
    function("cland", optional_override([](long long sid, bool ci, long long qi, long long qo) -> void {
        QrackWrapper::CLAND(sid, ci, qi, qo);
    }));
    function("clor", optional_override([](long long sid, bool ci, long long qi, long long qo) -> void {
        QrackWrapper::CLOR(sid, ci, qi, qo);
    }));
    function("clxor", optional_override([](long long sid, bool ci, long long qi, long long qo) -> void {
        QrackWrapper::CLXOR(sid, ci, qi, qo);
    }));
    function("clnand", optional_override([](long long sid, bool ci, long long qi, long long qo) -> void {
        QrackWrapper::CLNAND(sid, ci, qi, qo);
    }));
    function("clnor", optional_override([](long long sid, bool ci, long long qi, long long qo) -> void {
        QrackWrapper::CLNOR(sid, ci, qi, qo);
    }));
    function("clxnor", optional_override([](long long sid, bool ci, long long qi, long long qo) -> void {
        QrackWrapper::CLXNOR(sid, ci, qi, qo);
    }));
}
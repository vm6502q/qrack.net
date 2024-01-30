#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    emscripten::register_vector<long>("VectorInt");
    emscripten::register_vector<double>("VectorDouble");
    emscripten::register_vector<char>("VectorChar");

    // **NOTE** - 'write_bool(bool) -> bool' is in the method set.

    // Utility
    function("init_general", optional_override([](long length) -> long {
        return QrackWrapper::init_general(length);
    }));
    function("init_stabilizer", optional_override([](long length) -> long {
        return QrackWrapper::init_stabilizer(length);
    }));
    function("init_qbdd", optional_override([](long length) -> long {
        return QrackWrapper::init_qbdd(length);
    }));
    function("init_clone", optional_override([](long sid) -> long {
        return QrackWrapper::init_clone(sid);
    }));
    function("num_qubits", optional_override([](long sid) -> long {
        return QrackWrapper::num_qubits(sid);
    }));
    function("allocate_qubit", optional_override([](long sid, long q) -> void {
        QrackWrapper::allocateQubit(sid, q);
    }));
    function("release_qubit", optional_override([](long sid, long q) -> bool {
        return QrackWrapper::release(sid, q);
    }));
    function("destroy", optional_override([](long sid) -> void {
        QrackWrapper::destroy(sid);
    }));
    function("seed", optional_override([](long sid, long s) -> void {
        QrackWrapper::seed(sid, s);
    }));
    function("try_separate_1qb", optional_override([](long sid, long qi1) -> bool {
        return QrackWrapper::TrySeparate1Qb(sid, qi1);
    }));
    function("try_separate_2qb", optional_override([](long sid, long qi1, long qi2) -> bool {
        return QrackWrapper::TrySeparate2Qb(sid, qi1, qi2);
    }));
    function("try_separate_tol", optional_override([](long sid, std::vector<long> q, double tol) -> bool {
        return QrackWrapper::TrySeparateTol(sid, q, tol);
    }));
    function("get_unitary_fidelity", optional_override([](long sid) -> double {
        return QrackWrapper::GetUnitaryFidelity(sid);
    }));
    function("reset_unitary_fidelity", optional_override([](long sid) -> void {
        QrackWrapper::GetUnitaryFidelity(sid);
    }));
    function("set_sdrp", optional_override([](long sid, double sdrp) -> void {
        QrackWrapper::SetSdrp(sid, sdrp);
    }));
    function("set_ncrp", optional_override([](long sid, double ncrp) -> void {
        QrackWrapper::SetNcrp(sid, ncrp);
    }));
    function("set_reactive_separate", optional_override([](long sid, bool irs) -> void {
        QrackWrapper::SetReactiveSeparate(sid, irs);
    }));
    function("set_t_injection", optional_override([](long sid, bool iti) -> void {
        QrackWrapper::SetTInjection(sid, iti);
    }));

    // Expectation value output
    function("prob", optional_override([](long sid, long q) -> double {
        return QrackWrapper::Prob(sid, q);
    }));
    function("prob_rdm", optional_override([](long sid, long q) -> double {
        return QrackWrapper::ProbRdm(sid, q);
    }));
    function("perm_prob", optional_override([](long sid, std::vector<long> q, std::vector<char> s) -> double {
        return QrackWrapper::PermutationProb(sid, q, s);
    }));
    function("perm_prob_rdm", optional_override([](long sid, std::vector<long> q, std::vector<char> s, bool r) -> double {
        return QrackWrapper::PermutationProbRdm(sid, q, s, r);
    }));
    function("fact_exp", optional_override([](long sid, std::vector<long> q, std::vector<long> s) -> double {
        return QrackWrapper::FactorizedExpectation(sid, q, s);
    }));
    function("fact_exp_rdm", optional_override([](long sid, std::vector<long> q, std::vector<long> s, bool r) -> double {
        return QrackWrapper::FactorizedExpectationRdm(sid, q, s, r);
    }));
    function("fact_exp_fp", optional_override([](long sid, std::vector<long> q, std::vector<double> s) -> double {
        return QrackWrapper::FactorizedExpectationFp(sid, q, s);
    }));
    function("fact_exp_fp_rdm", optional_override([](long sid, std::vector<long> q, std::vector<double> s, bool r) -> double {
        return QrackWrapper::FactorizedExpectationFpRdm(sid, q, s, r);
    }));

    // Parity
    function("phase_parity", optional_override([](long sid, std::vector<long> q, double lambda) -> void {
        QrackWrapper::PhaseParity(sid, lambda, q);
    }));
    function("joint_ensemble_prob", optional_override([](long sid, std::vector<long> q, std::vector<char> b) -> double {
        return QrackWrapper::JointEnsembleProbability(sid, q, b);
    }));

    // Schmidt decomposition
    function("compose", optional_override([](long sid1, long sid2, std::vector<long> q) -> void {
        QrackWrapper::Compose(sid1, sid2, q);
    }));
    function("decompose", optional_override([](long sid, std::vector<long> q) -> long {
        return QrackWrapper::Decompose(sid, q);
    }));
    function("dispose", optional_override([](long sid, std::vector<long> q) -> void {
        QrackWrapper::Dispose(sid, q);
    }));

    // SPAM and non-unitary
    function("reset_all", optional_override([](long sid) -> void {
        QrackWrapper::ResetAll(sid);
    }));
    function("measure", optional_override([](long sid, long q) -> bool {
        return QrackWrapper::M(sid, q);
    }));
    function("force_measure", optional_override([](long sid, long q, bool v) -> void {
        QrackWrapper::ForceM(sid, q, v);
    }));
    function("measure_basis", optional_override([](long sid, std::vector<long> q, std::vector<char> b) -> bool {
        return QrackWrapper::Measure(sid, q, b);
    }));
    function("measure_all", optional_override([](long sid) -> long {
        return QrackWrapper::MAll(sid);
    }));
    function("measure_shots", optional_override([](long sid, std::vector<long> q, long s) -> std::vector<long> {
        return QrackWrapper::MeasureShots(sid, q, s);
    }));

    // single-qubit gates
    function("x", optional_override([](long sid, long q) -> void {
        QrackWrapper::X(sid, q);
    }));
    function("y", optional_override([](long sid, long q) -> void {
        QrackWrapper::Y(sid, q);
    }));
    function("z", optional_override([](long sid, long q) -> void {
        QrackWrapper::Z(sid, q);
    }));
    function("h", optional_override([](long sid, long q) -> void {
        QrackWrapper::H(sid, q);
    }));
    function("s", optional_override([](long sid, long q) -> void {
        QrackWrapper::S(sid, q);
    }));
    /// square root of x gate
    function("sx", optional_override([](long sid, long q) -> void {
        QrackWrapper::SX(sid, q);
    }));
    /// square root of y gate
    function("sy", optional_override([](long sid, long q) -> void {
        QrackWrapper::SY(sid, q);
    }));
    function("t", optional_override([](long sid, long q) -> void {
        QrackWrapper::T(sid, q);
    }));
    function("adjs", optional_override([](long sid, long q) -> void {
        QrackWrapper::AdjS(sid, q);
    }));
    /// inverse square root of x gate
    function("adjsx", optional_override([](long sid, long q) -> void {
        QrackWrapper::AdjSX(sid, q);
    }));
    /// inverse square root of y gate
    function("adjsy", optional_override([](long sid, long q) -> void {
        QrackWrapper::AdjSY(sid, q);
    }));
    function("adjt", optional_override([](long sid, long q) -> void {
        QrackWrapper::AdjT(sid, q);
    }));
    function("u", optional_override([](long sid, long q, double theta, double phi, double lambda) -> void {
        QrackWrapper::U(sid, q, theta, phi, lambda);
    }));
    function("mtrx", optional_override([](long sid, std::vector<double> m, long q) -> void {
        QrackWrapper::Mtrx(sid, m, q);
    }));

    // multi-controlled single-qubit gates
    function("mcx", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCX(sid, c, q);
    }));
    function("mcy", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCY(sid, c, q);
    }));
    function("mcz", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCZ(sid, c, q);
    }));
    function("mch", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCH(sid, c, q);
    }));
    function("mcs", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCS(sid, c, q);
    }));
    function("mct", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCT(sid, c, q);
    }));
    function("mcadjs", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCAdjS(sid, c, q);
    }));
    function("mcadjt", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MCAdjT(sid, c, q);
    }));
    function("mcu", optional_override([](long sid, std::vector<long> c, long q, double theta, double phi, double lambda) -> void {
        QrackWrapper::MCU(sid, c, q, theta, phi, lambda);
    }));
    function("mcmtrx", optional_override([](long sid, std::vector<long> c, std::vector<double> m, long q) -> void {
        QrackWrapper::MCMtrx(sid, c, m, q);
    }));
    function("macx", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACX(sid, c, q);
    }));
    function("macy", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACY(sid, c, q);
    }));
    function("macz", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACZ(sid, c, q);
    }));
    function("mach", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACH(sid, c, q);
    }));
    function("macs", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACS(sid, c, q);
    }));
    function("mact", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACT(sid, c, q);
    }));
    function("macadjs", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACAdjS(sid, c, q);
    }));
    function("macadjt", optional_override([](long sid, std::vector<long> c, long q) -> void {
        QrackWrapper::MACAdjT(sid, c, q);
    }));
    function("macu", optional_override([](long sid, std::vector<long> c, long q, double theta, double phi, double lambda) -> void {
        QrackWrapper::MACU(sid, c, q, theta, phi, lambda);
    }));
    function("macmtrx", optional_override([](long sid, std::vector<long> c, std::vector<double> m, long q) -> void {
        QrackWrapper::MACMtrx(sid, c, m, q);
    }));
    function("ucmtrx", optional_override([](long sid, std::vector<long> c, std::vector<double> m, long q, long p) -> void {
        QrackWrapper::UCMtrx(sid, c, m, q, p);
    }));
    function("multiplex_1qb_mtrx", optional_override([](long sid, std::vector<long> c, std::vector<double> m, long q) -> void {
        QrackWrapper::Multiplex1Mtrx(sid, c, q, m);
    }));

    // coalesced single-qubit gates
    function("mx", optional_override([](long sid, std::vector<long> q) -> void {
        QrackWrapper::MX(sid, q);
    }));
    function("my", optional_override([](long sid, std::vector<long> q) -> void {
        QrackWrapper::MY(sid, q);
    }));
    function("mz", optional_override([](long sid, std::vector<long> q) -> void {
        QrackWrapper::MZ(sid, q);
    }));

    // single-qubit rotations
    function("r", optional_override([](long sid, double phi, long q, char b) -> void {
        QrackWrapper::R(sid, phi, q, b);
    }));
    // multi-controlled single-qubit rotations
    function("mcr", optional_override([](long sid, std::vector<long> c, double phi, long q, char b) -> void {
        QrackWrapper::MCR(sid, phi, c, q, b);
    }));

    // exponential of Pauli operators
    function("exp", optional_override([](long sid, std::vector<long> q, std::vector<char> b, double phi) -> void {
        QrackWrapper::Exp(sid, phi, q, b);
    }));
    // multi-controlled exponential of Pauli operators
    function("mcexp", optional_override([](long sid, std::vector<long> c, std::vector<long> q, std::vector<char> b, double phi) -> void {
        QrackWrapper::MCExp(sid, phi, c, q, b);
    }));

    // swap variants
    function("swap", optional_override([](long sid, long q1, long q2) -> void {
        QrackWrapper::SWAP(sid, q1, q2);
    }));
    function("iswap", optional_override([](long sid, long q1, long q2) -> void {
        QrackWrapper::ISWAP(sid, q1, q2);
    }));
    function("adjiswap", optional_override([](long sid, long q1, long q2) -> void {
        QrackWrapper::AdjISWAP(sid, q1, q2);
    }));
    function("fsim", optional_override([](long sid, double theta, double phi, long q1, long q2) -> void {
        QrackWrapper::FSim(sid, theta, phi, q1, q2);
    }));
    function("mcswap", optional_override([](long sid, std::vector<long> c, long q1, long q2) -> void {
        QrackWrapper::CSWAP(sid, c, q1, q2);
    }));
    function("macswap", optional_override([](long sid, std::vector<long> c, long q1, long q2) -> void {
        QrackWrapper::ACSWAP(sid, c, q1, q2);
    }));

    // Quantum boolean (Toffoli) operations
    function("and", optional_override([](long sid, long qi1, long qi2, long qo) -> void {
        QrackWrapper::AND(sid, qi1, qi2, qo);
    }));
    function("or", optional_override([](long sid, long qi1, long qi2, long qo) -> void {
        QrackWrapper::OR(sid, qi1, qi2, qo);
    }));
    function("xor", optional_override([](long sid, long qi1, long qi2, long qo) -> void {
        QrackWrapper::XOR(sid, qi1, qi2, qo);
    }));
    function("nand", optional_override([](long sid, long qi1, long qi2, long qo) -> void {
        QrackWrapper::NAND(sid, qi1, qi2, qo);
    }));
    function("nor", optional_override([](long sid, long qi1, long qi2, long qo) -> void {
        QrackWrapper::NOR(sid, qi1, qi2, qo);
    }));
    function("xnor", optional_override([](long sid, long qi1, long qi2, long qo) -> void {
        QrackWrapper::XNOR(sid, qi1, qi2, qo);
    }));
    function("cland", optional_override([](long sid, bool ci, long qi, long qo) -> void {
        QrackWrapper::CLAND(sid, ci, qi, qo);
    }));
    function("clor", optional_override([](long sid, bool ci, long qi, long qo) -> void {
        QrackWrapper::CLOR(sid, ci, qi, qo);
    }));
    function("clxor", optional_override([](long sid, bool ci, long qi, long qo) -> void {
        QrackWrapper::CLXOR(sid, ci, qi, qo);
    }));
    function("clnand", optional_override([](long sid, bool ci, long qi, long qo) -> void {
        QrackWrapper::CLNAND(sid, ci, qi, qo);
    }));
    function("clnor", optional_override([](long sid, bool ci, long qi, long qo) -> void {
        QrackWrapper::CLNOR(sid, ci, qi, qo);
    }));
    function("clxnor", optional_override([](long sid, bool ci, long qi, long qo) -> void {
        QrackWrapper::CLXNOR(sid, ci, qi, qo);
    }));

    // Quantum Fourier Transform
    function("qft", optional_override([](long sid, std::vector<long> q) -> void {
        QrackWrapper::QFT(sid, q);
    }));
    function("iqft", optional_override([](long sid, std::vector<long> q) -> void {
        QrackWrapper::IQFT(sid, q);
    }));

    // Arithmetic logic unit
    function("add", optional_override([](long sid, std::vector<long> q, long a) -> void {
        QrackWrapper::ADD(sid, a, q);
    }));
    function("sub", optional_override([](long sid, std::vector<long> q, long a) -> void {
        QrackWrapper::SUB(sid, a, q);
    }));
    function("adds", optional_override([](long sid, std::vector<long> q, long a, long s) -> void {
        QrackWrapper::ADD(sid, a, q);
    }));
    function("subs", optional_override([](long sid, std::vector<long> q, long a, long s) -> void {
        QrackWrapper::SUB(sid, a, q);
    }));
    function("mcadd", optional_override([](long sid, std::vector<long> c, std::vector<long> q, long a) -> void {
        QrackWrapper::MCADD(sid, a, c, q);
    }));
    function("mcsub", optional_override([](long sid, std::vector<long> c, std::vector<long> q, long a) -> void {
        QrackWrapper::MCSUB(sid, a, c, q);
    }));
    function("mul", optional_override([](long sid, std::vector<long> q, std::vector<long> o, long a) -> void {
        QrackWrapper::MUL(sid, a, q, o);
    }));
    function("div", optional_override([](long sid, std::vector<long> q, std::vector<long> o, long a) -> void {
        QrackWrapper::DIV(sid, a, q, o);
    }));
    function("muln", optional_override([](long sid, std::vector<long> q, std::vector<long> o, long a, long m) -> void {
        QrackWrapper::MULN(sid, a, m, q, o);
    }));
    function("divn", optional_override([](long sid, std::vector<long> q, std::vector<long> o, long a, long m) -> void {
        QrackWrapper::DIVN(sid, a, m, q, o);
    }));
    function("pown", optional_override([](long sid, std::vector<long> q, std::vector<long> o, long a, long m) -> void {
        QrackWrapper::POWN(sid, a, m, q, o);
    }));
    function("mcmul", optional_override([](long sid, std::vector<long> c, std::vector<long> q, std::vector<long> o, long a) -> void {
        QrackWrapper::MCMUL(sid, a, c, q, o);
    }));
    function("mcdiv", optional_override([](long sid, std::vector<long> c, std::vector<long> q, std::vector<long> o, long a) -> void {
        QrackWrapper::MCDIV(sid, a, c, q, o);
    }));
    function("mcmuln", optional_override([](long sid, std::vector<long> c, std::vector<long> q, std::vector<long> o, long a, long m) -> void {
        QrackWrapper::MCMULN(sid, a, c, m, q, o);
    }));
    function("mcdivn", optional_override([](long sid, std::vector<long> c, std::vector<long> q, std::vector<long> o, long a, long m) -> void {
        QrackWrapper::MCDIVN(sid, a, c, m, q, o);
    }));
    function("mcpown", optional_override([](long sid, std::vector<long> c, std::vector<long> q, std::vector<long> o, long a, long m) -> void {
        QrackWrapper::MCPOWN(sid, a, c, m, q, o);
    }));

    function("init_qneuron", optional_override([](long sid, std::vector<long> c, long q, char f, double a, double tol) -> long {
        return QrackWrapper::init_qneuron(sid, c, q, f, a, tol);
    }));
    function("clone_qneuron", optional_override([](long nid) -> long {
        return QrackWrapper::clone_qneuron(nid);
    }));
    function("destroy_qneuron", optional_override([](long nid) -> void {
        QrackWrapper::destroy_qneuron(nid);
    }));
    function("set_qneuron_angles", optional_override([](long nid, std::vector<double> angles) -> void {
        QrackWrapper::set_qneuron_angles(nid, angles);
    }));
    function("get_qneuron_angles", optional_override([](long nid) -> std::vector<double> {
        return QrackWrapper::get_qneuron_angles(nid);
    }));
    function("set_qneuron_alpha", optional_override([](long nid, double alpha) -> void {
        QrackWrapper::set_qneuron_alpha(nid, alpha);
    }));
    function("get_qneuron_alpha", optional_override([](long nid) -> double {
        return QrackWrapper::get_qneuron_alpha(nid);
    }));
    function("set_qneuron_activation_fn", optional_override([](long nid, char f) -> void {
        QrackWrapper::set_qneuron_activation_fn(nid, f);
    }));
    function("get_qneuron_activation_fn", optional_override([](long nid) -> char {
        return QrackWrapper::get_qneuron_activation_fn(nid);
    }));
    function("qneuron_predict", optional_override([](long nid, bool e, bool r) -> double {
        return QrackWrapper::qneuron_predict(nid, e, r);
    }));
    function("qneuron_unpredict", optional_override([](long nid, bool e) -> double {
        return QrackWrapper::qneuron_unpredict(nid, e);
    }));
    function("qneuron_learn_cycle", optional_override([](long nid, bool e) -> double {
        return QrackWrapper::qneuron_learn_cycle(nid, e);
    }));
    function("qneuron_learn", optional_override([](long nid, double eta, bool e, bool r) -> void {
        QrackWrapper::qneuron_learn(nid, eta, e, r);
    }));
    function("qneuron_learn_permutation", optional_override([](long nid, double eta, bool e, bool r) -> void {
        QrackWrapper::qneuron_learn_permutation(nid, eta, e, r);
    }));
}
#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    emscripten::register_vector<long long>("VectorLong");
    emscripten::register_vector<double>("VectorDouble");
    emscripten::register_vector<char>("VectorChar");

    // Utility
    function("get_error", optional_override([](long long sid) -> long long {
        return QrackWrapper::get_error(sid);
    }));
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

    // SPAM and non-unitary
    function("reset_all", optional_override([](long long sid) -> void {
        QrackWrapper::ResetAll(sid);
    }));
    function("allocate_qubit", optional_override([](long long sid, long long q) -> void {
        QrackWrapper::allocateQubit(sid, q);
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
}

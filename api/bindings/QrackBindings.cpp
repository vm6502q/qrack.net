#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    emscripten::register_vector<long long>("VectorLong");
    emscripten::register_vector<float>("VectorFloat");
    emscripten::register_vector<char>("VectorChar");

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
    function("destroy", optional_override([](long long sid) -> void {
        QrackWrapper::destroy(sid);
    }));
    function("seed", optional_override([](long long sid, long long s) -> void {
        QrackWrapper::seed(sid, s);
    }));
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
    function("reset_all", optional_override([](long long sid) -> void {
        QrackWrapper::ResetAll(sid);
    }));
    function("allocate_qubit", optional_override([](long long sid, long long qid) -> void {
        QrackWrapper::allocateQubit(sid, qid);
    }));
}

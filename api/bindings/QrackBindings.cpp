#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    function("get_error", optional_override([](int sid) -> int {
        return QrackWrapper::get_error(sid);
    }));
    function("init_general", optional_override([](int length) -> int {
        return QrackWrapper::init_general(length);
    }));
    function("init_stabilizer", optional_override([](int length) -> int {
        return QrackWrapper::init_stabilizer(length);
    }));
    function("init_qbdd", optional_override([](int length) -> int {
        return QrackWrapper::init_qbdd(length);
    }));
    function("init_clone", optional_override([](int sid) -> int {
        return QrackWrapper::init_clone(sid);
    }));
    function("destroy", optional_override([](int sid) -> void {
        QrackWrapper::destroy(sid);
    }));
    function("seed", optional_override([](int sid, int s) -> void {
        QrackWrapper::seed(sid, s);
    }));
    function("prob", optional_override([](int sid, int q) -> void {
        QrackWrapper::Prob(sid, q);
    }));
    function("prob_rdm", optional_override([](int sid, int q) -> void {
        QrackWrapper::ProbRdm(sid, q);
    }));
    function("reset_all", optional_override([](int sid) -> void {
        QrackWrapper::ResetAll(sid);
    }));
}

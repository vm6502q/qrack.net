#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    function("qft_perm", optional_override([](int length, int perm) -> int {
        return QrackWrapper::qft_perm(length, perm);
    }));
}

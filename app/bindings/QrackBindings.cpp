#include <emscripten.h>
#include <emscripten/bind.h>
#include "QrackWrapper.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(QrackWrapper) {
    function("qft_u3", optional_override([](int length) -> int {
        return QrackWrapper::qft_u3(length);
    }));
}

#include <emscripten.h>
#include <emscripten/bind.h>
#include "Sample.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(Sample) {
    function("qft", optional_override([](int length) -> int {
        return Sample::qft(length);
    }));
}

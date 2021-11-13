#include "Sample.h"

#include "qrack/qfactory.hpp"

using namespace Qrack;

int Sample::qft(int length) {
    QInterfacePtr qftReg = CreateQuantumInterface(QINTERFACE_OPTIMAL, length, 0U);

    real1_f theta, phi, lambda;
    for (bitLenInt i = 0U; i < length; i++) {
        theta = 4 * PI_R1 * qftReg->Rand();
        phi = 2 * PI_R1 * qftReg->Rand();
        lambda = 2 * PI_R1 * qftReg->Rand();

        qftReg->U(i, theta, phi, lambda);
    }

    qftReg->QFT(0U, length);

    return (int)qftReg->MAll();
}

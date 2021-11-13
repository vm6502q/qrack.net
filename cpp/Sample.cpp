#include "Sample.h"

#include "qrack/qfactory.hpp"

Qrack::QInterfacePtr MakeRandQubit()
{
    Qrack::QInterfacePtr qubit = Qrack::CreateQuantumInterface(Qrack::QINTERFACE_OPTIMAL_MULTI, 1U, 0U);

    Qrack::real1 theta = 4 * M_PI * qubit->Rand();
    Qrack::real1 phi = 2 * M_PI * qubit->Rand();
    Qrack::real1 lambda = 2 * M_PI * qubit->Rand();

    qubit->U(0U, theta, phi, lambda);

    return qubit;
}

int Sample::qft(int length) {
    Qrack::QInterfacePtr qftReg = MakeRandQubit();

    for (bitLenInt i = 1U; i < length; i++) {
        qftReg->Compose(MakeRandQubit());
    }

    qftReg->QFT(0U, length, false);
    return (int)qftReg->MAll();
}

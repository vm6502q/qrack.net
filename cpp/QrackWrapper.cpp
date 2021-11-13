#include "QrackWrapper.h"

#include "qrack/qfactory.hpp"

int QrackWrapper::qft_perm(int length, int perm) {
    Qrack::QInterfacePtr qReg = Qrack::CreateQuantumInterface(Qrack::QINTERFACE_OPTIMAL, length, perm);
    qReg->QFT(0, length);
    
    return (int)qReg->MAll();
}

#include "QrackWrapper.h"

#include "qrack/qfactory.hpp"

int QrackWrapper::qft_perm(int length, int perm) {
    Qrack::QInterfacePtr qReg = Qrack::CreateQuantumInterface(Qrack::QINTERFACE_OPTIMAL, length, perm);
    qReg->QFT(0, length);
    
    return (int)qReg->MAll();
}

int QrackWrapper::qft_u3(int length, int perm) {
    Qrack::QInterfacePtr qReg = Qrack::CreateQuantumInterface(Qrack::QINTERFACE_OPTIMAL, length, perm);
    
    Qrack::real1_f th, ph, lm;
    for (bitLenInt i = 0U; i < length; i++) {
        th = 4 * (Qrack::real1_f)M_PI * qReg->Rand();
        ph = 2 * (Qrack::real1_f)M_PI * qReg->Rand();
        lm = 2 * (Qrack::real1_f)M_PI * qReg->Rand();
        
        qReg->U(i, th, ph, lm);
    }
    
    qReg->QFT(0, length);
    
    return (int)qReg->MAll();
}

#include "QrackWrapper.h"

#include "qrack/qfactory.hpp"

int QrackWrapper::qft_u3(int length) {
    Qrack::QInterfacePtr qReg = Qrack::CreateQuantumInterface({ Qrack::QINTERFACE_TENSOR_NETWORK, Qrack::QINTERFACE_QUNIT, Qrack::QINTERFACE_CPU }, (bitLenInt)length, Qrack::ZERO_BCI);
    
    Qrack::real1_f th, ph, lm;
    for (bitLenInt i = 0U; i < length; i++) {
        th = 4 * (Qrack::real1_f)M_PI * qReg->Rand();
        ph = 2 * (Qrack::real1_f)M_PI * qReg->Rand();
        lm = 2 * (Qrack::real1_f)M_PI * qReg->Rand();
        
        qReg->U(i, th, ph, lm);
    }
    
    qReg->QFT(0, length);
    
    return (int)(uint32_t)qReg->MAll();
}

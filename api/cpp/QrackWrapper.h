#include <vector>

class QrackWrapper {
public:
    // Utility
    static long long init_general(long long length);
    static long long init_stabilizer(long long length);
    static long long init_qbdd(long long length);
    static long long init_clone(long long sid);
    static long long num_qubits(long long sid);
    static void destroy(long long sid);
    static void seed(long long sid, long long s);
    static void allocateQubit(long long sid, long long qid);
    static bool release(long long sid, long long q);

    // Expectation value output
    static double Prob(long long sid, long long q);
    static double ProbRdm(long long sid, long long q);
    static double PermutationProb(long long sid, std::vector<long long> q, std::vector<char> s);
    static double PermutationProbRdm(long long sid, std::vector<long long> q, std::vector<char> s, bool r);
    static double FactorizedExpectation(long long sid, std::vector<long long> q, std::vector<long long> s);
    static double FactorizedExpectationRdm(long long sid, std::vector<long long> q, std::vector<long long> s, bool r);
    static double FactorizedExpectationFp(long long sid, std::vector<long long> q, std::vector<double> s);
    static double FactorizedExpectationFpRdm(long long sid, std::vector<long long> q, std::vector<double> s, bool r);

    // Parity
    static void PhaseParity(long long sid, double lambda, std::vector<long long> q);
    static double JointEnsembleProbability(long long sid, std::vector<long long> q, std::vector<char> b);

    // Schmidt decomposition
    static void Compose(long long sid1, long long sid2, std::vector<long long> q);
    static long long Decompose(long long sid, std::vector<long long> q);
    static void Dispose(long long sid, std::vector<long long> q);

    // SPAM and non-unitary
    static bool M(long long sid, long long q);
    static bool ForceM(long long sid, long long q, bool r);
    static bool Measure(long long sid, std::vector<long long> q, std::vector<char> b);
    static unsigned long long MAll(long long sid);
    static std::vector<unsigned long long> MeasureShots(long long sid, std::vector<long long> q, unsigned s);
    static void ResetAll(long long sid);

    // single-qubit gates
    static void X(long long sid, long long q);
    static void Y(long long sid, long long q);
    static void Z(long long sid, long long q);
    static void H(long long sid, long long q);
    static void S(long long sid, long long q);
    static void T(long long sid, long long q);
    static void AdjS(long long sid, long long q);
    static void AdjT(long long sid, long long q);
    static void U(long long sid, long long q, double theta, double phi, double lambda);
    static void Mtrx(long long sid, std::vector<double> m, long long q);

    // multi-controlled single-qubit gates
    static void MCX(long long sid, std::vector<long long> c, long long q);
    static void MCY(long long sid, std::vector<long long> c, long long q);
    static void MCZ(long long sid, std::vector<long long> c, long long q);
    static void MCH(long long sid, std::vector<long long> c, long long q);
    static void MCS(long long sid, std::vector<long long> c, long long q);
    static void MCT(long long sid, std::vector<long long> c, long long q);
    static void MCAdjS(long long sid, std::vector<long long> c, long long q);
    static void MCAdjT(long long sid, std::vector<long long> c, long long q);
    static void MCU(long long sid, std::vector<long long> c, long long q, double theta, double phi, double lambda);
    static void MCMtrx(long long sid, std::vector<long long> c, std::vector<double> m, long long q);
    static void MACX(long long sid, std::vector<long long> c, long long q);
    static void MACY(long long sid, std::vector<long long> c, long long q);
    static void MACZ(long long sid, std::vector<long long> c, long long q);
    static void MACH(long long sid, std::vector<long long> c, long long q);
    static void MACS(long long sid, std::vector<long long> c, long long q);
    static void MACT(long long sid, std::vector<long long> c, long long q);
    static void MACAdjS(long long sid, std::vector<long long> c, long long q);
    static void MACAdjT(long long sid, std::vector<long long> c, long long q);
    static void MACU(long long sid, std::vector<long long> c, long long q, double theta, double phi, double lambda);
    static void MACMtrx(long long sid, std::vector<long long> c, std::vector<double> m, long long q);
    static void UCMtrx(long long sid, std::vector<long long> c, std::vector<double> m, long long q, long long p);
    static void Multiplex1Mtrx(long long sid, std::vector<long long> c, long long q, std::vector<double> m);

    // coalesced single-qubit gates
    static void MX(long long sid, std::vector<long long> q);
    static void MY(long long sid, std::vector<long long> q);
    static void MZ(long long sid, std::vector<long long> q);

    // single-qubit rotations
    static void R(long long sid, double phi, long long q, char b);
    // multi-controlled single-qubit rotations
    static void MCR(long long sid, double phi, std::vector<long long> c, long long q, char b);

    // exponential of Pauli operators
    static void Exp(long long sid, double phi, std::vector<long long> q, std::vector<char> b);
    // multi-controlled exponential of Pauli operators
    static void MCExp(long long sid, double phi, std::vector<long long> c, std::vector<long long> q, std::vector<char> b);

    // swap variants
    static void SWAP(long long sid, long long qi1, long long qi2);
    static void ISWAP(long long sid, long long qi1, long long qi2);
    static void AdjISWAP(long long sid, long long qi1, long long qi2);
    static void FSim(long long sid, double theta, double phi, long long qi1, long long qi2);
    static void CSWAP(long long sid, std::vector<long long> c, long long qi1, long long qi2);
    static void ACSWAP(long long sid, std::vector<long long> c, long long qi1, long long qi2);

    // Quantum boolean (Toffoli) operations
    static void AND(long long sid, long long qi1, long long qi2, long long qo);
    static void OR(long long sid, long long qi1, long long qi2, long long qo);
    static void XOR(long long sid, long long qi1, long long qi2, long long qo);
    static void NAND(long long sid, long long qi1, long long qi2, long long qo);
    static void NOR(long long sid, long long qi1, long long qi2, long long qo);
    static void XNOR(long long sid, long long qi1, long long qi2, long long qo);
    static void CLAND(long long sid, bool ci, long long qi, long long qo);
    static void CLOR(long long sid, bool ci, long long qi, long long qo);
    static void CLXOR(long long sid, bool ci, long long qi, long long qo);
    static void CLNAND(long long sid, bool ci, long long qi, long long qo);
    static void CLNOR(long long sid, bool ci, long long qi, long long qo);
    static void CLXNOR(long long sid, bool ci, long long qi, long long qo);

    // Quantum Fourier Transform
    static void QFT(long long sid, std::vector<long long> q);
    static void IQFT(long long sid, std::vector<long long> q);

    // Arithmetic logic unit
    static void ADD(long long sid, unsigned long long a, std::vector<long long> q);
    static void SUB(long long sid, unsigned long long a, std::vector<long long> q);
    static void ADDS(long long sid, unsigned long long a, long long s, std::vector<long long> q);
    static void SUBS(long long sid, unsigned long long a, long long s, std::vector<long long> q);
    static void MCADD(long long sid, unsigned long long a, std::vector<long long> c, std::vector<long long> q);
    static void MCSUB(long long sid, unsigned long long a, std::vector<long long> c, std::vector<long long> q);
    static void MUL(long long sid, unsigned long long a, std::vector<long long> q, std::vector<long long> o);
    static void DIV(long long sid, unsigned long long a, std::vector<long long> q, std::vector<long long> o);
    static void MULN(long long sid, unsigned long long a, unsigned long long m, std::vector<long long> q, std::vector<long long> o);
    static void DIVN(long long sid, unsigned long long a, unsigned long long m, std::vector<long long> q, std::vector<long long> o);
    static void POWN(long long sid, unsigned long long a, unsigned long long m, std::vector<long long> q, std::vector<long long> o);
    static void MCMUL(long long sid, unsigned long long a, std::vector<long long> c, std::vector<long long> q, std::vector<long long> o);
    static void MCDIV(long long sid, unsigned long long a, std::vector<long long> c, std::vector<long long> q, std::vector<long long> o);
    static void MCMULN(long long sid, unsigned long long a, std::vector<long long> c, unsigned long long m, std::vector<long long> q, std::vector<long long> o);
    static void MCDIVN(long long sid, unsigned long long a, std::vector<long long> c, unsigned long long m, std::vector<long long> q, std::vector<long long> o);
    static void MCPOWN(long long sid, unsigned long long a, std::vector<long long> c, unsigned long long m, std::vector<long long> q, std::vector<long long> o);
};

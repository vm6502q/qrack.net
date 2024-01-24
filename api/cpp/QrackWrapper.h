#include <vector>

class QrackWrapper {
public:
    // Utility
    static long init_general(long length);
    static long init_stabilizer(long length);
    static long init_qbdd(long length);
    static long init_clone(long sid);
    static long num_qubits(long sid);
    static void destroy(long sid);
    static void seed(long sid, long s);
    static void allocateQubit(long sid, long qid);
    static bool release(long sid, long q);
    static bool TrySeparate1Qb(long sid, long qi1);
    static bool TrySeparate2Qb(long sid, long qi1, long qi2);
    static bool TrySeparateTol(long sid, std::vector<long> q, double tol);
    static double GetUnitaryFidelity(long sid);
    static void ResetUnitaryFidelity(long sid);
    static void SetSdrp(long sid, double sdrp);
    static void SetReactiveSeparate(long sid, bool irs);
    static void SetTInjection(long sid, bool iti);

    // Expectation value output
    static double Prob(long sid, long q);
    static double ProbRdm(long sid, long q);
    static double PermutationProb(long sid, std::vector<long> q, std::vector<char> s);
    static double PermutationProbRdm(long sid, std::vector<long> q, std::vector<char> s, bool r);
    static double FactorizedExpectation(long sid, std::vector<long> q, std::vector<long> s);
    static double FactorizedExpectationRdm(long sid, std::vector<long> q, std::vector<long> s, bool r);
    static double FactorizedExpectationFp(long sid, std::vector<long> q, std::vector<double> s);
    static double FactorizedExpectationFpRdm(long sid, std::vector<long> q, std::vector<double> s, bool r);

    // Parity
    static void PhaseParity(long sid, double lambda, std::vector<long> q);
    static double JointEnsembleProbability(long sid, std::vector<long> q, std::vector<char> b);

    // Schmidt decomposition
    static void Compose(long sid1, long sid2, std::vector<long> q);
    static long Decompose(long sid, std::vector<long> q);
    static void Dispose(long sid, std::vector<long> q);

    // SPAM and non-unitary
    static bool M(long sid, long q);
    static bool ForceM(long sid, long q, bool r);
    static bool Measure(long sid, std::vector<long> q, std::vector<char> b);
    static long MAll(long sid);
    static std::vector<long> MeasureShots(long sid, std::vector<long> q, long s);
    static void ResetAll(long sid);

    // single-qubit gates
    static void X(long sid, long q);
    static void Y(long sid, long q);
    static void Z(long sid, long q);
    static void H(long sid, long q);
    static void S(long sid, long q);
    static void T(long sid, long q);
    static void AdjS(long sid, long q);
    static void AdjT(long sid, long q);
    static void U(long sid, long q, double theta, double phi, double lambda);
    static void Mtrx(long sid, std::vector<double> m, long q);

    // multi-controlled single-qubit gates
    static void MCX(long sid, std::vector<long> c, long q);
    static void MCY(long sid, std::vector<long> c, long q);
    static void MCZ(long sid, std::vector<long> c, long q);
    static void MCH(long sid, std::vector<long> c, long q);
    static void MCS(long sid, std::vector<long> c, long q);
    static void MCT(long sid, std::vector<long> c, long q);
    static void MCAdjS(long sid, std::vector<long> c, long q);
    static void MCAdjT(long sid, std::vector<long> c, long q);
    static void MCU(long sid, std::vector<long> c, long q, double theta, double phi, double lambda);
    static void MCMtrx(long sid, std::vector<long> c, std::vector<double> m, long q);
    static void MACX(long sid, std::vector<long> c, long q);
    static void MACY(long sid, std::vector<long> c, long q);
    static void MACZ(long sid, std::vector<long> c, long q);
    static void MACH(long sid, std::vector<long> c, long q);
    static void MACS(long sid, std::vector<long> c, long q);
    static void MACT(long sid, std::vector<long> c, long q);
    static void MACAdjS(long sid, std::vector<long> c, long q);
    static void MACAdjT(long sid, std::vector<long> c, long q);
    static void MACU(long sid, std::vector<long> c, long q, double theta, double phi, double lambda);
    static void MACMtrx(long sid, std::vector<long> c, std::vector<double> m, long q);
    static void UCMtrx(long sid, std::vector<long> c, std::vector<double> m, long q, long p);
    static void Multiplex1Mtrx(long sid, std::vector<long> c, long q, std::vector<double> m);

    // coalesced single-qubit gates
    static void MX(long sid, std::vector<long> q);
    static void MY(long sid, std::vector<long> q);
    static void MZ(long sid, std::vector<long> q);

    // single-qubit rotations
    static void R(long sid, double phi, long q, char b);
    // multi-controlled single-qubit rotations
    static void MCR(long sid, double phi, std::vector<long> c, long q, char b);

    // exponential of Pauli operators
    static void Exp(long sid, double phi, std::vector<long> q, std::vector<char> b);
    // multi-controlled exponential of Pauli operators
    static void MCExp(long sid, double phi, std::vector<long> c, std::vector<long> q, std::vector<char> b);

    // swap variants
    static void SWAP(long sid, long qi1, long qi2);
    static void ISWAP(long sid, long qi1, long qi2);
    static void AdjISWAP(long sid, long qi1, long qi2);
    static void FSim(long sid, double theta, double phi, long qi1, long qi2);
    static void CSWAP(long sid, std::vector<long> c, long qi1, long qi2);
    static void ACSWAP(long sid, std::vector<long> c, long qi1, long qi2);

    // Quantum boolean (Toffoli) operations
    static void AND(long sid, long qi1, long qi2, long qo);
    static void OR(long sid, long qi1, long qi2, long qo);
    static void XOR(long sid, long qi1, long qi2, long qo);
    static void NAND(long sid, long qi1, long qi2, long qo);
    static void NOR(long sid, long qi1, long qi2, long qo);
    static void XNOR(long sid, long qi1, long qi2, long qo);
    static void CLAND(long sid, bool ci, long qi, long qo);
    static void CLOR(long sid, bool ci, long qi, long qo);
    static void CLXOR(long sid, bool ci, long qi, long qo);
    static void CLNAND(long sid, bool ci, long qi, long qo);
    static void CLNOR(long sid, bool ci, long qi, long qo);
    static void CLXNOR(long sid, bool ci, long qi, long qo);

    // Quantum Fourier Transform
    static void QFT(long sid, std::vector<long> q);
    static void IQFT(long sid, std::vector<long> q);

    // Arithmetic logic unit
    static void ADD(long sid, long a, std::vector<long> q);
    static void SUB(long sid, long a, std::vector<long> q);
    static void ADDS(long sid, long a, long s, std::vector<long> q);
    static void SUBS(long sid, long a, long s, std::vector<long> q);
    static void MCADD(long sid, long a, std::vector<long> c, std::vector<long> q);
    static void MCSUB(long sid, long a, std::vector<long> c, std::vector<long> q);
    static void MUL(long sid, long a, std::vector<long> q, std::vector<long> o);
    static void DIV(long sid, long a, std::vector<long> q, std::vector<long> o);
    static void MULN(long sid, long a, long m, std::vector<long> q, std::vector<long> o);
    static void DIVN(long sid, long a, long m, std::vector<long> q, std::vector<long> o);
    static void POWN(long sid, long a, long m, std::vector<long> q, std::vector<long> o);
    static void MCMUL(long sid, long a, std::vector<long> c, std::vector<long> q, std::vector<long> o);
    static void MCDIV(long sid, long a, std::vector<long> c, std::vector<long> q, std::vector<long> o);
    static void MCMULN(long sid, long a, std::vector<long> c, long m, std::vector<long> q, std::vector<long> o);
    static void MCDIVN(long sid, long a, std::vector<long> c, long m, std::vector<long> q, std::vector<long> o);
    static void MCPOWN(long sid, long a, std::vector<long> c, long m, std::vector<long> q, std::vector<long> o);

    // (Single-target-multiplexer-based) quantum neurons
    static long init_qneuron(long sid, std::vector<long> c, long q, char f, double a, double tol);
    static long clone_qneuron(long nid);
    static void destroy_qneuron(long nid);
    static void set_qneuron_angles(long nid, std::vector<double> angles);
    static std::vector<double> get_qneuron_angles(long nid);
    static void set_qneuron_alpha(long nid, double alpha);
    static double get_qneuron_alpha(long nid);
    static void set_qneuron_activation_fn(long nid, char f);
    static char get_qneuron_activation_fn(long nid);
    static double qneuron_predict(long nid, bool e, bool r);
    static double qneuron_unpredict(long nid, bool e);
    static double qneuron_learn_cycle(long nid, bool e);
    static void qneuron_learn(long nid, double eta, bool e, bool r);
    static void qneuron_learn_permutation(long nid, double eta, bool e, bool r);
};

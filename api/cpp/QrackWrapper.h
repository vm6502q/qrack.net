#include <vector>

class QrackWrapper {
public:
    // Utility
    static int init_general(int length);
    static int init_stabilizer(int length);
    static int init_qbdd(int length);
    static int init_clone(int sid);
    static int num_qubits(int sid);
    static void destroy(int sid);
    static void seed(int sid, int s);
    static void allocateQubit(int sid, int qid);
    static bool release(int sid, int q);
    static bool TrySeparate1Qb(int sid, int qi1);
    static bool TrySeparate2Qb(int sid, int qi1, int qi2);
    static bool TrySeparateTol(int sid, std::vector<int> q, double tol);
    static double GetUnitaryFidelity(int sid);
    static void ResetUnitaryFidelity(int sid);
    static void SetSdrp(int sid, double sdrp);
    static void SetReactiveSeparate(int sid, bool irs);
    static void SetTInjection(int sid, bool iti);

    // Expectation value output
    static double Prob(int sid, int q);
    static double ProbRdm(int sid, int q);
    static double PermutationProb(int sid, std::vector<int> q, std::vector<char> s);
    static double PermutationProbRdm(int sid, std::vector<int> q, std::vector<char> s, bool r);
    static double FactorizedExpectation(int sid, std::vector<int> q, std::vector<int> s);
    static double FactorizedExpectationRdm(int sid, std::vector<int> q, std::vector<int> s, bool r);
    static double FactorizedExpectationFp(int sid, std::vector<int> q, std::vector<double> s);
    static double FactorizedExpectationFpRdm(int sid, std::vector<int> q, std::vector<double> s, bool r);

    // Parity
    static void PhaseParity(int sid, double lambda, std::vector<int> q);
    static double JointEnsembleProbability(int sid, std::vector<int> q, std::vector<char> b);

    // Schmidt decomposition
    static void Compose(int sid1, int sid2, std::vector<int> q);
    static int Decompose(int sid, std::vector<int> q);
    static void Dispose(int sid, std::vector<int> q);

    // SPAM and non-unitary
    static bool M(int sid, int q);
    static bool ForceM(int sid, int q, bool r);
    static bool Measure(int sid, std::vector<int> q, std::vector<char> b);
    static unsigned int MAll(int sid);
    static std::vector<unsigned int> MeasureShots(int sid, std::vector<int> q, unsigned s);
    static void ResetAll(int sid);

    // single-qubit gates
    static void X(int sid, int q);
    static void Y(int sid, int q);
    static void Z(int sid, int q);
    static void H(int sid, int q);
    static void S(int sid, int q);
    static void T(int sid, int q);
    static void AdjS(int sid, int q);
    static void AdjT(int sid, int q);
    static void U(int sid, int q, double theta, double phi, double lambda);
    static void Mtrx(int sid, std::vector<double> m, int q);

    // multi-controlled single-qubit gates
    static void MCX(int sid, std::vector<int> c, int q);
    static void MCY(int sid, std::vector<int> c, int q);
    static void MCZ(int sid, std::vector<int> c, int q);
    static void MCH(int sid, std::vector<int> c, int q);
    static void MCS(int sid, std::vector<int> c, int q);
    static void MCT(int sid, std::vector<int> c, int q);
    static void MCAdjS(int sid, std::vector<int> c, int q);
    static void MCAdjT(int sid, std::vector<int> c, int q);
    static void MCU(int sid, std::vector<int> c, int q, double theta, double phi, double lambda);
    static void MCMtrx(int sid, std::vector<int> c, std::vector<double> m, int q);
    static void MACX(int sid, std::vector<int> c, int q);
    static void MACY(int sid, std::vector<int> c, int q);
    static void MACZ(int sid, std::vector<int> c, int q);
    static void MACH(int sid, std::vector<int> c, int q);
    static void MACS(int sid, std::vector<int> c, int q);
    static void MACT(int sid, std::vector<int> c, int q);
    static void MACAdjS(int sid, std::vector<int> c, int q);
    static void MACAdjT(int sid, std::vector<int> c, int q);
    static void MACU(int sid, std::vector<int> c, int q, double theta, double phi, double lambda);
    static void MACMtrx(int sid, std::vector<int> c, std::vector<double> m, int q);
    static void UCMtrx(int sid, std::vector<int> c, std::vector<double> m, int q, int p);
    static void Multiplex1Mtrx(int sid, std::vector<int> c, int q, std::vector<double> m);

    // coalesced single-qubit gates
    static void MX(int sid, std::vector<int> q);
    static void MY(int sid, std::vector<int> q);
    static void MZ(int sid, std::vector<int> q);

    // single-qubit rotations
    static void R(int sid, double phi, int q, char b);
    // multi-controlled single-qubit rotations
    static void MCR(int sid, double phi, std::vector<int> c, int q, char b);

    // exponential of Pauli operators
    static void Exp(int sid, double phi, std::vector<int> q, std::vector<char> b);
    // multi-controlled exponential of Pauli operators
    static void MCExp(int sid, double phi, std::vector<int> c, std::vector<int> q, std::vector<char> b);

    // swap variants
    static void SWAP(int sid, int qi1, int qi2);
    static void ISWAP(int sid, int qi1, int qi2);
    static void AdjISWAP(int sid, int qi1, int qi2);
    static void FSim(int sid, double theta, double phi, int qi1, int qi2);
    static void CSWAP(int sid, std::vector<int> c, int qi1, int qi2);
    static void ACSWAP(int sid, std::vector<int> c, int qi1, int qi2);

    // Quantum boolean (Toffoli) operations
    static void AND(int sid, int qi1, int qi2, int qo);
    static void OR(int sid, int qi1, int qi2, int qo);
    static void XOR(int sid, int qi1, int qi2, int qo);
    static void NAND(int sid, int qi1, int qi2, int qo);
    static void NOR(int sid, int qi1, int qi2, int qo);
    static void XNOR(int sid, int qi1, int qi2, int qo);
    static void CLAND(int sid, bool ci, int qi, int qo);
    static void CLOR(int sid, bool ci, int qi, int qo);
    static void CLXOR(int sid, bool ci, int qi, int qo);
    static void CLNAND(int sid, bool ci, int qi, int qo);
    static void CLNOR(int sid, bool ci, int qi, int qo);
    static void CLXNOR(int sid, bool ci, int qi, int qo);

    // Quantum Fourier Transform
    static void QFT(int sid, std::vector<int> q);
    static void IQFT(int sid, std::vector<int> q);

    // Arithmetic logic unit
    static void ADD(int sid, unsigned int a, std::vector<int> q);
    static void SUB(int sid, unsigned int a, std::vector<int> q);
    static void ADDS(int sid, unsigned int a, int s, std::vector<int> q);
    static void SUBS(int sid, unsigned int a, int s, std::vector<int> q);
    static void MCADD(int sid, unsigned int a, std::vector<int> c, std::vector<int> q);
    static void MCSUB(int sid, unsigned int a, std::vector<int> c, std::vector<int> q);
    static void MUL(int sid, unsigned int a, std::vector<int> q, std::vector<int> o);
    static void DIV(int sid, unsigned int a, std::vector<int> q, std::vector<int> o);
    static void MULN(int sid, unsigned int a, unsigned int m, std::vector<int> q, std::vector<int> o);
    static void DIVN(int sid, unsigned int a, unsigned int m, std::vector<int> q, std::vector<int> o);
    static void POWN(int sid, unsigned int a, unsigned int m, std::vector<int> q, std::vector<int> o);
    static void MCMUL(int sid, unsigned int a, std::vector<int> c, std::vector<int> q, std::vector<int> o);
    static void MCDIV(int sid, unsigned int a, std::vector<int> c, std::vector<int> q, std::vector<int> o);
    static void MCMULN(int sid, unsigned int a, std::vector<int> c, unsigned int m, std::vector<int> q, std::vector<int> o);
    static void MCDIVN(int sid, unsigned int a, std::vector<int> c, unsigned int m, std::vector<int> q, std::vector<int> o);
    static void MCPOWN(int sid, unsigned int a, std::vector<int> c, unsigned int m, std::vector<int> q, std::vector<int> o);

    // (Single-target-multiplexer-based) quantum neurons
    static int init_qneuron(int sid, std::vector<int> c, int q, char f, double a, double tol);
    static int clone_qneuron(int nid);
    static void destroy_qneuron(int nid);
    static void set_qneuron_angles(int nid, std::vector<double> angles);
    static std::vector<double> get_qneuron_angles(int nid);
    static void set_qneuron_alpha(int nid, double alpha);
    static double get_qneuron_alpha(int nid);
    static void set_qneuron_activation_fn(int nid, char f);
    static char get_qneuron_activation_fn(int nid);
    static double qneuron_predict(int nid, bool e, bool r);
    static double qneuron_unpredict(int nid, bool e);
    static double qneuron_learn_cycle(int nid, bool e);
    static void qneuron_learn(int nid, double eta, bool e, bool r);
    static void qneuron_learn_permutation(int nid, double eta, bool e, bool r);
};

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
};

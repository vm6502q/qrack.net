#include <vector>

class QrackWrapper {
public:
    // Utility
    static long long get_error(long long sid);
    static long long init_general(long long length);
    static long long init_stabilizer(long long length);
    static long long init_qbdd(long long length);
    static long long init_clone(long long sid);
    static void destroy(long long sid);
    static void seed(long long sid, long long s);

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

    //SPAM and non-unitary
    static void ResetAll(long long sid);
    static void allocateQubit(long long sid, long long qid);
};

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

    //SPAM and non-unitary
    static void ResetAll(long long sid);
};

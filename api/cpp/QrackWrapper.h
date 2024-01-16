class QrackWrapper {
public:
    // Utility
    static int get_error(int sid);
    static int init_general(int length);
    static int init_stabilizer(int length);
    static int init_qbdd(int length);
    static int init_clone(int sid);
    static void destroy(int sid);
    static void seed(int sid, int s);

    // Expectation value output
    static double Prob(int sid, int q);
};

class QrackWrapper {
public:
    static int get_error(int sid);
    static int init_general(int length);
    static int init_stabilizer(int length);
    static int init_qbdd(int length);
};

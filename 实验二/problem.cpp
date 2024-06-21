#include <iostream>
using namespace std;
int main() {
    int A;  // A偷了手表
    int B;  // B偷了手表
    int C;  // 作案不在营业时间内
    int D;  // B提供的证据正确
    int E;  // 货柜未上锁
    // 则命题为：(A||B) && (!A||C) && (!D||E) && (D||!C) && !E
    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            for (C = 0; C < 2; C++)
                for (D = 0; D < 2; D++)
                    for (E = 0; E < 2; E++)
                        if ((A || B) && (!A || C) && (!D || E) && (D || !C) && !E)
                            printf("A=%d, B=%d", A, B);
    return 0;
}
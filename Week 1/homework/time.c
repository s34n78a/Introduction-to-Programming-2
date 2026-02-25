#include <stdio.h>

int main(void) {
    int h1, m1, s1, h2, m2, s2;
    scanf("%d %d %d", &h1, &m1, &s1);
    scanf("%d %d %d", &h2, &m2, &s2);

    int s_diff = s1-s2;
    if (s_diff < 0) {
        s_diff += 60;
        m1--;
    }

    int m_diff = m1 - m2;
    if (m_diff < 0) {
        m_diff += 60;
        h1--;
    }

    int h_diff = h1 - h2;
    if (h_diff < 0) {
        h_diff += 24;
    }

    if (h_diff < 10) {
        printf("0");
    }
    printf("%d ", h_diff);

    if (m_diff < 10) {
        printf("0");
    }
    printf("%d ", m_diff);

    if (s_diff < 10) {
        printf("0");
    }
    printf("%d", s_diff);

    return 0;
}
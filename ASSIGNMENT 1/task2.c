#include <stdio.h>

int main(void) {
    float marks[5] = {85, 78, 92, 66, 74};
    float total = 0.0f;
    int gradePoints[5];

    for (int i = 0; i < 5; i++) {
        switch ((int)marks[i] / 10) {
            case 10: case 9: gradePoints[i] = 10; break;
            case 8: gradePoints[i] = 9; break;
            case 7: gradePoints[i] = 8; break;
            case 6: gradePoints[i] = 7; break;
            case 5: gradePoints[i] = 6; break;
            case 4: gradePoints[i] = 5; break;
            default: gradePoints[i] = 0; break;
        }
        total += gradePoints[i];
    }

    printf("CGPA calculation for 5 subjects\n");
    for (int i = 0; i < 5; i++)
        printf("Subject %d: %.0f marks -> %d grade points\n", i + 1, marks[i], gradePoints[i]);
    printf("CGPA = %.2f\n", total / 5.0f);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Function to generate a single normally distributed random number
double generate_normal(double mean, double stddev) {
    // Generate two uniform random numbers between 0 and 1
    double u1 = ((double) rand()) / RAND_MAX;
    double u2 = ((double) rand()) / RAND_MAX;

    // Apply the Box-Muller transform
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    
    // Scale and shift the result to have the desired mean and standard deviation
    return z0 * stddev + mean;
}

int main() {
    int n, i;
    double mean, stddev;

    // Initialize the random number generator
    srand(time(NULL));

    // Input the number of elements, mean, and standard deviation
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    printf("Enter the mean: ");
    scanf("%lf", &mean);
    printf("Enter the standard deviation: ");
    scanf("%lf", &stddev);

    // Output the generated data
    printf("\nGenerated Normal Distribution Data:\n");
    for (i = 0; i < n; i++) {
        double value = generate_normal(mean, stddev);
        printf("%.5f ", value);
    }

    printf("\n");
    return 0;
}

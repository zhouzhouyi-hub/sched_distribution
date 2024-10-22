#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_NUMBERS 1000

// Function to calculate mean
double calculate_mean(double data[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

// Function to calculate median
double calculate_median(double data[], int n) {
    // Sort the array to find the median
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                double temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    if (n % 2 == 0)
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    else
        return data[n/2];
}

// Function to calculate variance
double calculate_variance(double data[], int n, double mean) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += pow(data[i] - mean, 2);
    }
    return sum / n;
}

// Function to calculate skewness
double calculate_skewness(double data[], int n, double mean, double std_dev) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += pow((data[i] - mean) / std_dev, 3);
    }
    return sum / n;
}

// Function to determine the type of distribution
void classify_distribution(double data[], int n) {
    double mean = calculate_mean(data, n);
    double median = calculate_median(data, n);
    double variance = calculate_variance(data, n, mean);
    double std_dev = sqrt(variance);
    double skewness = calculate_skewness(data, n, mean, std_dev);

    printf("\nStatistical Information:\n");
    printf("Mean: %.2f\n", mean);
    printf("Median: %.2f\n", median);
    printf("Variance: %.2f\n", variance);
    printf("Standard Deviation: %.2f\n", std_dev);
    printf("Skewness: %.2f\n", skewness);

    // Heuristic to classify the distribution
    if (fabs(mean - median) < 0.1 * mean && fabs(skewness) < 0.5) {
        printf("\nThe dataset is likely following a Normal distribution.\n");
    } else if (mean > median && skewness > 1.0) {
        printf("\nThe dataset is likely following a Log-Normal distribution.\n");
    } else if (skewness > 1.5 && mean > 1.5 * median) {
        printf("\nThe dataset is likely following a Pareto distribution.\n");
    } else if (mean > median && skewness > 0.5 && skewness < 1.5) {
        printf("\nThe dataset is likely following an Exponential distribution.\n");
    } else {
        printf("\nThe distribution type could not be determined with these heuristics.\n");
    }
}

int main() {
    int n;
    double data[MAX_NUMBERS];

    // Input the number of elements
    // printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Input the elements
    // printf("Enter the elements: ");
    for (int i = 0; i < n; i++) {
        scanf("%lf", &data[i]);
    }

    // Classify the distribution based on statistical properties
    classify_distribution(data, n);

    return 0;
}

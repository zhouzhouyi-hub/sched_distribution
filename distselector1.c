#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 1000

void input_data(double data[], int *size) {
    //printf("Enter the number of elements in the dataset (max %d): ", MAX_SIZE);
    scanf("%d", size);
    if (*size > MAX_SIZE) {
        printf("Size exceeds maximum limit of %d.\n", MAX_SIZE);
        exit(1);
    }

    //printf("Enter the elements (space-separated): ");
    for (int i = 0; i < *size; i++) {
        scanf("%lf", &data[i]);
    }
}

double mean(double data[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

double variance(double data[], int size, double mean_value) {
    double var = 0.0;
    for (int i = 0; i < size; i++) {
        var += (data[i] - mean_value) * (data[i] - mean_value);
    }
    return var / size;
}

double skewness(double data[], int size, double mean_value, double variance_value) {
    double skew = 0.0;
    for (int i = 0; i < size; i++) {
        skew += pow(data[i] - mean_value, 3);
    }
    return (skew / size) / pow(variance_value, 1.5);
}

int is_exponential_distribution(double data[], int size) {
    double mean_value = mean(data, size);
    return (mean_value > 0); // Simple check
}

int is_lognormal_distribution(double data[], int size) {
    for (int i = 0; i < size; i++) {
        if (data[i] <= 0) return 0; // Log-normal requires positive values
    }

    double log_data[MAX_SIZE];
    for (int i = 0; i < size; i++) {
        log_data[i] = log(data[i]);
    }

    double log_mean = mean(log_data, size);
    double log_variance = variance(log_data, size, log_mean);

    // Basic heuristic check for normality
    return (log_variance < 1e6 && fabs(log_mean) < 1e6); // Example conditions
}

int is_pareto_distribution(double data[], int size) {
    for (int i = 0; i < size; i++) {
        if (data[i] <= 0) return 0; // Pareto requires positive values
    }

    // Estimate the shape parameter (alpha) using MLE
    double min_val = data[0];
    for (int i = 1; i < size; i++) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }

    double alpha_estimate = size / (mean(data, size) - size * min_val);

    return (alpha_estimate > 1); // Simplified check
}

void analyze_data(double data[], int size) {
    double mean_value = mean(data, size);
    double variance_value = variance(data, size, mean_value);
    double sd = sqrt(variance_value);
    double skew_value = skewness(data, size, mean_value, variance_value);

    printf("Mean: %.2f\n", mean_value);
    printf("Variance: %.2f\n", variance_value);
    printf("Standard Deviation: %.2f\n", sd);
    printf("Skewness: %.2f\n", skew_value);

    // Check for other distributions
    if (is_exponential_distribution(data, size)) {
        printf("The dataset is likely following an Exponential Distribution.\n");
    }

    if (is_lognormal_distribution(data, size)) {
        printf("The dataset is likely following a Log-Normal Distribution.\n");
    }

    if (is_pareto_distribution(data, size)) {
        printf("The dataset is likely following a Pareto Distribution.\n");
    } else {
        printf("The dataset does not clearly match any of the distributions.\n");
    }
}

int main() {
    double data[MAX_SIZE];
    int size = 0;

    input_data(data, &size);
    analyze_data(data, size);

    return 0;
}

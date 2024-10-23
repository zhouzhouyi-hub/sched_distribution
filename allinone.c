#include <stdio.h>
#include <math.h>

double compute_alpha(double *data, int n, double xm) {
    double sum_log = 0.0;
    for (int i = 0; i < n; i++) {
        sum_log += log(data[i] / xm);
    }
    return (double)n / sum_log;
}

double compute_xm(double *data, int n) {
    double xm = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] < xm) {
            xm = data[i];
        }
    }
    return xm;
}

double compute_lambda(double *data, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;  // For Poisson, lambda is the mean
}

double compute_mean(double *data, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;  // Mean for Normal
}

double compute_stddev(double *data, int n, double mean) {
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq_diff += (data[i] - mean) * (data[i] - mean);
    }
    return sqrt(sum_sq_diff / n);  // Standard deviation
}

double log_likelihood_exponential(double lambda, double *data, int n) {
    double log_likelihood = 0.0;
    for (int i = 0; i < n; i++) {
        log_likelihood += log(lambda) - lambda * data[i];
    }
    return log_likelihood;
}

double log_likelihood_normal(double mean, double stddev, double *data, int n) {
    double log_likelihood = 0.0;
    for (int i = 0; i < n; i++) {
        log_likelihood += -0.5 * log(2 * M_PI * stddev * stddev) - (data[i] - mean) * (data[i] - mean) / (2 * stddev * stddev);
    }
    return log_likelihood;
}

double log_likelihood_poisson(double lambda, int *data, int n) {
    double log_likelihood = 0.0;
    for (int i = 0; i < n; i++) {
        log_likelihood += data[i] * log(lambda) - lambda - lgamma(data[i] + 1);
    }
    return log_likelihood;
}

double log_likelihood_pareto(double alpha, double xm, double *data, int n) {
    double log_likelihood = 0.0;
    for (int i = 0; i < n; i++) {
        if (data[i] < xm) {
            return -INFINITY; // Invalid for Pareto if any data point is below xm
        }
        log_likelihood += log(alpha) + alpha * log(xm) - (alpha + 1) * log(data[i]);
    }
    return log_likelihood;
}

int main() {
    int n;
    
    // Input number of elements
    // printf("Enter the number of elements: ");
    scanf("%d", &n);
    
    // Check for valid input
    if (n <= 0) {
        printf("Number of elements must be positive.\n");
        return 1;
    }

    // Allocate memory to hold the elements
    double data[n];
    
    // Input elements
    // printf("Enter the elements: ");
    for (int i = 0; i < n; i++) {
        scanf("%lf", &data[i]);
    }
    
    // Compute parameters
    double xm = compute_xm(data, n);
    double alpha = compute_alpha(data, n, xm);
    double mean = compute_mean(data, n);
    double stddev = compute_stddev(data, n, mean);
    double lambda = compute_lambda(data, n);
    
    // Compute log-likelihoods
    double ll_exponential = log_likelihood_exponential(1.0 / mean, data, n);
    double ll_normal = log_likelihood_normal(mean, stddev, data, n);
    double ll_poisson = log_likelihood_poisson(lambda, (int *)data, n); // Assuming data is suitable for Poisson
    double ll_pareto = log_likelihood_pareto(alpha, xm, data, n);

    // Output the results
    printf("Estimated xm for Pareto: %.5lf\n", xm);
    printf("Estimated alpha for Pareto: %.5lf\n", alpha);
    printf("Estimated lambda for Exponential: %.5lf\n", 1.0 / mean);
    printf("Estimated lambda for Poisson: %.5lf\n", lambda);
    printf("Estimated mean for Normal: %.5lf\n", mean);
    printf("Estimated standard deviation for Normal: %.5lf\n", stddev);

    // Determine the best fit distribution
    printf("\nLog-likelihood values:\n");
    printf("Exponential: %.5lf\n", ll_exponential);
    printf("Normal: %.5lf\n", ll_normal);
    printf("Poisson: %.5lf\n", ll_poisson);
    printf("Pareto: %.5lf\n", ll_pareto);

    double best_fit = ll_exponential;
    char *best_fit_name = "Exponential";

    if (ll_normal > best_fit) {
        best_fit = ll_normal;
        best_fit_name = "Normal";
    }
    if (ll_poisson > best_fit) {
        best_fit = ll_poisson;
        best_fit_name = "Poisson";
    }
    if (ll_pareto > best_fit) {
        best_fit = ll_pareto;
        best_fit_name = "Pareto";
    }

    printf("Best fit distribution: %s\n", best_fit_name);

    return 0;
}

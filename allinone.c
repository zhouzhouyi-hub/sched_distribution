#include <stdio.h>
#include <math.h>
#include <gsl/gsl_sf_erf.h>

#define LOGSCALE 1000000
#define DIVSCALE 100000
#define ALPHASCALE 10
unsigned int logdata[LOGSCALE] = {0};
unsigned int logdata1[LOGSCALE] = {0};
void compute_log() {
	float i;
	for (i = 0; i < LOGSCALE; i++) {
		logdata[(int)i] = log(i/DIVSCALE)*DIVSCALE;
		logdata1[(int)i] = (DIVSCALE*pow(0.01, (double)i/DIVSCALE));
	}
}

unsigned long inverse_pareto1(unsigned long alpha1, double xm) {
	unsigned long xm1 = xm*DIVSCALE;
	return xm1/logdata1[(DIVSCALE*ALPHASCALE/alpha1)%LOGSCALE];
}

double compute_alpha(double *data, int n, double xm) {
    double sum_log = 0.0;
    for (int i = 0; i < n; i++) {
            sum_log += log(data[i] / xm);
    }
    return (double)n / sum_log;
}

unsigned long compute_alpha1(double *data, int n, double xm) {
    unsigned long sum_log1 = 0;
    for (int i = 0; i < n; i++) {
	    unsigned long data1 = data[i];
	    unsigned long xm1 = xm;
	    sum_log1 += logdata[((int)(data1*DIVSCALE/xm1))%LOGSCALE];
    }
    return n*DIVSCALE*ALPHASCALE/sum_log1;
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

double erfinv(double x) {
    double w, p;
    w = log((1.0 - x) * (1.0 + x));
    p = 2.0 / (M_PI * 0.147) + 0.5 * w;
    return (x * sqrt(sqrt(p * p - w / 0.147) - p));
}

double inverse_normal(double p, double mean, double stddev) {
    return mean + stddev * sqrt(2) * erfinv(2 * p - 1);  // Quantile for normal distribution
}

float inverse_poisson(double p, double lambda) {
    double cumulative_prob = 0.0;
    float x = 0;
    while (cumulative_prob < p) {
        cumulative_prob += exp(-lambda) * pow(lambda, x) / tgamma(x + 1);
        x++;
    }
    return x - 1;  // Return the smallest x such that cumulative probability >= p
}

double inverse_exp(double p, double lambda) {
    return -log(1 - p) / lambda;  // Quantile for Exponential distribution
}

double inverse_pareto(double p, double alpha, double xm) {
	double pow1 = pow(1 - p, 1 / alpha); 
    return xm / pow(1 - p, 1 / alpha);  // Quantile for Pareto distribution
}


int main() {
    int n;
    compute_log();
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

    double probability;
    printf("Enter probability (e.g., 0.95): ");
    scanf("%lf", &probability);

    // Compute parameters
    double xm = compute_xm(data, n);
    double alpha = compute_alpha(data, n, xm);
    unsigned long alpha1 = compute_alpha1(data, n, xm);
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
    printf("Estimated alpha for Pareto: %.5lf alpha1 %lu\n", alpha, alpha1);
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

    double normal_quantile = inverse_normal(probability, mean, stddev);
    int poisson_quantile = inverse_poisson(probability, lambda);
    double pareto_quantile = inverse_pareto(probability, alpha, xm);
    double exp_quantile = inverse_exp(probability, 1.0/mean);
    unsigned int pareto_quantile1 = inverse_pareto1(alpha1, xm);


    double best_fit = ll_exponential;
    char *best_fit_name = "Exponential";

    double quantile = exp_quantile;
    if (ll_normal > best_fit) {
        best_fit = ll_normal;
        best_fit_name = "Normal";
	quantile = normal_quantile;
    }
    if (ll_poisson > best_fit) {
        best_fit = ll_poisson;
        best_fit_name = "Poisson";
	quantile = poisson_quantile;
    }
    if (ll_pareto > best_fit) {
        best_fit = ll_pareto;
        best_fit_name = "Pareto";
	quantile = pareto_quantile;
    }

    printf("Best fit distribution: %s %f %lu\n", best_fit_name, quantile, pareto_quantile1);

    return 0;
}

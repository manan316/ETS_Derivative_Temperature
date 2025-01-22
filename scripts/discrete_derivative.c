#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For usleep (progress bar)

// Define the maximum length for a line in the CSV file
#define MAX_LINE_LENGTH 1024

// Structure to hold forecast data
typedef struct {
    char date[20];      // Date as a string
    double temperature; // Forecasted temperature
} ForecastData;

// ANSI escape codes for colors
#define RESET_COLOR "\033[0m"
#define INFO_COLOR "\033[1;34m"
#define ERROR_COLOR "\033[1;31m"
#define SUCCESS_COLOR "\033[1;32m"

// Function to print colored messages
void print_colored(const char *text, const char *color) {
    printf("%s%s%s", color, text, RESET_COLOR);
}

// Function to show a simple progress bar
void show_progress(const char *message) {
    printf("%s%s", INFO_COLOR, message);
    fflush(stdout);
    for (int i = 0; i <= 100; i += 10) {
        printf("\r%s%s [%3d%%]%s", INFO_COLOR, message, i, RESET_COLOR);
        fflush(stdout);
        usleep(200000); // Delay in microseconds
    }
    printf("\n");
}

// Function to read forecast data from a CSV file
int read_forecast_data(const char *filename, ForecastData **data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        print_colored("[ERROR] Could not open file: ", ERROR_COLOR);
        printf("%s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    // Count the number of rows (excluding header)
    fgets(line, MAX_LINE_LENGTH, file); // Skip header
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        count++;
    }
    rewind(file); // Reset file pointer

    // Allocate memory for forecast data
    *data = (ForecastData *)malloc(count * sizeof(ForecastData));
    if (!*data) {
        print_colored("[ERROR] Memory allocation failed.\n", ERROR_COLOR);
        fclose(file);
        return -1;
    }

    // Read data into the allocated array
    fgets(line, MAX_LINE_LENGTH, file); // Skip header
    int index = 0;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        if (sscanf(line, "%[^,],%lf", (*data)[index].date, &(*data)[index].temperature) != 2) {
            print_colored("[WARNING] Skipping malformed line in CSV file.\n", ERROR_COLOR);
            continue;
        }
        index++;
    }

    fclose(file);
    return index; // Return number of valid rows
}

// Function to compute and save discrete derivatives
void compute_and_save_derivatives(ForecastData *data, int count, const char *output_filename) {
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        print_colored("[ERROR] Could not open file: ", ERROR_COLOR);
        printf("%s\n", output_filename);
        return;
    }

    // Write header to the output file
    fprintf(output_file, "Date,Temperature,ForwardDerivative,BackwardDerivative,CentralDerivative\n");

    print_colored("\n[INFO] Computing discrete derivatives...\n", INFO_COLOR);
    show_progress("Computing Derivatives");
    for (int i = 0; i < count; i++) {
        double forward_derivative = 0.0;
        double backward_derivative = 0.0;
        double central_derivative = 0.0;

        if (i < count - 1) {
            // Forward difference
            forward_derivative = (data[i + 1].temperature - data[i].temperature);
        }

        if (i > 0) {
            // Backward difference
            backward_derivative = (data[i].temperature - data[i - 1].temperature);
        }

        if (i > 0 && i < count - 1) {
            // Central difference
            central_derivative = (data[i + 1].temperature - data[i - 1].temperature) / 2.0;
        }

        // Write results to the file
        fprintf(output_file, "%s,%.2f,%.2f,%.2f,%.2f\n", data[i].date, data[i].temperature,
                forward_derivative, backward_derivative, central_derivative);
    }

    fclose(output_file);
    print_colored("[INFO] Discrete derivatives saved successfully to: ", SUCCESS_COLOR);
    printf("%s\n", output_filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_colored("[ERROR] Invalid arguments. Usage: ./discrete_derivative <input_csv> <output_csv>\n", ERROR_COLOR);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    ForecastData *data = NULL;

    // Read forecast data from the CSV file
    int count = read_forecast_data(input_filename, &data);
    if (count <= 0) {
        print_colored("[ERROR] Could not read data from file.\n", ERROR_COLOR);
        return 1;
    }

    // Compute and save discrete derivatives
    compute_and_save_derivatives(data, count, output_filename);

    // Free allocated memory
    free(data);

    return 0;
}

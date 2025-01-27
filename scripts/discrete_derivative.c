#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For usleep (used for the progress bar)

// Define the maximum length for a line in the CSV file
#define MAX_LINE_LENGTH 1024

// Structure to hold forecast data
typedef struct {
    char date[20];      // Date as a string (e.g., "mm-dd-yyyy")
    double temperature; // Forecasted temperature
} ForecastData;

// ANSI escape codes for colors for improved output visibility
#define RESET_COLOR "\033[0m"          // Reset color to default
#define INFO_COLOR "\033[1;34m"        // Blue color for informational messages
#define ERROR_COLOR "\033[1;31m"       // Red color for error messages
#define SUCCESS_COLOR "\033[1;32m"     // Green color for success messages

// Function to print colored messages
void print_colored(const char *text, const char *color) {
    printf("%s%s%s", color, text, RESET_COLOR); // Apply color and reset
}

// Function to show a simple progress bar
void show_progress(const char *message) {
    printf("%s%s", INFO_COLOR, message); // Display message in blue
    fflush(stdout); // Ensure output is immediately shown
    for (int i = 0; i <= 100; i += 10) { // Progress from 0% to 100% in steps of 10%
        printf("\r%s%s [%3d%%]%s", INFO_COLOR, message, i, RESET_COLOR); // Overwrite progress line
        fflush(stdout);
        usleep(200000); // 200ms delay to simulate progress
    }
    printf("\n");
}

// Function to read forecast data from a CSV file
int read_forecast_data(const char *filename, ForecastData **data) {
    FILE *file = fopen(filename, "r"); // Open the CSV file for reading
    if (!file) {
        print_colored("[ERROR] Could not open file: ", ERROR_COLOR);
        printf("%s\n", filename); // Print error message if file doesn't open
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    // Count the number of rows (excluding header)
    fgets(line, MAX_LINE_LENGTH, file); // Skip header
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        count++; // Count each data row
    }
    rewind(file); // Reset file pointer to the beginning

    // Allocate memory to store the forecast data
    *data = (ForecastData *)malloc(count * sizeof(ForecastData));
    if (!*data) {
        print_colored("[ERROR] Memory allocation failed.\n", ERROR_COLOR); // Memory allocation failed
        fclose(file);
        return -1;
    }

    // Read data into the allocated array
    fgets(line, MAX_LINE_LENGTH, file); // Skip header
    int index = 0;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        // Read date and temperature values from each line
        if (sscanf(line, "%[^,],%lf", (*data)[index].date, &(*data)[index].temperature) != 2) {
            print_colored("[WARNING] Skipping malformed line in CSV file.\n", ERROR_COLOR); // Malformed line warning
            continue;
        }
        index++;
    }

    fclose(file); // Close the file
    return index; // Return the number of valid rows read
}

// Function to compute and save discrete derivatives
void compute_and_save_derivatives(ForecastData *data, int count, const char *output_filename) {
    FILE *output_file = fopen(output_filename, "w"); // Open the output file for writing
    if (!output_file) {
        print_colored("[ERROR] Could not open file: ", ERROR_COLOR);
        printf("%s\n", output_filename); // Print error message if file doesn't open
        return;
    }

    // Write the header to the output file
    fprintf(output_file, "Date,Temperature,ForwardDerivative,BackwardDerivative,CentralDerivative\n");

    print_colored("\n[INFO] Computing discrete derivatives...\n", INFO_COLOR);
    show_progress("Computing Derivatives"); // Show progress bar
    for (int i = 0; i < count; i++) {
        double forward_derivative = 0.0;
        double backward_derivative = 0.0;
        double central_derivative = 0.0;

        // Compute forward, backward, and central derivatives
        if (i < count - 1) {
            // Forward difference (temperature difference with next value)
            forward_derivative = (data[i + 1].temperature - data[i].temperature);
        }

        if (i > 0) {
            // Backward difference (temperature difference with previous value)
            backward_derivative = (data[i].temperature - data[i - 1].temperature);
        }

        if (i > 0 && i < count - 1) {
            // Central difference (average of forward and backward differences)
            central_derivative = (data[i + 1].temperature - data[i - 1].temperature) / 2.0;
        }

        // Write the results (date, temperature, derivatives) to the output file
        fprintf(output_file, "%s,%.2f,%.2f,%.2f,%.2f\n", data[i].date, data[i].temperature,
                forward_derivative, backward_derivative, central_derivative);
    }

    fclose(output_file); // Close the output file
    print_colored("[INFO] Discrete derivatives saved successfully to: ", SUCCESS_COLOR);
    printf("%s\n", output_filename); // Output success message with file location
}

// Main program function to read data, compute derivatives, and save results
int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_colored("[ERROR] Invalid arguments. Usage: ./discrete_derivative <input_csv> <output_csv>\n", ERROR_COLOR);
        return 1; // If incorrect number of arguments, print error and exit
    }

    const char *input_filename = argv[1]; // Get input CSV filename from arguments
    const char *output_filename = argv[2]; // Get output CSV filename from arguments
    ForecastData *data = NULL; // Pointer to store forecast data

    // Read forecast data from the CSV file
    int count = read_forecast_data(input_filename, &data);
    if (count <= 0) {
        print_colored("[ERROR] Could not read data from file.\n", ERROR_COLOR);
        return 1; // If data couldn't be read, print error and exit
    }

    // Compute and save discrete derivatives
    compute_and_save_derivatives(data, count, output_filename);

    // Free allocated memory for data
    free(data);

    return 0; // Exit the program successfully
}

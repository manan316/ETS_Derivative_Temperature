#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// ANSI escape codes for colors for better output visualization
#define RESET_COLOR "\033[0m"          // Reset color to default
#define INFO_COLOR "\033[1;34m"        // Blue color for informational messages
#define ERROR_COLOR "\033[1;31m"       // Red color for error messages
#define SUCCESS_COLOR "\033[1;32m"     // Green color for success messages
#define TITLE_COLOR "\033[1;36m"       // Cyan color for title headers
#define MENU_COLOR "\033[1;33m"        // Yellow color for menu options

// Function to print colored text to the console
void print_colored(const char *text, const char *color) {
    printf("%s%s%s", color, text, RESET_COLOR); // Apply color and reset
}

// Simulate a progress bar during an operation
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

// Function to clear the input buffer after reading invalid input
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Clear all characters until newline or EOF
}

// Function to center text within a specified width
void center_text(const char *text, int width, const char *color) {
    int padding = (width - strlen(text)) / 2; // Calculate padding for centering
    for (int i = 0; i < padding; i++) {
        printf(" "); // Print spaces before text
    }
    print_colored(text, color); // Print the text in specified color
    for (int i = 0; i < padding; i++) {
        printf(" "); // Print spaces after text
    }
    if ((width - strlen(text)) % 2 != 0) printf(" "); // Handle odd widths
}

// Function to left-align text with padding
void left_align_text(const char *text, int width, const char *color) {
    print_colored(text, color); // Print the text in specified color
    int padding = width - strlen(text); // Calculate the padding
    for (int i = 0; i < padding; i++) {
        printf(" "); // Add padding spaces after text
    }
}

// Function to display the main menu with options
void display_menu() {
    int menu_width = 55; // Set width for the menu

    print_colored("\n", TITLE_COLOR); // Print empty line before the title

    print_colored("+", TITLE_COLOR); // Start of top border
    for (int i = 0; i < menu_width - 2; i++) print_colored("=", TITLE_COLOR); // Horizontal border
    print_colored("+\n", TITLE_COLOR); // End of top border

    // Print the title and subtitle centered
    print_colored("|", TITLE_COLOR);
    center_text("Temperature Forecast Using ETS Model", menu_width - 2, TITLE_COLOR);
    print_colored("|\n", TITLE_COLOR);
    print_colored("|", TITLE_COLOR);
    center_text("A Multi-Language Implementation", menu_width - 2, TITLE_COLOR);
    print_colored("|\n", TITLE_COLOR);

    print_colored("|", TITLE_COLOR);
    for (int i = 0; i < menu_width - 2; i++) print_colored("=", TITLE_COLOR); // Horizontal separator
    print_colored("|\n", TITLE_COLOR);

    // Menu Options (Left-aligned with vertical bar)
    print_colored("| ", MENU_COLOR); 
    print_colored("- ", MENU_COLOR); 
    left_align_text("1. Generate Forecast", menu_width - 5, MENU_COLOR); // Option 1
    print_colored("|\n", MENU_COLOR);

    print_colored("| ", MENU_COLOR);
    print_colored("- ", MENU_COLOR);
    left_align_text("2. View Results", menu_width - 5, MENU_COLOR); // Option 2
    print_colored("|\n", MENU_COLOR);

    print_colored("| ", MENU_COLOR);
    print_colored("- ", MENU_COLOR);
    left_align_text("3. Compute Derivative", menu_width - 5, MENU_COLOR); // Option 3
    print_colored("|\n", MENU_COLOR);

    print_colored("| ", MENU_COLOR);
    print_colored("- ", MENU_COLOR);
    left_align_text("4. Exit", menu_width - 5, MENU_COLOR); // Option 4
    print_colored("|\n", MENU_COLOR);

    print_colored("+", TITLE_COLOR);
    for (int i = 0; i < menu_width - 2; i++) print_colored("=", TITLE_COLOR); // Bottom border
    print_colored("+\n", TITLE_COLOR);

    print_colored("\n", TITLE_COLOR); // Print an empty line for spacing
}

// Function to generate the temperature forecast using an ETS model (in Python)
void generate_forecast() {
    print_colored("\n[INFO] Generating temperature forecast using ETS model...\n", INFO_COLOR);
    show_progress("[INFO] Starting ETS Model Using Statsmodels in Python"); // Show progress during operation
    int result = system("python ./scripts/forecast_ets.py ./data/dataset.csv ./output/forecast_output.csv"); // Run the Python script
    if (result != 0) {
        print_colored("\n[ERROR] Forecast generation failed. Check your data or script.\n", ERROR_COLOR); // If error occurs
    } else {
        print_colored("\n[INFO] Forecast generation completed successfully.\n", SUCCESS_COLOR); // If success
    }
}

// Function to view the forecast results
void view_forecast() {
    print_colored("\n[INFO] Opening forecast results...\n", INFO_COLOR);
    if (system("start excel ./output/forecast_output.csv") != 0) { // Attempt to open in Excel
        print_colored("\n[ERROR] Could not open forecast output file.\n", ERROR_COLOR); // Handle error
    }
}

// Function to compute the discrete derivative from the forecast output
void compute_discrete_derivative() {
    const char *input_file = "./output/forecast_output.csv"; // Input file
    const char *output_file = "./output/discrete_derivative_output.csv"; // Output file for derivative
    char command[512]; // Command to run the executable

    // Compile the C code for derivative calculation
    if (system("gcc -o ./scripts/discrete_derivative.exe ./scripts/discrete_derivative.c") != 0) {
        print_colored("\n[ERROR] Compilation of discrete_derivative.c failed.\n", ERROR_COLOR); // Compilation error
        return;
    }

    // Execute the compiled C program to compute the derivative
    snprintf(command, sizeof(command), ".\\scripts\\discrete_derivative.exe %s %s", input_file, output_file);
    if (system(command) != 0) {
        print_colored("\n[ERROR] Execution of discrete_derivative failed.\n", ERROR_COLOR); // Execution error
        return;
    }

    print_colored("\n[INFO] Discrete derivatives computed successfully.\n", SUCCESS_COLOR); // Success message
    if (system("start excel ./output/discrete_derivative_output.csv") != 0) { // Open the result in Excel
        print_colored("\n[ERROR] Could not open discrete derivative output file.\n", ERROR_COLOR); // Error opening file
    }
}

// Main program function to display the menu and handle user choices
int main() {
    int choice;

    while (1) { // Infinite loop to keep showing the menu until exit is chosen
        display_menu(); // Show the menu

        print_colored("ENTER YOUR CHOICE: ", MENU_COLOR); // Prompt for user input
        while (scanf("%d", &choice) != 1) { // Validate input (must be an integer)
            print_colored("\n[ERROR] Invalid input! Please enter a NUMBER.\n", ERROR_COLOR); // Error on invalid input
            clear_input_buffer(); // Clear invalid input
            print_colored("ENTER YOUR CHOICE: ", MENU_COLOR);
        }

        // Switch to handle different user choices
        switch (choice) {
            case 1:
                generate_forecast(); // Option 1: Generate forecast
                break;
            case 2:
                view_forecast(); // Option 2: View forecast results
                break;
            case 3:
                compute_discrete_derivative(); // Option 3: Compute derivative
                break;
            case 4:
                print_colored("\nExiting the program... Thank you!\n", SUCCESS_COLOR); // Option 4: Exit
                print_colored("Press Enter to exit...\n", INFO_COLOR);
                getchar(); // Wait for user to press Enter
                getchar();
                return 0; // Exit the program
            default:
                print_colored("\n[ERROR] Invalid choice! Please select a valid option.\n", ERROR_COLOR); // Invalid choice
                break;
        }
    }

    return 0;
}

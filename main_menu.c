#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For usleep (simulate progress bar)

// Functions
void generate_forecast();
void view_forecast();
void compute_discrete_derivative();
void display_menu();
void print_colored(const char *text, const char *color);
void show_progress(const char *message);

// ANSI escape codes for colors
#define RESET_COLOR "\033[0m"
#define INFO_COLOR "\033[1;34m"
#define ERROR_COLOR "\033[1;31m"
#define SUCCESS_COLOR "\033[1;32m"
#define TITLE_COLOR "\033[1;36m"
#define MENU_COLOR "\033[1;33m"

// Function to print colored text
void print_colored(const char *text, const char *color) {
    printf("%s%s%s", color, text, RESET_COLOR);
}

// Simulate a progress bar
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

int main() {
    int choice;

    while (1) {
        display_menu();

        print_colored("ENTER YOUR CHOICE: ", MENU_COLOR);
        if (scanf("%d", &choice) != 1) {
            print_colored("\n[ERROR] Invalid input! Please enter a number.\n", ERROR_COLOR);
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                generate_forecast();
                break;
            case 2:
                view_forecast();
                break;
            case 3:
                compute_discrete_derivative();
                break;
            case 4:
                print_colored("\nExiting the program... Thank you!\n", SUCCESS_COLOR);
                print_colored("Press Enter to exit...\n", INFO_COLOR);
                getchar();
                getchar(); // Wait for Enter
                return 0;
            default:
                print_colored("\n[ERROR] Invalid choice! Please select a valid option.\n", ERROR_COLOR);
                break;
        }
    }

    return 0;
}

void display_menu() {
    print_colored("\n=================================================\n", TITLE_COLOR);
    print_colored("  Temperature Forecast Using the ETS Model\n", TITLE_COLOR);
    print_colored("    A Multi-Language Implementation\n", TITLE_COLOR);
    print_colored("=================================================\n\n", TITLE_COLOR);
    print_colored("1.  Generate Temperature Forecast\n", MENU_COLOR);
    print_colored("2.  View Results\n", MENU_COLOR);
    print_colored("3.  Compute Discrete Derivative\n", MENU_COLOR);
    print_colored("4.  Exit\n", MENU_COLOR);
    print_colored("\n=================================================\n", TITLE_COLOR);
}

void generate_forecast() {
    print_colored("\n[INFO] Generating temperature forecast using ETS model...\n", INFO_COLOR);
    show_progress("Starting ETS Model Using Statsmodels in Python");
    int result = system("python ./scripts/forecast_ets.py ./data/dataset.csv ./output/forecast_output.csv");
    if (result != 0) {
        print_colored("\n[ERROR] Forecast generation failed. Check your data or script.\n", ERROR_COLOR);
    } else {
        print_colored("\n[INFO] Forecast generation completed successfully.\n", SUCCESS_COLOR);
    }
}

void view_forecast() {
    print_colored("\n[INFO] Opening forecast results...\n", INFO_COLOR);
    if (system("start excel ./output/forecast_output.csv") != 0) {
        print_colored("\n[ERROR] Could not open forecast output file.\n", ERROR_COLOR);
    }
}

void compute_discrete_derivative() {
    const char *input_file = "./output/forecast_output.csv";
    const char *output_file = "./output/discrete_derivative_output.csv";
    char command[512];

    if (system("gcc -o ./scripts/discrete_derivative.exe ./scripts/discrete_derivative.c") != 0) {
        print_colored("\n[ERROR] Compilation of discrete_derivative.c failed.\n", ERROR_COLOR);
        return;
    }

    snprintf(command, sizeof(command), ".\\scripts\\discrete_derivative.exe %s %s", input_file, output_file);
    if (system(command) != 0) {
        print_colored("\n[ERROR] Execution of discrete_derivative failed.\n", ERROR_COLOR);
        return;
    }

    print_colored("\n[INFO] Discrete derivatives computed successfully.\n", SUCCESS_COLOR);
    if (system("start excel ./output/discrete_derivative_output.csv") != 0) {
        print_colored("\n[ERROR] Could not open discrete derivative output file.\n", ERROR_COLOR);
    }
}

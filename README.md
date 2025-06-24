Temperature Prediction Using ETS Model: A Multi-Language Platform


This project predicts daily average temperatures using the Exponential Smoothing (ETS) model, implemented in Python, and computes discrete derivatives to analyze short-term temperature variations using C.
The project includes a user-friendly command-line interface (CLI) for seamless interaction, providing options to generate forecasts, compute derivatives, and visualize results.

Features
1. Forecast temperature trends over a 45-day period using the ETS model.
2. Analyze day-to-day temperature variations with forward, backward, and central derivatives.
3. Interactive CLI with color-coded messages and progress bars.
4. Seamless integration of Python for forecasting and C for computational analysis.


Prerequisites
### Software Requirements
- Windows 10 and Above (Windows 11 Recomended)
- Python 3.10+
- C Compiler (GCC recommended)
- Microsoft Excel (To view csv files)


### Python Libraries
Install the required Python libraries using:
```bash
pip install pandas statsmodels matplotlib tqdm
```


Getting Started
1. Clone the Repository:
```bash
git clone https://github.com/manan316/ETS_Derivative_Temperature.git
cd ETS_Derivative_Temperature
```

2. Set Up the Project:
- Place your dataset in the `data/` directory (e.g., `dataset.csv`).
- Ensure the dataset contains a `date` column and an `avtemp` column.

3. Compile the C Programs:
```bash
gcc -o main_menu main_menu.c
gcc -o ./scripts/discrete_derivative ./scripts/discrete_derivative.c
```


Usage
### Run the Main Menu
Launch the main menu:
```bash
./main_menu
```
#### Menu Options:
1. Generate Temperature Forecast: Invokes the Python script to train the ETS model and generate forecasts.
2. View Results: Opens the forecasted data in Excel.
3. Compute Discrete Derivative: Computes forward, backward, and central derivatives using the C program.
4. Exit: Closes the program.
How It Works

#### Forecasting with ETS
The `forecast_ets.py` script:
- Loads the temperature dataset.
- Preprocesses the data to handle missing values and standardize it.
- Configures and trains the ETS model to predict temperatures for the next 45 days.
- Outputs the results to `output/forecast_output.csv`.

#### Derivative Computation
The `discrete_derivative.c` program:
- Reads forecasted data from `output/forecast_output.csv`.
- Computes discrete derivatives to analyze daily temperature variations.
- Outputs the results to `output/discrete_derivative_output.csv`.


Sample Dataset
A sample dataset (`dataset.csv`) is provided in the `data/` directory. The dataset contains:
- `date`: Date of temperature recording.
- `avtemp`: Average daily temperature in Fahrenheit.


Outputs
- Forecast Output: `output/forecast_output.csv`
  - Contains 45-day forecasted temperatures.
- Derivative Output: `output/discrete_derivative_output.csv`
  - Includes forward, backward, and central derivatives for the forecasted data.

Created by
Manan Sharma 
Ishu Bharati Pandit 

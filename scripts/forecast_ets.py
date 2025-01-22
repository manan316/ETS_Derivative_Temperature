import pandas as pd
import matplotlib.pyplot as plt
from statsmodels.tsa.exponential_smoothing.ets import ETSModel
import warnings
from tqdm import tqdm
import sys
import time

# Color codes for console output
INFO_COLOR = "\033[1;34m"
ERROR_COLOR = "\033[1;31m"
RESET_COLOR = "\033[0m"

# Logging functions
def print_info(message):
    print(f"{INFO_COLOR}[INFO] {message}{RESET_COLOR}")

def print_error(message):
    print(f"{ERROR_COLOR}[ERROR] {message}{RESET_COLOR}")

warnings.filterwarnings('ignore')  # Suppress warnings for cleaner output

def main():
    if len(sys.argv) != 3:
        print_error("Invalid arguments. Usage: python forecast_ets.py <input_csv> <output_csv>")
        return

    input_csv = sys.argv[1]
    output_csv = sys.argv[2]

    try:
        # Step 1: Load the dataset
        print_info("Loading dataset...")
        df = pd.read_csv(input_csv, parse_dates=['date'], index_col='date')
        df = df.resample('D').asfreq()
        print_info("Dataset loaded successfully.")

        # Step 2: Preprocess the data
        print_info("Preprocessing data...")
        df['avtemp'] = df['avtemp'].interpolate(method='linear')
        mean_temp, std_temp = df['avtemp'].mean(), df['avtemp'].std()
        df['TAVG_scaled'] = (df['avtemp'] - mean_temp) / std_temp
        print_info("Data preprocessing complete.")

        # Step 3: Train the ETS model with progress updates
        print_info("Training ETS model...")

        # Initialize the ETS model
        model = ETSModel(
            df['TAVG_scaled'],
            error='add',
            trend='add',
            seasonal='add',
            seasonal_periods=365,
            damped_trend=True,
            initialization_method='estimated'
        )

        # Start training and simulate progress
        num_iterations = 1000
        total_time_seconds = 1200  # Total time in seconds (20 minutes)
        delay_per_iteration = total_time_seconds / num_iterations  # Delay per iteration in seconds

        progress_bar = tqdm(total=num_iterations, desc="Model Training Progress", ncols=100)
        for _ in range(num_iterations):
            time.sleep(delay_per_iteration)  # Simulate time delay for each iteration
            progress_bar.update(1)
        
        # Fit the model
        fitted_model = model.fit(disp=False, maxiter=num_iterations)

        progress_bar.n = num_iterations
        progress_bar.last_print_n = num_iterations
        progress_bar.refresh()
        progress_bar.close()
        print_info("Model training complete.")

        # Step 4: Generate forecasts
        print_info("Generating forecasts...")
        forecast_days = 45
        forecast_scaled = fitted_model.forecast(forecast_days)
        forecast = (forecast_scaled * std_temp) + mean_temp
        forecast_dates = pd.date_range(start=df.index[-1] + pd.Timedelta(days=1), periods=forecast_days, freq='D')
        forecast_df = pd.DataFrame({'date': forecast_dates, 'avtemp': forecast})
        forecast_df.set_index('date', inplace=True)
        forecast_df.to_csv(output_csv, date_format="%Y-%m-%d")
        print_info(f"Forecast saved to {output_csv}")

        # Step 5: Plot results
        print_info("Plotting results...")
        plt.figure(figsize=(14, 7))
        plt.plot(df.index[-365:], df['avtemp'][-365:], label='Historical Data', alpha=0.8, color='blue')
        plt.plot(forecast_df.index, forecast_df['avtemp'], label='Forecast', color='green')
        plt.title('Temperature Forecast', fontsize=16)
        plt.xlabel('Date', fontsize=12)
        plt.ylabel('Temperature (°F)', fontsize=12)
        plt.legend(fontsize=12)
        plt.grid(alpha=0.4)
        plt.tight_layout()
        plt.savefig("./output/forecast_plot.png")
        plt.show()
        print_info("Forecasting process completed successfully.")

    except Exception as e:
        print_error(f"An error occurred: {str(e)}")

if __name__ == "__main__":
    main()

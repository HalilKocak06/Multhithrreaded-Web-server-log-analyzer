# Multithreaded Web Server Log Analyzer

This project is a C++ application that analyzes Apache-style web server log files using multithreading for performance and scalability.

---

## 🔍 Features

- Parses large `access_generated.log` files efficiently
- Extracts key log data:
  - IP address statistics
  - Endpoint (URL path) access counts
  - Hourly traffic distribution
- Uses `std::thread` for concurrent processing
- Ensures thread-safe data access with `std::mutex`
- Measures total runtime using `std::chrono`
- Outputs results to `.csv` files for further analysis or visualization

---

## 🚀 Technologies Used

- **C++17**
- STL components: `std::thread`, `std::mutex`, `std::map`, `std::vector`, `std::chrono`
- File handling via `ifstream` / `ofstream`
- CSV format for output

---

## 📁 Sample Output Files

After execution, the following files are generated:

- `ip_stats.csv` – Number of requests per IP address
- `endpoint_stats.csv` – Number of requests per endpoint
- `hourly_stats.csv` – Request count per hour of day

These files are compatible with tools like Excel or Python for further analysis.

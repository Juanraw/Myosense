# Myosense

A real-time EMG signal acquisition and classification application using a Myo armband and a trained linear SVM model, implemented in C++ with Qt and CMake.

---

## 🧠 Overview

This application allows users to:

- Acquire EMG data from the **Myo armband**.
- Store the data for further analysis.
- Classify short EMG sequences in real time using a **linear Support Vector Machine (SVM)**.

> No external MATLAB or CSV files are required — the trained SVM model is **hardcoded** inside the `classifier.cpp` file.

---

## 📁 Project Structure
```
MyoSense/
├── libs/ # Includes Myo SDK and driver files
├── src/ # Main source code and SVM logic
│ ├── main.cpp
│ ├── classifier.h/cpp
│ ├── mainwindow.h/cpp
│ └── myoread.hpp/cpp
├── front/ # Visual interface code
│ └── mainwindow.ui
├── CMakeLists.txt # Build system definition
├── README.md # This file
└── LICENSE
```
## ⚙️ Dependencies

The following components are required:

- **Myo SDK** (included in `/libs`)
- **Qt 5 or 6**
- **CMake ≥ 3.14**
- A C++ compiler (GCC, MSVC, or Clang)
- Myo connect application Running (MUST)

---

## 🚀 How to Build

### 1. Clone this repository

bash
`git clone https://github.com/Juanraw/Myosense.git
cd MyoSense`

### 2. Clone this repository
`mkdir build
cd build
cmake ..`

### 3. Build the application

`cmake --build`

🎯 How to Use

1. Connect your Myo armband via Bluetooth.

2. Ensure that the Myo Connect software is running.

3. Run the application:

./MyoSense       

4. The application will:
   
  -Start acquiring data

  -Save signals locally

  -Classify each signal in real time using the built-in SVM model


🧪 Reproducibility

This capsule contains:

  -A complete copy of the SVM model logic.

  -All dependencies locally in /libs. (Not the Myo Connect app)

  -All source code and build instructions.

  -No hidden training or data files required.

You can reproduce results or fork the classifier logic for your own EMG-based applications.

🙋 Support
If you encounter any issues:

Open an issue on GitHub

Or contact the maintainer: [est.juan.ramirez3@unimilitar.edu.co]

📌 Acknowledgments
This project uses the Myo SDK by Thalmic Labs.

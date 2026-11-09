# Chase Banking Demo

A small desktop banking demonstration built with C++17 and Qt 6 Widgets. It replaces a console menu with a graphical dashboard for creating accounts, selecting an active account, depositing and withdrawing funds, and reviewing recent activity.

> **Prototype only:** This project is an educational, local demo. It is not a real banking service, does not connect to Chase or any financial institution, and is not an official Chase product. It has no authentication, secure storage, or persistent transaction system. Do not enter real personal or financial information.

## What the application does

When launched, the program opens a desktop window with three sections:

- **Overview** shows the sum of all account balances, the number of accounts, a count of recorded activity, the active account, quick deposit/withdraw actions, and the five most recent activity entries.
- **Accounts** lists the account holder, account number, and current balance. Select a row to make that account active.
- **Activity** shows account openings, deposits, and withdrawals in reverse chronological order.

Use **New account** or **Create account** to enter an account number, first and last name, and optional starting balance. Account numbers must be unique. A successfully created account becomes active.

The overview's deposit and withdrawal actions operate on the active account. Amounts must be greater than zero; a withdrawal cannot exceed the available balance. A successful transaction changes the account balance and adds a timestamped activity entry. Invalid account details and declined transactions are reported in dialogs.

All amounts are displayed in US dollars. The overview and activity screens prepend bullet marks to account numbers but still show every digit; the Accounts table shows the number plainly.

## Source code tour

| File | Responsibility |
| --- | --- |
| [`main.cpp`](main.cpp) | Starts Qt with `QApplication`, sets application metadata, creates and displays the main window, and runs the GUI event loop. |
| [`include/bankingWindow.h`](include/bankingWindow.h) | Declares the `BankingWindow`, its pages, in-memory account and transaction collections, and UI/update handlers. |
| [`bankingWindow.cpp`](bankingWindow.cpp) | Builds and styles the dashboard, account and activity tables, navigation, and dialogs. It validates account creation, calls the account model for transactions, and refreshes the UI. |
| [`include/bankAccount.h`](include/bankAccount.h) | Declares the `bankAccount` data model and its account, name, card-detail, and balance fields and operations. |
| [`bankAccount.cpp`](bankAccount.cpp) | Implements account initialization, balance access, and deposit/withdraw validation and balance updates. |
| [`CMakeLists.txt`](CMakeLists.txt) | Defines the CMake project, C++ standard, Qt dependency, executable sources, include directory, and link target. On Apple platforms, configures the executable as a macOS app bundle. |

### How the pieces work together

1. `main()` constructs `QApplication` and `BankingWindow`; Qt dispatches user input through its event loop.
2. `BankingWindow` owns a `std::vector<bankAccount>` and tracks the selected account by index. The UI is built from Qt Widgets in code; there is no `.ui` designer file.
3. Creating an account validates the names and ensures its number is not already in use, then adds a `bankAccount` and selects it.
4. A deposit or withdrawal dialog asks the model to perform the operation. `bankAccount::deposit()` accepts a finite positive amount; `bankAccount::withdraw()` also requires the amount not to exceed the current balance. Both return success or failure to the UI.
5. On success, the window appends a timestamped transaction record and refreshes the balance summary, account table, and activity tables.

The interface collects a card PIN, CVC, or expiration date nowhere; those fields only exist as unused strings in the account model.

## Requirements and build tools

- **C++ compiler** with C++17 support
- **CMake 3.25** or newer
- **Qt 6.2** or newer, with the **Widgets** module
- A platform-appropriate build toolchain (for example, Apple Clang and Ninja/Xcode on macOS, GCC/Clang and Ninja/Make on Linux, or MSVC and Visual Studio on Windows)

CMake configures the project, locates Qt 6 Widgets, generates native build files, and builds the `bankAccount` executable. The CMake target links against `Qt6::Widgets`. No additional application libraries or package manager are required.

### macOS

With [Homebrew](https://brew.sh/):

```sh
brew install cmake qt
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
cmake --build build --config Release
open build/bankAccount.app
```

The project marks the target as a macOS app bundle. If you prefer to launch the executable directly:

```sh
./build/bankAccount.app/Contents/MacOS/bankAccount
```

### Linux (Ubuntu/Debian example)

Install CMake, a C++ compiler, and the Qt 6 Widgets development package:

```sh
sudo apt update
sudo apt install build-essential cmake qt6-base-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/bankAccount
```

Package names can differ across distributions. Install the distribution's Qt 6 Widgets development package if it uses a different name.

### Windows

Install CMake, Visual Studio with the **Desktop development with C++** workload (MSVC), and Qt 6 Widgets built for the same MSVC toolchain. Open a Visual Studio Developer PowerShell, replacing the example Qt path with the installed Qt kit prefix:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2022_64"
cmake --build build --config Release
.\build\Release\bankAccount.exe
```

Use a Qt kit that matches the compiler/generator in use (for example, do not mix an MSVC Qt kit with a MinGW build). For a single-configuration generator such as Ninja, specify `-DCMAKE_BUILD_TYPE=Release` during configuration; the `--config Release` option is primarily for multi-configuration generators such as Visual Studio.

### Qt package discovery

If CMake reports that it cannot find Qt 6, point `CMAKE_PREFIX_PATH` at the Qt installation or kit prefix containing the `Qt6Config.cmake` package files. For example:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x.x/<platform-kit>"
```

After changing the Qt installation or generator, configure a fresh build directory (for example, `build-qt`) so CMake does not reuse stale configuration.

## Data, safety, and current limitations

- Accounts and activity are held in memory by the window. Closing the application discards them; there is no file or database storage.
- Account numbers are not actually redacted in the UI: bullet prefixes are decorative, and the full digits remain visible.
- The app has no user login, authorization, encryption, server, network connection, or connection to a bank.
- PIN, CVC, and expiration-date members are unused and are not collected by the UI.
- This is a single-process demo, not a financial ledger. It does not provide production banking guarantees, auditability, concurrency controls, or financial-grade precision.
- The repository does not currently define a CMake test target or automated test suite.

## License

No license file is currently included in this repository. Contact the project owner before redistributing this project.

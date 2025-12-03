# Simple Task Scheduler (C++ implementation)
> Made by - Subhajit Das (60) & Saksham Kumar Gupta (50)
---

This project implements a Task Scheduler using C++ to manage the lifecycle of tasks with both a console-based CLI and an enhanced FTXUI-based graphical terminal interface. Tasks transition through three distinct states: **Staged** (waiting to start), **Active** (in progress), and **Finished** (completed).
The application features:
- **Modular object-oriented design** with separation of concerns
- **Two user interfaces**: Classic CLI and modern FTXUI TUI
- **CSV logging** for finished tasks with timestamps and duration tracking
- **Time tracking** in seconds for precise task duration measurement

---

## Project Structure

The codebase is organized into these main components:

1.  **`Task.h`:** Defines the `Task` data structure and its `Status` enum (header-only implementation).
2.  **`Scheduler.h` / `Scheduler.cpp`:** Defines and implements the `Scheduler` class, which handles all task management, state transitions, and CSV logging.
3.  **`main_ftxui.cpp`:** Provides an enhanced FTXUI-based graphical terminal interface with interactive navigation.

---

## Core Components and Functions

### 1. Task Class (in `Task.h`)

This class represents a single unit of work, along with its description, time estimates, and actual timing data.

| Function Name | Description |
| :--- | :--- |
| `Task(int id, const std::string& description, int estimate)` | Constructor to create a new task in the **Staged** state. Time estimate in **seconds**. |
| `void markActive()` | Sets the task's status to **Active** and records the `startTime`. |
| `void markFinished()` | Sets the task's status to **Finished** and records the `finishTime`. |
| `std::string getDetails() const` | Returns a formatted string with all task details for display. |

**Member Variables:**
- `int id` - Unique task identifier
- `std::string description` - Task description
- `Status status` - Current state (Staged/Active/Finished)
- `std::time_t startTime` - Timestamp when task was started
- `std::time_t finishTime` - Timestamp when task was completed
- `int estimatedDurationSeconds` - Estimated duration in seconds

### 2. Scheduler Class (in `Scheduler.h` and `Scheduler.cpp`)

This class manages the three internal lists of tasks (`stagedTasks`, `activeTasks`, `finishedLog`) and handles all state transitions and CSV logging.

| Function Name | Description |
| :--- | :--- |
| `Scheduler()` | Constructor. Initializes the internal task ID counter (`nextId`) to 1. |
| `void addTask(const std::string& description, int estimate)` | Creates a new task with time estimate in **seconds** and adds it to the **Staged** list. |
| `void startTask(int id)` | Finds the task by ID in the **Staged** list, marks it as active, and moves it to the **Active** list. |
| `void finishTask(int id)` | Finds the task by ID in the **Active** list, marks it as finished, moves it to the **Finished Log**, and **logs it to CSV**. |
| `void viewStagedTasks() const` | Prints all tasks currently in the **Staged** list. |
| `void viewActiveTasks() const` | Prints all tasks currently in the **Active** list. |
| `void printLog() const` | Prints the **Finished Log**, including actual duration calculated as (finish time - start time). |
| `const std::vector<Task>& getStagedTasks() const` | Returns a reference to the staged tasks list (used by FTXUI). |
| `const std::vector<Task>& getActiveTasks() const` | Returns a reference to the active tasks list (used by FTXUI). |
| `const std::vector<Task>& getFinishedTasks() const` | Returns a reference to the finished tasks list (used by FTXUI). |
| `Task* findTaskById(int id, std::vector<Task>& list)` | Public utility function to locate a task within a specified vector by its ID. |
| `void logTaskToCSV(const Task& task)` | Private function that logs finished tasks to `finished_tasks.csv` with full details. |

### 3. CSV Logging Feature

When a task is finished, it is automatically logged to `finished_tasks.csv` with the following fields:
- **ID** - Task identifier
- **Description** - Task description (properly escaped for CSV)
- **Estimated Duration (sec)** - Original time estimate in seconds
- **Start Time** - Formatted as `YYYY-MM-DD HH:MM:SS`
- **Finish Time** - Formatted as `YYYY-MM-DD HH:MM:SS`
- **Actual Duration (sec)** - Calculated duration between start and finish

### 4. User Interfaces

#### FTXUI Interface (`main_ftxui.cpp`)
An enhanced terminal user interface featuring:
- **Interactive menu navigation** with arrow keys
- **Tab key** to switch between input fields
- **Real-time task viewing** with color-coded displays
- **Visual borders and formatted layouts**
- **Status messages** with success/error indicators
- **Integrated task panels** showing tasks directly in the interface

**Features:**
- Cyan-colored headers for input sections
- Green checkmarks (✓) for successful operations
- Red X marks (✗) for errors
- Color-coded task lists (Cyan for Staged, Yellow for Active, Green for Finished)
- Responsive layout with bordered panels

---

## Compilation Instructions

### Prerequisites
- **C++17 compiler** (g++ or clang++)
- **FTXUI library** (included in the repository under `FTXUI/` directory)
```git
git clone https://github.com/ArthurSonzogni/FTXUI.git
```

### Building the FTXUI Library

```bash
cd FTXUI
mkdir build
cd build
cmake ..
cmake --build .
cd ../..
```

### Compiling the Scheduler

**FTXUI Version:**
```bash
g++ -std=c++17 -o scheduler_ftxui main_ftxui.cpp src/Scheduler.cpp -I./FTXUI/include -L./FTXUI/build -lftxui-component -lftxui-dom -lftxui-screen
```

---

## Usage Instructions

### Running the FTXUI Version

```bash
./scheduler_ftxui.exe   # Windows
./scheduler_ftxui       # Linux/Mac
```

**Navigation:**
- **Arrow keys** - Navigate menu options
- **Tab** - Switch between input fields
- **Enter** - Execute selected action or submit input
- **Type** - Enter task details directly

**Menu Options:**
1. **Add Task** - Enter description and estimated duration (in seconds)
2. **Start Task** - Enter task ID to start
3. **Finish Task** - Enter task ID to finish (automatically logs to CSV)
4. **View Staged Tasks** - Display all tasks waiting to be started
5. **View Active Tasks** - Display all tasks currently in progress
6. **View Finished Log** - Display completed tasks with actual durations
7. **Exit** - Close the application

---

## Features Summary

- **Task Lifecycle Management** - Staged → Active → Finished  
- **Automatic CSV Logging** - All finished tasks saved to `finished_tasks.csv`  
- **Time Tracking** - Precise duration tracking in seconds  
- **Enhanced UI** - Beautiful FTXUI terminal interface with colors and borders  
- **Interactive Navigation** - Keyboard-driven menu system  
- **Real-time Display** - Tasks displayed directly in the interface  
- **Data Persistence** - CSV format for easy data analysis  
- **Proper Documentation** - Comprehensive Doxygen comments in source code  

---

## Output Files

- **`finished_tasks.csv`** - Automatically created/updated when tasks are finished, containing : *ID, Description, Estimated Duration, Start Time, Finish Time, Actual Duration*

---

## Technical Details

- **C++ Standard:** C++17
- **Libraries:** FTXUI (v5.0.0+)
- **Time Tracking:** Uses `<ctime>` for timestamp recording
- **CSV Format:** RFC 4180 compliant with proper escaping
- **Memory Management:** Uses STL containers (`std::vector`)
- **Design Pattern:** Object-Oriented with clear separation of concerns
# AI Algorithmic Challenge Game Environment 

## 🎮 Game Concept

The game presents sequential algorithmic challenges where an AI agent must solve problems optimally while being penalized for inefficiency, invalid moves, resource mismanagement, and logic failures. Different AI implementations can be "plugged in" to compete against each other.

### Challenge Types

1. **Pathfinding Challenge**: Navigate a grid with obstacles from start to goal with minimal steps
2. **Sorting Challenge**: Sort an array using a minimum number of comparisons and swaps
3. **Resource Allocation Challenge**: Optimally distribute resources across tasks while respecting constraints

## 📋 Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                   GAME ENGINE (Game.h)                   │
│                                                          │
│  ┌──────────────────┐  ┌──────────────────┐           │
│  │   Challenges     │  │   AI Agents      │           │
│  │  (Challenge.h)   │  │  (AIAgent.h)     │           │
│  │                  │  │                  │           │
│  │ • Pathfinding    │  │ • SimpleGreedy   │           │
│  │ • Sorting        │  │ • Optimized      │           │
│  │ • Resources      │  │ • Custom (user)  │           │
│  └──────────────────┘  └──────────────────┘           │
│         △                        △                      │
│         └────────────┬───────────┘                      │
│                      │                                  │
│          ┌─────────────────────────┐                   │
│          │  PENALTY SCOREBOARD     │                   │
│          │ (PenaltyScoreboard.h)   │                   │
│          │                         │                   │
│          │ • Error Tracking        │                   │
│          │ • Penalty Calculation   │                   │
│          │ • Performance Metrics   │                   │
│          └─────────────────────────┘                   │
└─────────────────────────────────────────────────────────┘
```

## 🛠️ Core Components

### 1. **PenaltyScoreboard** (`PenaltyScoreboard.h/cpp`)
Tracks and manages penalties for AI agent errors:
- **Algorithmic Inefficiency**: Exceeding step/time limits
- **Invalid Moves**: Actions violating game rules
- **Resource Mismanagement**: Memory limit violations
- **Logic Failures**: Incorrect outputs or failed conditions
- **Time Limit Exceeded**: Execution timeout

**Key Methods**:
```cpp
void recordPenalty(ErrorCategory, int points, std::string description, int levelId);
void penalizeAlgorithmicInefficiency(int stepsUsed, int stepsAllowed, int levelId);
void penalizeInvalidMove(const std::string& action, int levelId);
void penalizeResourceMismanagement(size_t memoryUsed, size_t limit, int levelId);
void penalizeLogicFailure(const std::string& reason, const std::string& expected, int levelId);
int getEfficiencyScore() const;  // Returns 0-100
std::string generateReport() const;
```

### 2. **Challenge** (`Challenge.h/cpp`)
Abstract base class defining the interface for algorithmic challenges:
```cpp
class Challenge {
    virtual void initialize() = 0;
    virtual bool executeAction(const std::string& action) = 0;
    virtual bool isSolved() const = 0;
    virtual std::string getState() const = 0;
    virtual std::string getExpectedResult() const = 0;

    // Built-in tracking
    void incrementStepCount();
    bool isStepLimitExceeded() const;
    void recordMemoryUsage(size_t bytes);
    bool isMemoryLimitExceeded() const;
};
```

### 3. **Concrete Challenges** (`ConcreteChallenges.h/cpp`)

#### PathfindingChallenge
- **Objective**: Reach goal position with minimum steps
- **Actions**: `MOVE_UP`, `MOVE_DOWN`, `MOVE_LEFT`, `MOVE_RIGHT`
- **Configuration**: 10×10 grid, max 15 steps, 1KB memory
- **Mechanics**: Grid-based navigation with obstacles

#### SortingChallenge
- **Objective**: Sort array using minimum operations
- **Actions**: `COMPARE x y`, `SWAP x y`, `VERIFY`
- **Configuration**: 7-element array, max 50 steps, 2KB memory
- **Mechanics**: Track comparison and swap counts

#### ResourceAllocationChallenge
- **Objective**: Allocate resources to tasks optimally
- **Actions**: `ALLOCATE task_id amount`
- **Configuration**: 5 tasks, 100 units total, max 20 steps, 4KB memory
- **Mechanics**: Respect min/max constraints per task

### 4. **AIAgent** (`AIAgent.h/cpp`)
Abstract base class for AI implementations:
```cpp
class AIAgent {
    virtual bool solveChallenge(Challenge& challenge,
                                PenaltyScoreboard& scoreboard) = 0;
    void recordCompletion(bool success, int stepsUsed, int penaltyPoints);
    AgentStats getStats() const;
};
```

**Implementations Provided**:
- **SimpleGreedyAgent**: Heuristic-based greedy approach
- **OptimizedAgent**: More sophisticated algorithm selection

**Custom Implementation Example**:
```cpp
class MyCustomAgent : public AIAgent {
public:
    MyCustomAgent() : AIAgent("MyAgent") {}

    bool solveChallenge(Challenge& challenge,
                       PenaltyScoreboard& scoreboard) override {
        // Your algorithm here
        while (!challenge.isSolved()) {
            std::string action = computeNextAction(challenge);
            if (!challenge.executeAction(action)) {
                scoreboard.penalizeInvalidMove(action,
                    challenge.getConfig().levelId);
                return false;
            }
        }
        return true;
    }

private:
    std::string computeNextAction(Challenge& challenge) {
        // Your AI logic
        return "MOVE_RIGHT";
    }
};
```

### 5. **Game** (`Game.h/cpp`)
Main orchestrator managing gameplay flow:
```cpp
class Game {
    void addChallenge(std::unique_ptr<Challenge> challenge);
    Game::SessionResult playSession(AIAgent& agent);
    bool playLevel(int levelIndex, AIAgent& agent);
    const PenaltyScoreboard& getScoreboard() const;
};
```

## 🏗️ Building & Compilation

### Using CMake (Recommended)

```bash
# Navigate to project directory
cd d:\HOCTAP\IT\C++

# Create build directory
mkdir build
cd build

# Configure project
cmake ..

# Compile
cmake --build . --config Release

# Run executable
./bin/ai_game_env    # Linux/Mac
.\bin\ai_game_env.exe  # Windows
```

### Manual Compilation (g++/clang)

```bash
# Compile all source files
g++ -std=c++17 -O2 -o ai_game_env \
    main.cpp \
    PenaltyScoreboard.cpp \
    Challenge.cpp \
    ConcreteChallenges.cpp \
    AIAgent.cpp \
    Game.cpp

# Run
./ai_game_env
```

### Using MSVC (Windows)

```bash
cl /std:c++17 /EHsc /O2 *.cpp
ai_game_env.exe
```

## 📊 Penalty System Examples

### Algorithmic Inefficiency
```
Challenge: Find shortest path (ideal: 10 steps)
AI Steps: 15
Penalty: (15-10) * 5 / 10 = 2.5 ≈ 3 points (capped at 50)
```

### Invalid Move
```
Attempted Action: "MOVE_OUTSIDE_BOUNDARY"
Penalty: 15 points (fixed)
```

### Resource Mismanagement
```
Memory Limit: 2048 bytes
Memory Used: 3072 bytes
Excess: 1024 bytes
Penalty: (1024 * 100) / 2048 = 50 points (capped at 40)
```

### Logic Failure
```
Expected: "Array sorted in ascending order"
Actual: "Array not sorted"
Penalty: 30 points (fixed)
```

## 🎯 Usage Example

```cpp
#include "Game.h"
#include "ConcreteChallenges.h"
#include "AIAgent.h"

int main() {
    // Create game
    Game::GameConfig config{"AI Challenge", 3, true};
    Game game(config);

    // Add challenges
    Challenge::ChallengeConfig pathConfig{
        0, 15, 1024, "Pathfinding", "Navigate to goal"
    };
    game.addChallenge(
        std::make_unique<PathfindingChallenge>(pathConfig, 10, 10)
    );

    // Create and test agent
    SimpleGreedyAgent agent;
    auto result = game.playSession(agent);

    std::cout << "Agent: " << result.agentName << std::endl;
    std::cout << "Efficiency: " << result.efficiencyScore << "/100" << std::endl;

    return 0;
}
```

## 🔧 Extensibility Guide

### Adding a New Challenge Type

```cpp
class MyCustomChallenge : public Challenge {
public:
    MyCustomChallenge(const ChallengeConfig& config)
        : Challenge(config) {}

    void initialize() override {
        // Setup challenge state
    }

    bool executeAction(const std::string& action) override {
        incrementStepCount();
        // Parse and execute action
        return isValidMove(action);
    }

    bool isSolved() const override {
        return solved;  // Check completion condition
    }

    std::string getState() const override {
        return "Current puzzle state...";
    }

    std::string getExpectedResult() const override {
        return "Expected solution...";
    }

private:
    bool isValidMove(const std::string& action) const override {
        return true;  // Validation logic
    }
};
```

### Adding a New AI Agent

```cpp
class MyAIAgent : public AIAgent {
public:
    MyAIAgent() : AIAgent("MyAI") {}

    bool solveChallenge(Challenge& challenge,
                       PenaltyScoreboard& scoreboard) override {
        // Implement your algorithm
        // Use challenge.executeAction() for moves
        // scoreboard records penalties automatically

        int penalties = scoreboard.getTotalPenalties();
        recordCompletion(success, challenge.getStepCount(), penalties);
        return success;
    }
};
```

### Customizing Penalty Weights

```cpp
PenaltyScoreboard scoreboard;

// Make algorithmic inefficiency penalties 2x harsh
scoreboard.setPenaltyWeight(
    PenaltyScoreboard::ErrorCategory::ALGORITHMIC_INEFFICIENCY, 2
);

// Make invalid moves 3x harsh
scoreboard.setPenaltyWeight(
    PenaltyScoreboard::ErrorCategory::INVALID_MOVE, 3
);
```

## 📈 Performance Analysis

The framework provides comprehensive performance metrics:
- **Levels Completed**: Success count per session
- **Total Penalties**: Accumulated error points
- **Efficiency Score**: Inverse of penalties (0-100 scale)
- **Penalty Breakdown**: Errors categorized by type
- **Detailed Event Log**: Timestamp and description of each penalty

### Example Report Output
```
========== PENALTY SCOREBOARD REPORT ==========
Total Penalty Points: 45
Efficiency Score: 55/100

--- Breakdown by Category ---
Algorithmic Inefficiency  : 2 errors, 20 points
Invalid Moves             : 1 errors, 15 points
Logic Failures            : 1 errors, 10 points

--- Detailed Event Log ---
[Level 0 @ 120ms] Exceeded step limit (5 points)
[Level 1 @ 450ms] Invalid move attempted (15 points)
...
=============================================
```

## 🔐 Safety Features

- **Smart Pointers**: `std::unique_ptr` prevents memory leaks
- **RAII Principles**: Automatic resource management
- **No Raw Pointers**: Eliminates dangling pointer risks
- **Move Semantics**: Efficient resource transfer
- **STL Containers**: Type-safe, bounds-checked operations

## 📚 Project Files

| File | Purpose |
|------|---------|
| `PenaltyScoreboard.h/cpp` | Error tracking and penalty system |
| `Challenge.h/cpp` | Base class for challenges |
| `ConcreteChallenges.h/cpp` | Pathfinding, Sorting, Resource challenges |
| `AIAgent.h/cpp` | Base class and sample agent implementations |
| `Game.h/cpp` | Main game engine orchestrator |
| `main.cpp` | Entry point and demonstration |
| `CMakeLists.txt` | Build configuration |

## 🚀 Future Enhancement Ideas

1. **Advanced Challenge Types**: Graph problems, constraint satisfaction, machine learning
2. **Distributed Testing**: Run multiple agents in parallel
3. **Replay System**: Record and playback agent decisions
4. **Visualization**: GUI to display agent progress
5. **Machine Learning Integration**: Neural network agents
6. **Performance Profiling**: Detailed time/memory profiling
7. **Benchmark Suite**: Standardized test cases
8. **Tournament Mode**: Multi-agent competitions

## 📝 License

This project is provided as-is for educational and testing purposes.

---

**Last Updated**: 2026-03-24
**C++ Standard**: C++17
**Compiler Requirements**: GCC 7+, Clang 5+, MSVC 2017+

#include "ConcreteChallenges.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cmath>

// ============ PathfindingChallenge ============

PathfindingChallenge::PathfindingChallenge(const ChallengeConfig& config,
                                           int gridWidth, int gridHeight)
    : Challenge(config), gridWidth(gridWidth), gridHeight(gridHeight),
      agentX(0), agentY(0), goalX(gridWidth - 1), goalY(gridHeight - 1),
      obstacles(gridHeight, std::vector<bool>(gridWidth, false)) {
    initialize();
}

void PathfindingChallenge::initialize() {
    agentX = 0;
    agentY = 0;
    goalX = gridWidth - 1;
    goalY = gridHeight - 1;
    generateGrid();
    // Calculate optimal path length (Manhattan distance for simplicity)
    optimalPath.clear();
    optimalPath.push_back({agentX, agentY});
    optimalPath.push_back({goalX, goalY});
}

void PathfindingChallenge::generateGrid() {
    // Reset obstacles
    for (auto& row : obstacles) {
        std::fill(row.begin(), row.end(), false);
    }
    // Add some random obstacles (30% of grid)
    srand(static_cast<unsigned>(time(nullptr)));
    for (int y = 1; y < gridHeight - 1; ++y) {
        for (int x = 1; x < gridWidth - 1; ++x) {
            if ((rand() % 100) < 30) {
                obstacles[y][x] = true;
            }
        }
    }
}

bool PathfindingChallenge::executeAction(const std::string& action) {
    if (!isValidMove(action)) {
        return false;
    }

    stepCount++;

    if (action == "MOVE_UP" && agentY > 0 && !obstacles[agentY - 1][agentX]) {
        agentY--;
    } else if (action == "MOVE_DOWN" && agentY < gridHeight - 1 &&
               !obstacles[agentY + 1][agentX]) {
        agentY++;
    } else if (action == "MOVE_LEFT" && agentX > 0 &&
               !obstacles[agentY][agentX - 1]) {
        agentX--;
    } else if (action == "MOVE_RIGHT" && agentX < gridWidth - 1 &&
               !obstacles[agentY][agentX + 1]) {
        agentX++;
    } else {
        return false;
    }

    if (agentX == goalX && agentY == goalY) {
        solved = true;
    }

    return true;
}

bool PathfindingChallenge::isSolved() const { return solved; }

std::string PathfindingChallenge::getState() const {
    std::ostringstream oss;
    oss << "Position (" << agentX << "," << agentY << ") Goal (" << goalX
        << "," << goalY << ") Distance: "
        << calculateManhattanDistance(agentX, agentY, goalX, goalY) << "\n";
    oss << "Step Count: " << stepCount << "/" << config.maxStepsAllowed
        << "\n";
    return oss.str();
}

std::string PathfindingChallenge::getExpectedResult() const {
    std::ostringstream oss;
    oss << "Reach goal at (" << goalX << "," << goalY << ") in ~"
        << calculateManhattanDistance(0, 0, goalX, goalY) << " steps";
    return oss.str();
}

bool PathfindingChallenge::isValidMove(const std::string& action) const {
    return (action == "MOVE_UP" || action == "MOVE_DOWN" ||
            action == "MOVE_LEFT" || action == "MOVE_RIGHT");
}

int PathfindingChallenge::calculateManhattanDistance(int x1, int y1, int x2,
                                                      int y2) const {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

void PathfindingChallenge::reset() {
    Challenge::reset();
    agentX = 0;
    agentY = 0;
}

// ============ SortingChallenge ============

SortingChallenge::SortingChallenge(const ChallengeConfig& config)
    : Challenge(config), comparisonCount(0), swapCount(0),
      array({64, 34, 25, 12, 22, 11, 90}) {
    initialize();
}

void SortingChallenge::initialize() {
    generateRandomArray(7);
    sortedArray = array;
    std::sort(sortedArray.begin(), sortedArray.end());
    comparisonCount = 0;
    swapCount = 0;
}

void SortingChallenge::generateRandomArray(int size) {
    array.clear();
    array.resize(size);
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % 100;
    }
}

bool SortingChallenge::executeAction(const std::string& action) {
    std::istringstream iss(action);
    std::string cmd;
    iss >> cmd;

    if (!isValidMove(action)) {
        return false;
    }

    stepCount++;

    if (cmd == "COMPARE") {
        int x, y;
        if (iss >> x >> y) {
            comparisonCount++;
            return (x >= 0 && x < (int)array.size() && y >= 0 &&
                    y < (int)array.size());
        }
    } else if (cmd == "SWAP") {
        int x, y;
        if (iss >> x >> y) {
            swapCount++;
            if (x >= 0 && x < (int)array.size() && y >= 0 &&
                y < (int)array.size()) {
                std::swap(array[x], array[y]);
                return true;
            }
        }
    } else if (cmd == "VERIFY") {
        if (isSorted()) {
            solved = true;
            return true;
        }
        return false;
    }

    return false;
}

bool SortingChallenge::isSorted() const {
    for (size_t i = 0; i < array.size() - 1; ++i) {
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

std::string SortingChallenge::getState() const {
    std::ostringstream oss;
    oss << "Array: [";
    for (size_t i = 0; i < array.size(); ++i) {
        oss << array[i];
        if (i < array.size() - 1) oss << ", ";
    }
    oss << "]\n";
    oss << "Comparisons: " << comparisonCount << ", Swaps: " << swapCount
        << "\n";
    oss << "Step Count: " << stepCount << "/" << config.maxStepsAllowed
        << "\n";
    return oss.str();
}

std::string SortingChallenge::getExpectedResult() const {
    std::ostringstream oss;
    oss << "Sorted Array: [";
    for (size_t i = 0; i < sortedArray.size(); ++i) {
        oss << sortedArray[i];
        if (i < sortedArray.size() - 1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}

bool SortingChallenge::isValidMove(const std::string& action) const {
    std::istringstream iss(action);
    std::string cmd;
    iss >> cmd;
    return (cmd == "COMPARE" || cmd == "SWAP" || cmd == "VERIFY");
}

void SortingChallenge::reset() {
    Challenge::reset();
    initialize();
}

// ============ ResourceAllocationChallenge ============

ResourceAllocationChallenge::ResourceAllocationChallenge(
    const ChallengeConfig& config)
    : Challenge(config), totalResourceAvailable(100), resourceAllocated(0),
      optimalScore(0), currentScore(0) {
    initialize();
}

void ResourceAllocationChallenge::initialize() {
    generateTasks(5);
    optimalScore = totalResourceAvailable;  // Perfect allocation
}

void ResourceAllocationChallenge::generateTasks(int numTasks) {
    tasks.clear();
    tasks.reserve(numTasks);
    srand(static_cast<unsigned>(time(nullptr)));

    for (int i = 0; i < numTasks; ++i) {
        Task task;
        task.id = i;
        task.minResource = 5 + (rand() % 10);
        task.maxResource = task.minResource + (rand() % 20);
        task.priority = 1 + (rand() % 5);
        task.currentAllocation = 0;
        tasks.push_back(task);
    }
}

bool ResourceAllocationChallenge::executeAction(const std::string& action) {
    std::istringstream iss(action);
    std::string cmd;
    int taskId, amount;

    if (!isValidMove(action)) {
        return false;
    }

    stepCount++;

    if (iss >> cmd >> taskId >> amount && cmd == "ALLOCATE") {
        if (taskId < 0 || taskId >= (int)tasks.size()) {
            return false;
        }

        Task& task = tasks[taskId];

        // Check constraints
        if (amount < task.minResource || amount > task.maxResource) {
            return false;
        }

        if (resourceAllocated + amount > totalResourceAvailable) {
            return false;
        }

        task.currentAllocation = amount;
        resourceAllocated += amount;
        currentScore = calculateEfficiency();

        // Check if all tasks allocated
        if (resourceAllocated >= totalResourceAvailable * 0.9) {
            solved = true;
        }

        return true;
    }

    return false;
}

bool ResourceAllocationChallenge::isSolved() const { return solved; }

std::string ResourceAllocationChallenge::getState() const {
    std::ostringstream oss;
    oss << "Resource Pool: " << resourceAllocated << "/"
        << totalResourceAvailable << "\n";
    oss << "Tasks:\n";
    for (const auto& task : tasks) {
        oss << "  Task " << task.id << " [" << task.minResource << "-"
            << task.maxResource << "]: " << task.currentAllocation << " (P"
            << task.priority << ")\n";
    }
    return oss.str();
}

std::string ResourceAllocationChallenge::getExpectedResult() const {
    return "Allocate all resources optimally respecting constraints";
}

bool ResourceAllocationChallenge::isValidMove(const std::string& action) const {
    std::istringstream iss(action);
    std::string cmd;
    return (iss >> cmd && cmd == "ALLOCATE");
}

int ResourceAllocationChallenge::calculateEfficiency() const {
    int efficiency = 0;
    for (const auto& task : tasks) {
        if (task.currentAllocation > 0) {
            efficiency += task.currentAllocation * task.priority;
        }
    }
    return efficiency;
}

bool ResourceAllocationChallenge::constraintsSatisfied() const {
    for (const auto& task : tasks) {
        if (task.currentAllocation > 0 &&
            (task.currentAllocation < task.minResource ||
             task.currentAllocation > task.maxResource)) {
            return false;
        }
    }
    return resourceAllocated <= totalResourceAvailable;
}

void ResourceAllocationChallenge::reset() {
    Challenge::reset();
    initialize();
}

/**
 * @file AIGameEnvironment.hpp
 * @brief Complete C++ AI Algorithmic Challenge Game Environment (Single Header)
 *
 * This is a unified, single-header implementation of the game environment.
 * It includes all components: Game Engine, Penalty Scoreboard, Challenges, and AI Agents.
 *
 * Compile with: g++ -std=c++17 -O2 main.cpp -o ai_game
 */

#ifndef AI_GAME_ENVIRONMENT_HPP
#define AI_GAME_ENVIRONMENT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>

// ============================================================================
// PENALTY SCOREBOARD
// ============================================================================

/**
 * @class PenaltyScoreboard
 * @brief Tracks and manages penalties for AI agent errors during gameplay.
 */
class PenaltyScoreboard {
public:
    enum class ErrorCategory {
        ALGORITHMIC_INEFFICIENCY = 0,
        INVALID_MOVE = 1,
        RESOURCE_MISMANAGEMENT = 2,
        LOGIC_FAILURE = 3,
        TIME_LIMIT_EXCEEDED = 4
    };

    struct PenaltyEvent {
        ErrorCategory category;
        int points;
        std::string description;
        long long timestamp;
        int levelId;
    };

    PenaltyScoreboard()
        : totalPenaltyPoints(0),
          gameStartTime(std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count()) {
        for (int i = 0; i < 5; ++i) {
            penaltyWeights[i] = 1;
            penaltiesByCategory[i] = 0;
            errorCountByCategory[i] = 0;
        }
    }

    void recordPenalty(ErrorCategory category, int points,
                      const std::string& description, int levelId) {
        long long currentTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();

        int weightedPoints = applyWeighting(points, category);

        PenaltyEvent event{
            category, weightedPoints, description,
            currentTime - gameStartTime, levelId};

        penaltyHistory.push_back(event);
        penaltiesByCategory[static_cast<int>(category)] += weightedPoints;
        errorCountByCategory[static_cast<int>(category)]++;
        totalPenaltyPoints += weightedPoints;
    }

    void penalizeAlgorithmicInefficiency(int stepsUsed, int stepsAllowed, int levelId) {
        if (stepsUsed > stepsAllowed) {
            int excess = stepsUsed - stepsAllowed;
            int penaltyPoints = std::min(50, (excess * 5) / stepsAllowed);
            std::string description =
                "Algorithmic Inefficiency: " + std::to_string(stepsUsed) +
                " steps used vs " + std::to_string(stepsAllowed) + " allowed";
            recordPenalty(ErrorCategory::ALGORITHMIC_INEFFICIENCY, penaltyPoints,
                         description, levelId);
        }
    }

    void penalizeInvalidMove(const std::string& attemptedAction, int levelId) {
        recordPenalty(ErrorCategory::INVALID_MOVE, 15,
                     "Invalid move: " + attemptedAction, levelId);
    }

    void penalizeResourceMismanagement(size_t memoryUsed, size_t memoryLimit, int levelId) {
        if (memoryUsed > memoryLimit) {
            size_t excess = memoryUsed - memoryLimit;
            int penaltyPoints = std::min(40, static_cast<int>((excess * 100) / memoryLimit));
            std::string description =
                "Memory overuse: " + std::to_string(memoryUsed) + " bytes vs " +
                std::to_string(memoryLimit) + " bytes allowed";
            recordPenalty(ErrorCategory::RESOURCE_MISMANAGEMENT, penaltyPoints,
                         description, levelId);
        }
    }

    void penalizeLogicFailure(const std::string& failureReason,
                             const std::string& expectedResult, int levelId) {
        recordPenalty(ErrorCategory::LOGIC_FAILURE, 30,
                     "Logic failure: " + failureReason + " (Expected: " +
                         expectedResult + ")",
                     levelId);
    }

    int getTotalPenalties() const { return totalPenaltyPoints; }

    int getPenaltiesByCategory(ErrorCategory category) const {
        auto it = penaltiesByCategory.find(static_cast<int>(category));
        return (it != penaltiesByCategory.end()) ? it->second : 0;
    }

    const std::vector<PenaltyEvent>& getAllPenalties() const {
        return penaltyHistory;
    }

    int getErrorCount(ErrorCategory category) const {
        auto it = errorCountByCategory.find(static_cast<int>(category));
        return (it != errorCountByCategory.end()) ? it->second : 0;
    }

    void reset() {
        penaltyHistory.clear();
        penaltiesByCategory.clear();
        errorCountByCategory.clear();
        totalPenaltyPoints = 0;
        gameStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

        for (int i = 0; i < 5; ++i) {
            penaltiesByCategory[i] = 0;
            errorCountByCategory[i] = 0;
        }
    }

    std::string generateReport() const {
        std::ostringstream report;
        report << "\n" << "========== PENALTY SCOREBOARD REPORT ==========" << std::endl;
        report << "Total Penalty Points: " << totalPenaltyPoints << std::endl;
        report << "Efficiency Score: " << getEfficiencyScore() << "/100" << std::endl;
        report << "\n--- Breakdown by Category ---" << std::endl;

        const char* categories[] = {"Algorithmic Inefficiency", "Invalid Moves",
                                    "Resource Mismanagement", "Logic Failures",
                                    "Time Limit Exceeded"};

        for (int i = 0; i < 5; ++i) {
            int points = penaltiesByCategory.at(i);
            int count = errorCountByCategory.at(i);
            if (count > 0) {
                report << std::left << std::setw(30) << categories[i] << ": "
                       << std::setw(3) << count << " errors, " << std::setw(3)
                       << points << " points" << std::endl;
            }
        }

        report << "\n--- Detailed Event Log ---" << std::endl;
        for (const auto& event : penaltyHistory) {
            report << "[Level " << event.levelId << " @ " << event.timestamp
                   << "ms] " << event.description << " (-" << event.points
                   << " pts)" << std::endl;
        }

        report << "=============================================" << std::endl;
        return report.str();
    }

    void setPenaltyWeight(ErrorCategory category, int weight) {
        penaltyWeights[static_cast<int>(category)] = weight;
    }

    int getEfficiencyScore() const {
        int score = 100 - std::min(100, totalPenaltyPoints);
        return std::max(0, score);
    }

private:
    std::vector<PenaltyEvent> penaltyHistory;
    std::unordered_map<int, int> penaltiesByCategory;
    std::unordered_map<int, int> errorCountByCategory;
    std::unordered_map<int, int> penaltyWeights;
    int totalPenaltyPoints;
    long long gameStartTime;

    int applyWeighting(int basePenalty, ErrorCategory category) const {
        int weight = penaltyWeights.at(static_cast<int>(category));
        return basePenalty * weight;
    }
};

// ============================================================================
// CHALLENGE BASE CLASS
// ============================================================================

/**
 * @class Challenge
 * @brief Abstract base class for algorithmic challenges.
 */
class Challenge {
public:
    struct ChallengeConfig {
        int levelId;
        int maxStepsAllowed;
        size_t maxMemoryBytes;
        std::string name;
        std::string description;
    };

    explicit Challenge(const ChallengeConfig& config)
        : config(config), stepCount(0), memoryUsage(0), solved(false) {}

    virtual ~Challenge() = default;

    Challenge(const Challenge&) = delete;
    Challenge& operator=(const Challenge&) = delete;
    Challenge(Challenge&&) noexcept = default;
    Challenge& operator=(Challenge&&) noexcept = default;

    virtual void initialize() = 0;
    virtual bool executeAction(const std::string& action) = 0;
    virtual bool isSolved() const = 0;
    virtual std::string getState() const = 0;
    virtual std::string getExpectedResult() const = 0;

    void incrementStepCount() { stepCount++; }
    bool isStepLimitExceeded() const { return stepCount > config.maxStepsAllowed; }
    int getStepCount() const { return stepCount; }
    int getMaxStepsAllowed() const { return config.maxStepsAllowed; }

    void recordMemoryUsage(size_t bytes) { memoryUsage = bytes; }
    size_t getMemoryUsage() const { return memoryUsage; }
    size_t getMemoryLimit() const { return config.maxMemoryBytes; }
    bool isMemoryLimitExceeded() const { return memoryUsage > config.maxMemoryBytes; }

    const ChallengeConfig& getConfig() const { return config; }

    std::string getDescription() const { return config.description; }

    virtual void reset() {
        stepCount = 0;
        memoryUsage = 0;
        solved = false;
        initialize();
    }

protected:
    ChallengeConfig config;
    int stepCount;
    size_t memoryUsage;
    bool solved;

    virtual bool isValidMove(const std::string& action) const = 0;
};

// ============================================================================
// CONCRETE CHALLENGES
// ============================================================================

class PathfindingChallenge : public Challenge {
public:
    PathfindingChallenge(const ChallengeConfig& config, int gridWidth, int gridHeight)
        : Challenge(config), gridWidth(gridWidth), gridHeight(gridHeight),
          agentX(0), agentY(0), goalX(gridWidth - 1), goalY(gridHeight - 1),
          obstacles(gridHeight, std::vector<bool>(gridWidth, false)) {
        initialize();
    }

    void initialize() override {
        agentX = 0;
        agentY = 0;
        goalX = gridWidth - 1;
        goalY = gridHeight - 1;
        generateGrid();
    }

    bool executeAction(const std::string& action) override {
        if (!isValidMove(action)) return false;
        stepCount++;

        if (action == "MOVE_UP" && agentY > 0 && !obstacles[agentY - 1][agentX]) {
            agentY--;
        } else if (action == "MOVE_DOWN" && agentY < gridHeight - 1 && !obstacles[agentY + 1][agentX]) {
            agentY++;
        } else if (action == "MOVE_LEFT" && agentX > 0 && !obstacles[agentY][agentX - 1]) {
            agentX--;
        } else if (action == "MOVE_RIGHT" && agentX < gridWidth - 1 && !obstacles[agentY][agentX + 1]) {
            agentX++;
        } else {
            return false;
        }

        if (agentX == goalX && agentY == goalY) solved = true;
        return true;
    }

    bool isSolved() const override { return solved; }

    std::string getState() const override {
        std::ostringstream oss;
        oss << "Position (" << agentX << "," << agentY << ") Goal (" << goalX << "," << goalY
            << ") Distance: " << calculateManhattanDistance(agentX, agentY, goalX, goalY)
            << "\nStep Count: " << stepCount << "/" << config.maxStepsAllowed << "\n";
        return oss.str();
    }

    std::string getExpectedResult() const override {
        std::ostringstream oss;
        oss << "Reach goal at (" << goalX << "," << goalY << ") in ~"
            << calculateManhattanDistance(0, 0, goalX, goalY) << " steps";
        return oss.str();
    }

    void reset() override {
        Challenge::reset();
        agentX = 0;
        agentY = 0;
    }

private:
    int gridWidth, gridHeight, agentX, agentY, goalX, goalY;
    std::vector<std::vector<bool>> obstacles;

    bool isValidMove(const std::string& action) const override {
        return (action == "MOVE_UP" || action == "MOVE_DOWN" ||
                action == "MOVE_LEFT" || action == "MOVE_RIGHT");
    }

    void generateGrid() {
        for (auto& row : obstacles) std::fill(row.begin(), row.end(), false);
        srand(static_cast<unsigned>(time(nullptr)));
        for (int y = 1; y < gridHeight - 1; ++y) {
            for (int x = 1; x < gridWidth - 1; ++x) {
                if ((rand() % 100) < 30) obstacles[y][x] = true;
            }
        }
    }

    int calculateManhattanDistance(int x1, int y1, int x2, int y2) const {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }
};

class SortingChallenge : public Challenge {
public:
    explicit SortingChallenge(const ChallengeConfig& config)
        : Challenge(config), comparisonCount(0), swapCount(0),
          array({64, 34, 25, 12, 22, 11, 90}) {
        initialize();
    }

    void initialize() override {
        generateRandomArray(7);
        sortedArray = array;
        std::sort(sortedArray.begin(), sortedArray.end());
        comparisonCount = 0;
        swapCount = 0;
    }

    bool executeAction(const std::string& action) override {
        std::istringstream iss(action);
        std::string cmd;
        iss >> cmd;

        if (!isValidMove(action)) return false;
        stepCount++;

        if (cmd == "COMPARE") {
            int x, y;
            if (iss >> x >> y) {
                comparisonCount++;
                return (x >= 0 && x < (int)array.size() && y >= 0 && y < (int)array.size());
            }
        } else if (cmd == "SWAP") {
            int x, y;
            if (iss >> x >> y) {
                swapCount++;
                if (x >= 0 && x < (int)array.size() && y >= 0 && y < (int)array.size()) {
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

    bool isSolved() const override { return solved; }

    std::string getState() const override {
        std::ostringstream oss;
        oss << "Array: [";
        for (size_t i = 0; i < array.size(); ++i) {
            oss << array[i];
            if (i < array.size() - 1) oss << ", ";
        }
        oss << "]\nComparisons: " << comparisonCount << ", Swaps: " << swapCount
            << "\nStep Count: " << stepCount << "/" << config.maxStepsAllowed << "\n";
        return oss.str();
    }

    std::string getExpectedResult() const override {
        std::ostringstream oss;
        oss << "Sorted Array: [";
        for (size_t i = 0; i < sortedArray.size(); ++i) {
            oss << sortedArray[i];
            if (i < sortedArray.size() - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    void reset() override {
        Challenge::reset();
        initialize();
    }

private:
    std::vector<int> array, sortedArray;
    int comparisonCount, swapCount;

    bool isValidMove(const std::string& action) const override {
        std::istringstream iss(action);
        std::string cmd;
        iss >> cmd;
        return (cmd == "COMPARE" || cmd == "SWAP" || cmd == "VERIFY");
    }

    bool isSorted() const {
        for (size_t i = 0; i < array.size() - 1; ++i) {
            if (array[i] > array[i + 1]) return false;
        }
        return true;
    }

    void generateRandomArray(int size) {
        array.clear();
        array.resize(size);
        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < size; ++i) {
            array[i] = rand() % 100;
        }
    }
};

class ResourceAllocationChallenge : public Challenge {
public:
    struct Task {
        int id, minResource, maxResource, priority, currentAllocation;
    };

    explicit ResourceAllocationChallenge(const ChallengeConfig& config)
        : Challenge(config), totalResourceAvailable(100), resourceAllocated(0),
          optimalScore(0), currentScore(0) {
        initialize();
    }

    void initialize() override {
        generateTasks(5);
        optimalScore = totalResourceAvailable;
    }

    bool executeAction(const std::string& action) override {
        std::istringstream iss(action);
        std::string cmd;
        int taskId, amount;

        if (!isValidMove(action)) return false;
        stepCount++;

        if (iss >> cmd >> taskId >> amount && cmd == "ALLOCATE") {
            if (taskId < 0 || taskId >= (int)tasks.size()) return false;

            Task& task = tasks[taskId];
            if (amount < task.minResource || amount > task.maxResource) return false;
            if (resourceAllocated + amount > totalResourceAvailable) return false;

            task.currentAllocation = amount;
            resourceAllocated += amount;
            currentScore = calculateEfficiency();

            if (resourceAllocated >= totalResourceAvailable * 0.9) solved = true;
            return true;
        }
        return false;
    }

    bool isSolved() const override { return solved; }

    std::string getState() const override {
        std::ostringstream oss;
        oss << "Resource Pool: " << resourceAllocated << "/" << totalResourceAvailable << "\nTasks:\n";
        for (const auto& task : tasks) {
            oss << "  Task " << task.id << " [" << task.minResource << "-"
                << task.maxResource << "]: " << task.currentAllocation << " (P"
                << task.priority << ")\n";
        }
        return oss.str();
    }

    std::string getExpectedResult() const override {
        return "Allocate all resources optimally respecting constraints";
    }

    void reset() override {
        Challenge::reset();
        initialize();
    }

private:
    std::vector<Task> tasks;
    int totalResourceAvailable, resourceAllocated, optimalScore, currentScore;

    bool isValidMove(const std::string& action) const override {
        std::istringstream iss(action);
        std::string cmd;
        return (iss >> cmd && cmd == "ALLOCATE");
    }

    void generateTasks(int numTasks) {
        tasks.clear();
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

    int calculateEfficiency() const {
        int efficiency = 0;
        for (const auto& task : tasks) {
            if (task.currentAllocation > 0) efficiency += task.currentAllocation * task.priority;
        }
        return efficiency;
    }
};

// ============================================================================
// AI AGENT BASE CLASS
// ============================================================================

class AIAgent {
public:
    struct AgentStats {
        int levelsCompleted, levelsFailed, totalStepsUsed, totalPenalties;
        double averageEfficiency;
        std::string agentName;
    };

    explicit AIAgent(const std::string& name)
        : agentName(name), levelsCompleted(0), levelsFailed(0),
          totalStepsUsed(0), totalPenalties(0) {}

    virtual ~AIAgent() = default;

    virtual bool solveChallenge(Challenge& challenge, PenaltyScoreboard& scoreboard) = 0;

    const std::string& getName() const { return agentName; }

    AgentStats getStats() const {
        return AgentStats{levelsCompleted,
                          levelsFailed,
                          totalStepsUsed,
                          totalPenalties,
                          (levelsCompleted + levelsFailed > 0)
                              ? 100.0 - (totalPenalties / (double)(levelsCompleted + levelsFailed))
                              : 0.0,
                          agentName};
    }

    void resetStats() {
        levelsCompleted = 0;
        levelsFailed = 0;
        totalStepsUsed = 0;
        totalPenalties = 0;
        levelPenalties.clear();
    }

    void recordCompletion(bool success, int stepsUsed, int penaltyPoints) {
        if (success) levelsCompleted++;
        else levelsFailed++;
        totalStepsUsed += stepsUsed;
        totalPenalties += penaltyPoints;
        levelPenalties.push_back(penaltyPoints);
    }

    void printPerformanceSummary() const {
        AgentStats stats = getStats();
        std::cout << "\n========== AGENT PERFORMANCE SUMMARY ==========" << std::endl;
        std::cout << "Agent: " << stats.agentName << std::endl;
        std::cout << "Levels Completed: " << stats.levelsCompleted << std::endl;
        std::cout << "Levels Failed: " << stats.levelsFailed << std::endl;
        std::cout << "Total Steps Used: " << stats.totalStepsUsed << std::endl;
        std::cout << "Total Penalties: " << stats.totalPenalties << std::endl;
        std::cout << "Average Efficiency: " << std::fixed << std::setprecision(2)
                  << stats.averageEfficiency << "/100" << std::endl;
        std::cout << "============================================" << std::endl;
    }

protected:
    std::string agentName;
    int levelsCompleted, levelsFailed, totalStepsUsed, totalPenalties;
    std::vector<int> levelPenalties;
};

class SimpleGreedyAgent : public AIAgent {
public:
    SimpleGreedyAgent() : AIAgent("SimpleGreedyAgent") {}

    bool solveChallenge(Challenge& challenge, PenaltyScoreboard& scoreboard) override {
        bool success = false;
        if (challenge.getConfig().name.find("Pathfinding") != std::string::npos) {
            success = solvePathfinding(challenge);
        } else if (challenge.getConfig().name.find("Sorting") != std::string::npos) {
            success = solveSorting(challenge);
        } else if (challenge.getConfig().name.find("Resource") != std::string::npos) {
            success = solveResourceAllocation(challenge);
        }

        int penalties = scoreboard.getTotalPenalties();
        recordCompletion(success, challenge.getStepCount(), penalties);
        return success;
    }

private:
    bool solvePathfinding(Challenge& challenge) {
        int maxSteps = challenge.getMaxStepsAllowed();
        while (!challenge.isSolved() && challenge.getStepCount() < maxSteps) {
            challenge.executeAction("MOVE_RIGHT");
        }
        return challenge.isSolved();
    }

    bool solveSorting(Challenge& challenge) {
        int maxSteps = challenge.getMaxStepsAllowed();
        int actionCount = 0;
        while (!challenge.isSolved() && actionCount < maxSteps) {
            challenge.executeAction("COMPARE 0 1");
            actionCount++;
            if (!challenge.isSolved()) challenge.executeAction("SWAP 0 1");
            actionCount++;
            if (!challenge.isSolved()) challenge.executeAction("VERIFY");
            actionCount++;
            if (challenge.isSolved()) break;
            if (actionCount >= maxSteps / 2) {
                challenge.executeAction("VERIFY");
                actionCount++;
            }
        }
        return challenge.isSolved();
    }

    bool solveResourceAllocation(Challenge& challenge) {
        for (int task = 0; task < 5 && challenge.getStepCount() < challenge.getMaxStepsAllowed(); ++task) {
            std::string action = "ALLOCATE " + std::to_string(task) + " 20";
            challenge.executeAction(action);
        }
        return challenge.isSolved();
    }
};

class OptimizedAgent : public AIAgent {
public:
    OptimizedAgent() : AIAgent("OptimizedAgent") {}

    bool solveChallenge(Challenge& challenge, PenaltyScoreboard& scoreboard) override {
        bool success = false;
        if (challenge.getConfig().name.find("Pathfinding") != std::string::npos) {
            success = solvePathfinding(challenge);
        } else if (challenge.getConfig().name.find("Sorting") != std::string::npos) {
            success = solveSorting(challenge);
        } else if (challenge.getConfig().name.find("Resource") != std::string::npos) {
            success = solveResourceAllocation(challenge);
        }

        int penalties = scoreboard.getTotalPenalties();
        recordCompletion(success, challenge.getStepCount(), penalties);
        return success;
    }

private:
    bool solvePathfinding(Challenge& challenge) {
        std::string moves[] = {"MOVE_RIGHT", "MOVE_DOWN", "MOVE_RIGHT", "MOVE_RIGHT"};
        for (const auto& move : moves) {
            if (challenge.getStepCount() >= challenge.getMaxStepsAllowed() || challenge.isSolved())
                break;
            challenge.executeAction(move);
        }
        return challenge.isSolved();
    }

    bool solveSorting(Challenge& challenge) {
        int compareSwapCycles = 0;
        while (!challenge.isSolved() && compareSwapCycles < challenge.getMaxStepsAllowed() / 3) {
            challenge.executeAction("COMPARE 0 1");
            challenge.executeAction("SWAP 0 1");
            challenge.executeAction("COMPARE 1 2");
            compareSwapCycles++;
        }
        challenge.executeAction("VERIFY");
        return challenge.isSolved();
    }

    bool solveResourceAllocation(Challenge& challenge) {
        std::vector<std::pair<int, int>> allocations = {
            {0, 20}, {1, 25}, {2, 20}, {3, 18}, {4, 15}};
        for (const auto& alloc : allocations) {
            std::string action = "ALLOCATE " + std::to_string(alloc.first) + " " +
                                std::to_string(alloc.second);
            challenge.executeAction(action);
        }
        return challenge.isSolved();
    }
};

// ============================================================================
// GAME ENGINE
// ============================================================================

class Game {
public:
    struct GameConfig {
        std::string gameTitle;
        int totalLevels;
        bool verboseLogging;
    };

    struct SessionResult {
        std::string agentName;
        int levelsCompleted, levelsFailed, totalPenalties;
        double efficiencyScore;
    };

    explicit Game(const GameConfig& config)
        : config(config), currentLevel(0) {}

    void addChallenge(std::unique_ptr<Challenge> challenge) {
        challenges.push_back(std::move(challenge));
    }

    SessionResult playSession(AIAgent& agent) {
        agent.resetStats();
        scoreboard.reset();
        currentLevel = 0;

        if (config.verboseLogging) {
            std::cout << "\n========== GAME SESSION START ==========" << std::endl;
            std::cout << "Game: " << config.gameTitle << std::endl;
            std::cout << "Agent: " << agent.getName() << std::endl;
            std::cout << "Total Levels: " << challenges.size() << std::endl;
            std::cout << "=======================================" << std::endl;
        }

        for (size_t i = 0; i < challenges.size(); ++i) {
            currentLevel = i;
            bool success = playLevel(i, agent);

            if (config.verboseLogging) {
                printLevelState(*challenges[i]);
                std::cout << (success ? "✓ LEVEL PASSED" : "✗ LEVEL FAILED") << std::endl << std::endl;
            }
        }

        if (config.verboseLogging) {
            std::cout << scoreboard.generateReport() << std::endl;
            agent.printPerformanceSummary();
        }

        AIAgent::AgentStats stats = agent.getStats();
        return SessionResult{agent.getName(), stats.levelsCompleted, stats.levelsFailed,
                            stats.totalPenalties, stats.averageEfficiency};
    }

    bool playLevel(int levelIndex, AIAgent& agent) {
        if (levelIndex < 0 || levelIndex >= (int)challenges.size()) return false;

        Challenge& challenge = *challenges[levelIndex];
        challenge.reset();

        bool success = agent.solveChallenge(challenge, scoreboard);

        if (success && challenge.isSolved()) {
            if (challenge.isStepLimitExceeded()) {
                scoreboard.penalizeAlgorithmicInefficiency(
                    challenge.getStepCount(), challenge.getMaxStepsAllowed(), levelIndex);
                success = false;
            }
            if (challenge.isMemoryLimitExceeded()) {
                scoreboard.penalizeResourceMismanagement(
                    challenge.getMemoryUsage(), challenge.getMemoryLimit(), levelIndex);
                success = false;
            }
        } else {
            scoreboard.penalizeLogicFailure("Challenge not solved",
                                           challenge.getExpectedResult(), levelIndex);
            success = false;
        }

        return success;
    }

    void printGameInfo() const {
        std::cout << "\n========== GAME INFORMATION ==========" << std::endl;
        std::cout << "Title: " << config.gameTitle << std::endl;
        std::cout << "Total Levels: " << challenges.size() << std::endl << std::endl;

        for (size_t i = 0; i < challenges.size(); ++i) {
            const Challenge& challenge = *challenges[i];
            std::cout << "Level " << (i + 1) << ": " << challenge.getConfig().name << std::endl;
            std::cout << "  Description: " << challenge.getDescription() << std::endl;
            std::cout << "  Max Steps: " << challenge.getMaxStepsAllowed() << std::endl;
            std::cout << "  Max Memory: " << challenge.getMemoryLimit() << " bytes" << std::endl << std::endl;
        }

        std::cout << "=====================================" << std::endl;
    }

private:
    GameConfig config;
    std::vector<std::unique_ptr<Challenge>> challenges;
    PenaltyScoreboard scoreboard;
    int currentLevel;

    void printLevelState(const Challenge& challenge) const {
        std::cout << "\nLevel State:" << std::endl;
        std::cout << challenge.getState();
        std::cout << "Expected: " << challenge.getExpectedResult() << std::endl;
    }
};

#endif // AI_GAME_ENVIRONMENT_HPP

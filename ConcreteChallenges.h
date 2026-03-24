#ifndef CONCRETE_CHALLENGES_H
#define CONCRETE_CHALLENGES_H

#include "Challenge.h"
#include <vector>
#include <set>
#include <queue>

/**
 * @class PathfindingChallenge
 * @brief Grid-based pathfinding challenge (A* or similar optimal pathfinding)
 *
 * The AI must find the shortest path from start to goal on a grid with obstacles.
 * Actions: "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT"
 * Penalty: Steps exceeding optimal path length
 */
class PathfindingChallenge : public Challenge {
public:
    PathfindingChallenge(const ChallengeConfig& config, int gridWidth,
                         int gridHeight);

    void initialize() override;
    bool executeAction(const std::string& action) override;
    bool isSolved() const override;
    std::string getState() const override;
    std::string getExpectedResult() const override;
    void reset() override;

private:
    int gridWidth, gridHeight;
    int agentX, agentY;
    int goalX, goalY;
    std::vector<std::vector<bool>> obstacles;  // true = obstacle
    std::vector<std::pair<int, int>> optimalPath;

    bool isValidMove(const std::string& action) const override;
    void generateGrid();
    int calculateManhattanDistance(int x1, int y1, int x2, int y2) const;
};

/**
 * @class SortingChallenge
 * @brief Array sorting challenge
 *
 * The AI must sort an array using a minimum number of comparisons/swaps.
 * Actions: "COMPARE x y", "SWAP x y", "VERIFY"
 * Penalty: Exceeding maximum allowed operations
 */
class SortingChallenge : public Challenge {
public:
    explicit SortingChallenge(const ChallengeConfig& config);

    void initialize() override;
    bool executeAction(const std::string& action) override;
    bool isSolved() const override;
    std::string getState() const override;
    std::string getExpectedResult() const override;
    void reset() override;

private:
    std::vector<int> array;
    std::vector<int> sortedArray;
    int comparisonCount;
    int swapCount;

    bool isValidMove(const std::string& action) const override;
    bool isSorted() const;
    void generateRandomArray(int size);
};

/**
 * @class ResourceAllocationChallenge
 * @brief Optimal resource allocation challenge
 *
 * Distribute resources (memory, CPU, bandwidth) across tasks to maximize
 * efficiency while meeting constraints.
 * Actions: "ALLOCATE task_id resource_amount"
 * Penalty: Suboptimal allocation or constraint violations
 */
class ResourceAllocationChallenge : public Challenge {
public:
    explicit ResourceAllocationChallenge(const ChallengeConfig& config);

    void initialize() override;
    bool executeAction(const std::string& action) override;
    bool isSolved() const override;
    std::string getState() const override;
    std::string getExpectedResult() const override;
    void reset() override;

private:
    struct Task {
        int id;
        int minResource;
        int maxResource;
        int priority;
        int currentAllocation;
    };

    std::vector<Task> tasks;
    int totalResourceAvailable;
    int resourceAllocated;
    int optimalScore;
    int currentScore;

    bool isValidMove(const std::string& action) const override;
    void generateTasks(int numTasks);
    int calculateEfficiency() const;
    bool constraintsSatisfied() const;
};

#endif // CONCRETE_CHALLENGES_H

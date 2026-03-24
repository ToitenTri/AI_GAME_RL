#ifndef AI_AGENT_H
#define AI_AGENT_H

#include "Challenge.h"
#include "PenaltyScoreboard.h"
#include <string>
#include <vector>
#include <memory>

/**
 * @class AIAgent
 * @brief Abstract base class for AI agents solving challenges.
 *
 * This interface allows different AI strategies to be plugged in and tested.
 * Implementations must override solveChallenge() to provide their specific
 * algorithm for solving a given challenge type.
 *
 * The framework handles penalty tracking automatically.
 */
class AIAgent {
public:
    struct AgentStats {
        int levelsCompleted;
        int levelsFailed;
        int totalStepsUsed;
        int totalPenalties;
        double averageEfficiency;
        std::string agentName;
    };

    explicit AIAgent(const std::string& name);
    virtual ~AIAgent() = default;

    /**
     * @brief Solve a given challenge
     * @param challenge Reference to the challenge to solve
     * @param scoreboard Reference to penalty tracker
     * @return true if challenge solved successfully, false otherwise
     */
    virtual bool solveChallenge(Challenge& challenge,
                                PenaltyScoreboard& scoreboard) = 0;

    /**
     * @brief Get agent name
     */
    const std::string& getName() const;

    /**
     * @brief Get agent statistics
     */
    AgentStats getStats() const;

    /**
     * @brief Reset agent statistics for new game session
     */
    void resetStats();

    /**
     * @brief Record challenge completion
     */
    void recordCompletion(bool success, int stepsUsed, int penaltyPoints);

    /**
     * @brief Print agent performance summary
     */
    void printPerformanceSummary() const;

protected:
    std::string agentName;
    int levelsCompleted;
    int levelsFailed;
    int totalStepsUsed;
    int totalPenalties;
    std::vector<int> levelPenalties;

    /**
     * @brief Log action for debugging/analysis
     */
    void logAction(const std::string& action, bool successful);
};

/**
 * @class SimpleGreedyAgent
 * @brief Sample AI agent using simple greedy/heuristic strategies
 *
 * This agent provides a reference implementation showing how to:
 * - Parse challenge state
 * - Execute actions
 * - Handle exceptions and invalid moves
 * - Track step efficiency
 */
class SimpleGreedyAgent : public AIAgent {
public:
    SimpleGreedyAgent();

    bool solveChallenge(Challenge& challenge,
                        PenaltyScoreboard& scoreboard) override;

private:
    /**
     * @brief Solve pathfinding challenge using greedy approach
     */
    bool solvePathfinding(Challenge& challenge, PenaltyScoreboard& scoreboard);

    /**
     * @brief Solve sorting challenge using basic sort algorithm
     */
    bool solveSorting(Challenge& challenge, PenaltyScoreboard& scoreboard);

    /**
     * @brief Solve resource allocation greedily
     */
    bool solveResourceAllocation(Challenge& challenge,
                                  PenaltyScoreboard& scoreboard);
};

/**
 * @class OptimizedAgent
 * @brief Sample AI agent using more sophisticated algorithms
 *
 * Demonstrates advanced techniques:
 * - Memoization and caching
 * - Optimal decision making
 * - Memory-efficient approaches
 */
class OptimizedAgent : public AIAgent {
public:
    OptimizedAgent();

    bool solveChallenge(Challenge& challenge,
                        PenaltyScoreboard& scoreboard) override;

private:
    bool solvePathfinding(Challenge& challenge, PenaltyScoreboard& scoreboard);
    bool solveSorting(Challenge& challenge, PenaltyScoreboard& scoreboard);
    bool solveResourceAllocation(Challenge& challenge,
                                  PenaltyScoreboard& scoreboard);
};

#endif // AI_AGENT_H

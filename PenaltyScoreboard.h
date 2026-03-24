#ifndef PENALTY_SCOREBOARD_H
#define PENALTY_SCOREBOARD_H

#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>

/**
 * @class PenaltyScoreboard
 * @brief Tracks and manages penalties for AI agent errors during gameplay.
 *
 * This class maintains comprehensive error tracking across multiple categories:
 * - Algorithmic inefficiency (exceeding step/time limits)
 * - Invalid moves (actions violating game rules)
 * - Resource mismanagement (memory limit violations)
 * - Logic failures (incorrect outputs/conditions)
 */
class PenaltyScoreboard {
public:
    // Penalty categories
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
        long long timestamp;  // milliseconds since game start
        int levelId;
    };

    PenaltyScoreboard();
    ~PenaltyScoreboard() = default;

    /**
     * @brief Records a penalty event
     * @param category The type of error
     * @param points Penalty points to deduct
     * @param description Human-readable description of the error
     * @param levelId Current level ID
     */
    void recordPenalty(ErrorCategory category, int points,
                      const std::string& description, int levelId);

    /**
     * @brief Records inefficiency penalty when step count exceeds threshold
     * @param stepsUsed Actual steps taken by AI
     * @param stepsAllowed Maximum allowed steps
     * @param levelId Current level ID
     */
    void penalizeAlgorithmicInefficiency(int stepsUsed, int stepsAllowed, int levelId);

    /**
     * @brief Records penalty for invalid move attempts
     * @param attemptedAction Description of invalid action
     * @param levelId Current level ID
     */
    void penalizeInvalidMove(const std::string& attemptedAction, int levelId);

    /**
     * @brief Records penalty for exceeding memory limits
     * @param memoryUsed Amount of memory used (in bytes)
     * @param memoryLimit Memory limit (in bytes)
     * @param levelId Current level ID
     */
    void penalizeResourceMismanagement(size_t memoryUsed,
                                       size_t memoryLimit, int levelId);

    /**
     * @brief Records penalty for logic failures or incorrect outputs
     * @param failureReason Description of the logic failure
     * @param expectedResult Expected output/condition
     * @param levelId Current level ID
     */
    void penalizeLogicFailure(const std::string& failureReason,
                             const std::string& expectedResult, int levelId);

    /**
     * @brief Get total accumulated penalty points
     */
    int getTotalPenalties() const;

    /**
     * @brief Get penalty points for a specific category
     */
    int getPenaltiesByCategory(ErrorCategory category) const;

    /**
     * @brief Get all penalty events recorded
     */
    const std::vector<PenaltyEvent>& getAllPenalties() const;

    /**
     * @brief Get penalty count by category
     */
    int getErrorCount(ErrorCategory category) const;

    /**
     * @brief Reset scoreboard (for new game session)
     */
    void reset();

    /**
     * @brief Generate human-readable penalty report
     */
    std::string generateReport() const;

    /**
     * @brief Set custom penalty weights for different error types
     */
    void setPenaltyWeight(ErrorCategory category, int weight);

    /**
     * @brief Calculate efficiency score (inverse of penalties)
     * Maximum 100 points, reduced by penalties
     */
    int getEfficiencyScore() const;

private:
    std::vector<PenaltyEvent> penaltyHistory;
    std::unordered_map<int, int> penaltiesByCategory;  // ErrorCategory -> total points
    std::unordered_map<int, int> errorCountByCategory;  // ErrorCategory -> error count
    std::unordered_map<int, int> penaltyWeights;        // ErrorCategory -> weight multiplier
    int totalPenaltyPoints;
    long long gameStartTime;

    /**
     * @brief Apply weight multiplier to penalty points
     */
    int applyWeighting(int basePenalty, ErrorCategory category) const;
};

#endif // PENALTY_SCOREBOARD_H

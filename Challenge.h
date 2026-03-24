#ifndef CHALLENGE_H
#define CHALLENGE_H

#include "PenaltyScoreboard.h"
#include <string>
#include <vector>
#include <memory>

/**
 * @class Challenge
 * @brief Abstract base class for algorithmic challenges in the game.
 *
 * Each challenge represents a specific algorithmic problem that the AI agent
 * must solve. Subclasses implement specific challenge types (pathfinding,
 * sorting, resource optimization, etc.).
 *
 * The Challenge class enforces:
 * - Step counting to detect inefficiency
 * - Memory limits to prevent resource misuse
 * - Result validation against expected outcomes
 * - Invalid move detection and penalization
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

    explicit Challenge(const ChallengeConfig& config);
    virtual ~Challenge();

    // Disable copy, allow move
    Challenge(const Challenge&) = delete;
    Challenge& operator=(const Challenge&) = delete;
    Challenge(Challenge&&) noexcept = default;
    Challenge& operator=(Challenge&&) noexcept = default;

    /**
     * @brief Initialize challenge state and reset step counter
     */
    virtual void initialize() = 0;

    /**
     * @brief Validate and execute an agent's action
     * @param action The action attempt (format depends on challenge type)
     * @return true if action is valid and executed, false if invalid
     */
    virtual bool executeAction(const std::string& action) = 0;

    /**
     * @brief Check if challenge is solved/completed
     */
    virtual bool isSolved() const = 0;

    /**
     * @brief Get current state of the challenge (for AI agent observation)
     */
    virtual std::string getState() const = 0;

    /**
     * @brief Get expected solution output for validation
     */
    virtual std::string getExpectedResult() const = 0;

    /**
     * @brief Increment step counter (called by executing agent)
     */
    void incrementStepCount();

    /**
     * @brief Check if step limit exceeded
     */
    bool isStepLimitExceeded() const;

    /**
     * @brief Get current step count
     */
    int getStepCount() const;

    /**
     * @brief Get allowed step count
     */
    int getMaxStepsAllowed() const;

    /**
     * @brief Update memory usage tracking
     */
    void recordMemoryUsage(size_t bytes);

    /**
     * @brief Get current memory usage
     */
    size_t getMemoryUsage() const;

    /**
     * @brief Get memory limit
     */
    size_t getMemoryLimit() const;

    /**
     * @brief Check if memory limit exceeded
     */
    bool isMemoryLimitExceeded() const;

    /**
     * @brief Get challenge configuration
     */
    const ChallengeConfig& getConfig() const;

    /**
     * @brief Generate challenge description for display
     */
    virtual std::string getDescription() const;

    /**
     * @brief Reset challenge state for retry
     */
    virtual void reset();

protected:
    ChallengeConfig config;
    int stepCount;
    size_t memoryUsage;
    bool solved;

    /**
     * @brief Validate an action is inside game rules (abstract)
     * Called before executeAction to check move legitimacy
     */
    virtual bool isValidMove(const std::string& action) const = 0;
};

#endif // CHALLENGE_H

#ifndef GAME_H
#define GAME_H

#include "Challenge.h"
#include "AIAgent.h"
#include "PenaltyScoreboard.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @class Game
 * @brief Main game engine orchestrating challenges and AI agents
 *
 * The Game class manages:
 * - Challenge sequence and progression
 * - AI agent interaction with challenges
 * - Penalty tracking and scoring
 * - Game state and level transitions
 * - Session results and reporting
 *
 * Architecture ensures clean separation of concerns:
 * - Game doesn't know implementation details of challenges
 * - Challenges are agnostic to AI type
 * - Penalty system is independent of game logic
 */
class Game {
public:
    struct GameConfig {
        std::string gameTitle;
        int totalLevels;
        bool verboseLogging;
    };

    struct SessionResult {
        std::string agentName;
        int levelsCompleted;
        int levelsFailed;
        int totalPenalties;
        double efficiencyScore;
        std::vector<std::string> levelReports;
    };

    explicit Game(const GameConfig& config);
    ~Game() = default;

    /**
     * @brief Add a challenge to the game
     * @param challenge Unique pointer to challenge instance
     */
    void addChallenge(std::unique_ptr<Challenge> challenge);

    /**
     * @brief Run game session with given AI agent
     * @param agent Reference to AI agent to test
     * @return Session results and scoring
     */
    SessionResult playSession(AIAgent& agent);

    /**
     * @brief Run a single level
     * @param levelIndex Index of level to run
     * @param agent Reference to AI agent
     * @return true if level was completed successfully
     */
    bool playLevel(int levelIndex, AIAgent& agent);

    /**
     * @brief Get number of challenges/levels
     */
    int getNumChallenges() const;

    /**
     * @brief Get challenge at index
     */
    Challenge* getChallenge(int index);

    /**
     * @brief Print game information
     */
    void printGameInfo() const;

    /**
     * @brief Get current session penalty scoreboard
     */
    const PenaltyScoreboard& getScoreboard() const;

private:
    GameConfig config;
    std::vector<std::unique_ptr<Challenge>> challenges;
    PenaltyScoreboard scoreboard;
    int currentLevel;

    /**
     * @brief Log level result
     */
    void logLevelResult(int levelIndex, bool success, const Challenge& challenge);

    /**
     * @brief Print level state for debugging
     */
    void printLevelState(const Challenge& challenge) const;
};

#endif // GAME_H

#include "Game.h"
#include <iostream>
#include <iomanip>

Game::Game(const GameConfig& config)
    : config(config), currentLevel(0) {}

void Game::addChallenge(std::unique_ptr<Challenge> challenge) {
    challenges.push_back(std::move(challenge));
}

Game::SessionResult Game::playSession(AIAgent& agent) {
    // Reset for new session
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

    // Play each level
    for (size_t i = 0; i < challenges.size(); ++i) {
        currentLevel = i;
        bool success = playLevel(i, agent);

        if (config.verboseLogging) {
            printLevelState(*challenges[i]);
            std::cout << (success ? "✓ LEVEL PASSED" : "✗ LEVEL FAILED")
                      << std::endl
                      << std::endl;
        }

        logLevelResult(i, success, *challenges[i]);
    }

    if (config.verboseLogging) {
        std::cout << scoreboard.generateReport() << std::endl;
        agent.printPerformanceSummary();
    }

    // Compile session results
    AIAgent::AgentStats stats = agent.getStats();
    SessionResult result{agent.getName(),
                        stats.levelsCompleted,
                        stats.levelsFailed,
                        stats.totalPenalties,
                        stats.averageEfficiency,
                        {}};

    return result;
}

bool Game::playLevel(int levelIndex, AIAgent& agent) {
    if (levelIndex < 0 || levelIndex >= (int)challenges.size()) {
        return false;
    }

    Challenge& challenge = *challenges[levelIndex];
    challenge.reset();

    // Agent attempts to solve challenge
    bool success = agent.solveChallenge(challenge, scoreboard);

    // Verify solution and apply penalties
    if (success && challenge.isSolved()) {
        if (challenge.isStepLimitExceeded()) {
            scoreboard.penalizeAlgorithmicInefficiency(
                challenge.getStepCount(), challenge.getMaxStepsAllowed(),
                levelIndex);
            success = false;
        }
        if (challenge.isMemoryLimitExceeded()) {
            scoreboard.penalizeResourceMismanagement(
                challenge.getMemoryUsage(), challenge.getMemoryLimit(),
                levelIndex);
            success = false;
        }
    } else {
        scoreboard.penalizeLogicFailure("Challenge not solved",
                                        challenge.getExpectedResult(),
                                        levelIndex);
        success = false;
    }

    return success;
}

int Game::getNumChallenges() const { return challenges.size(); }

Challenge* Game::getChallenge(int index) {
    if (index < 0 || index >= (int)challenges.size()) {
        return nullptr;
    }
    return challenges[index].get();
}

void Game::printGameInfo() const {
    std::cout << "\n========== GAME INFORMATION ==========" << std::endl;
    std::cout << "Title: " << config.gameTitle << std::endl;
    std::cout << "Total Levels: " << challenges.size() << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < challenges.size(); ++i) {
        const Challenge& challenge = *challenges[i];
        std::cout << "Level " << (i + 1) << ": " << challenge.getConfig().name
                  << std::endl;
        std::cout << "  Description: " << challenge.getDescription()
                  << std::endl;
        std::cout << "  Max Steps: " << challenge.getMaxStepsAllowed()
                  << std::endl;
        std::cout << "  Max Memory: " << challenge.getMemoryLimit()
                  << " bytes" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "=====================================" << std::endl;
}

const PenaltyScoreboard& Game::getScoreboard() const { return scoreboard; }

void Game::logLevelResult(int levelIndex, bool success,
                          const Challenge& challenge) {
    // Can be extended for detailed logging to file
    (void)levelIndex;
    (void)success;
    (void)challenge;
}

void Game::printLevelState(const Challenge& challenge) const {
    std::cout << "\nLevel State:" << std::endl;
    std::cout << challenge.getState();
    std::cout << "Expected: " << challenge.getExpectedResult() << std::endl;
}

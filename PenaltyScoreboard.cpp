#include "PenaltyScoreboard.h"
#include <iostream>
#include <sstream>
#include <iomanip>

PenaltyScoreboard::PenaltyScoreboard()
    : totalPenaltyPoints(0),
      gameStartTime(std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch()).count()) {
    // Initialize penalty weights (default = 1x multiplier)
    for (int i = 0; i < 5; ++i) {
        penaltyWeights[i] = 1;
        penaltiesByCategory[i] = 0;
        errorCountByCategory[i] = 0;
    }
}

void PenaltyScoreboard::recordPenalty(ErrorCategory category, int points,
                                       const std::string& description,
                                       int levelId) {
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

void PenaltyScoreboard::penalizeAlgorithmicInefficiency(int stepsUsed,
                                                         int stepsAllowed,
                                                         int levelId) {
    if (stepsUsed > stepsAllowed) {
        int excess = stepsUsed - stepsAllowed;
        int penaltyPoints = std::min(50, (excess * 5) / stepsAllowed);  // Cap at 50
        std::string description =
            "Algorithmic Inefficiency: " + std::to_string(stepsUsed) +
            " steps used vs " + std::to_string(stepsAllowed) + " allowed";
        recordPenalty(ErrorCategory::ALGORITHMIC_INEFFICIENCY, penaltyPoints,
                     description, levelId);
    }
}

void PenaltyScoreboard::penalizeInvalidMove(
    const std::string& attemptedAction, int levelId) {
    recordPenalty(ErrorCategory::INVALID_MOVE, 15,
                 "Invalid move: " + attemptedAction, levelId);
}

void PenaltyScoreboard::penalizeResourceMismanagement(
    size_t memoryUsed, size_t memoryLimit, int levelId) {
    if (memoryUsed > memoryLimit) {
        size_t excess = memoryUsed - memoryLimit;
        int penaltyPoints =
            std::min(40, static_cast<int>((excess * 100) / memoryLimit));
        std::string description =
            "Memory overuse: " + std::to_string(memoryUsed) + " bytes vs " +
            std::to_string(memoryLimit) + " bytes allowed";
        recordPenalty(ErrorCategory::RESOURCE_MISMANAGEMENT, penaltyPoints,
                     description, levelId);
    }
}

void PenaltyScoreboard::penalizeLogicFailure(const std::string& failureReason,
                                              const std::string& expectedResult,
                                              int levelId) {
    recordPenalty(ErrorCategory::LOGIC_FAILURE, 30,
                 "Logic failure: " + failureReason + " (Expected: " +
                     expectedResult + ")",
                 levelId);
}

int PenaltyScoreboard::getTotalPenalties() const {
    return totalPenaltyPoints;
}

int PenaltyScoreboard::getPenaltiesByCategory(ErrorCategory category) const {
    auto it = penaltiesByCategory.find(static_cast<int>(category));
    return (it != penaltiesByCategory.end()) ? it->second : 0;
}

const std::vector<PenaltyScoreboard::PenaltyEvent>&
PenaltyScoreboard::getAllPenalties() const {
    return penaltyHistory;
}

int PenaltyScoreboard::getErrorCount(ErrorCategory category) const {
    auto it = errorCountByCategory.find(static_cast<int>(category));
    return (it != errorCountByCategory.end()) ? it->second : 0;
}

void PenaltyScoreboard::reset() {
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

std::string PenaltyScoreboard::generateReport() const {
    std::ostringstream report;
    report << "\n"
           << "========== PENALTY SCOREBOARD REPORT ==========" << std::endl;
    report << "Total Penalty Points: " << totalPenaltyPoints << std::endl;
    report << "Efficiency Score: " << getEfficiencyScore() << "/100"
           << std::endl;
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

void PenaltyScoreboard::setPenaltyWeight(ErrorCategory category, int weight) {
    penaltyWeights[static_cast<int>(category)] = weight;
}

int PenaltyScoreboard::getEfficiencyScore() const {
    int score = 100 - std::min(100, totalPenaltyPoints);
    return std::max(0, score);
}

int PenaltyScoreboard::applyWeighting(int basePenalty,
                                       ErrorCategory category) const {
    int weight = penaltyWeights.at(static_cast<int>(category));
    return basePenalty * weight;
}

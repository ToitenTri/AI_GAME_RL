#include "Game.h"
#include "ConcreteChallenges.h"
#include "AIAgent.h"
#include <iostream>
#include <iomanip>
#include <memory>

/**
 * @file main.cpp
 * @brief Entry point demonstrating the AI game environment
 *
 * This program demonstrates:
 * 1. Creating a game with multiple challenge types
 * 2. Instantiating different AI agents
 * 3. Running game sessions and collecting results
 * 4. Comparing AI agent performance
 * 5. Analyzing penalty systems
 */

int main() {
    std::cout << "==================================================\n"
              << "   AI ALGORITHMIC CHALLENGE GAME ENVIRONMENT\n"
              << "             C++ Implementation\n"
              << "==================================================\n"
              << std::endl;

    // ===== GAME SETUP =====
    Game::GameConfig gameConfig{
        "AI Algorithm Gauntlet",  // Game title
        3,                         // Number of levels
        true                       // Verbose logging
    };

    Game game(gameConfig);

    // ===== CHALLENGE 1: Pathfinding =====
    {
        Challenge::ChallengeConfig pathConfig{
            0,                          // levelId
            15,                         // maxStepsAllowed (Manhattan distance ~14)
            1024,                       // maxMemoryBytes
            "Pathfinding",              // name
            "Navigate grid from (0,0) to goal, avoiding obstacles"  // description
        };
        game.addChallenge(std::make_unique<PathfindingChallenge>(pathConfig, 10, 10));
    }

    // ===== CHALLENGE 2: Sorting =====
    {
        Challenge::ChallengeConfig sortConfig{
            1,                                // levelId
            50,                               // maxStepsAllowed
            2048,                             // maxMemoryBytes
            "Sorting",                        // name
            "Sort array [64,34,25,12,22,11,90] with minimum operations"  // description
        };
        game.addChallenge(std::make_unique<SortingChallenge>(sortConfig));
    }

    // ===== CHALLENGE 3: Resource Allocation =====
    {
        Challenge::ChallengeConfig resourceConfig{
            2,                              // levelId
            20,                             // maxStepsAllowed
            4096,                           // maxMemoryBytes
            "Resource Allocation",          // name
            "Optimally allocate 100 resource units across 5 tasks"  // description
        };
        game.addChallenge(
            std::make_unique<ResourceAllocationChallenge>(resourceConfig));
    }

    // ===== PRINT GAME INFO =====
    game.printGameInfo();

    // ===== CREATE AND TEST AGENTS =====
    SimpleGreedyAgent greedyAgent;
    OptimizedAgent optimizedAgent;

    std::cout << "\n========== TESTING AGENTS ==========" << std::endl;

    // Test Greedy Agent
    std::cout << "\n--- Running SimpleGreedyAgent ---" << std::endl;
    auto greedyResult = game.playSession(greedyAgent);

    std::cout << "\nGreedy Agent Results:" << std::endl;
    std::cout << "  Levels Completed: " << greedyResult.levelsCompleted
              << std::endl;
    std::cout << "  Levels Failed: " << greedyResult.levelsFailed << std::endl;
    std::cout << "  Total Penalties: " << greedyResult.totalPenalties
              << std::endl;
    std::cout << "  Efficiency Score: " << std::fixed << std::setprecision(2)
              << greedyResult.efficiencyScore << "/100" << std::endl;

    // Test Optimized Agent
    std::cout << "\n--- Running OptimizedAgent ---" << std::endl;
    auto optimizedResult = game.playSession(optimizedAgent);

    std::cout << "\nOptimized Agent Results:" << std::endl;
    std::cout << "  Levels Completed: " << optimizedResult.levelsCompleted
              << std::endl;
    std::cout << "  Levels Failed: " << optimizedResult.levelsFailed
              << std::endl;
    std::cout << "  Total Penalties: " << optimizedResult.totalPenalties
              << std::endl;
    std::cout << "  Efficiency Score: " << std::fixed << std::setprecision(2)
              << optimizedResult.efficiencyScore << "/100" << std::endl;

    // ===== COMPARISON =====
    std::cout << "\n========== AGENT COMPARISON ==========" << std::endl;
    std::cout << "Winner: "
              << (greedyResult.totalPenalties < optimizedResult.totalPenalties
                      ? "SimpleGreedyAgent (fewer penalties)"
                      : "OptimizedAgent (fewer penalties)")
              << std::endl;
    std::cout << "Penalty Difference: "
              << std::abs(greedyResult.totalPenalties - optimizedResult.totalPenalties)
              << " points" << std::endl;
    std::cout << "Efficiency Delta: " << std::fixed << std::setprecision(2)
              << std::abs(greedyResult.efficiencyScore -
                         optimizedResult.efficiencyScore)
              << " points" << std::endl;

    // ===== DEMONSTRATE PENALTY SYSTEM =====
    std::cout << "\n========== PENALTY SYSTEM DEMO ==========" << std::endl;
    std::cout << "The penalty system tracks multiple error types:" << std::endl;
    std::cout << "  • Algorithmic Inefficiency (exceeding step limits)" << std::endl;
    std::cout << "  • Invalid Moves (rule violations)" << std::endl;
    std::cout << "  • Resource Mismanagement (memory limit violations)"
              << std::endl;
    std::cout << "  • Logic Failures (incorrect solutions)" << std::endl;
    std::cout << "  • Time Limit Exceeded (execution timeout)" << std::endl;

    std::cout << "\nCustomizable penalty weights can be applied per category"
              << std::endl;
    std::cout << "to emphasize different types of errors." << std::endl;

    // ===== MODULAR ARCHITECTURE NOTES =====
    std::cout << "\n========== ARCHITECTURE HIGHLIGHTS ==========" << std::endl;
    std::cout << "1. SEPARATION OF CONCERNS:" << std::endl;
    std::cout << "   - Game Engine (Game.h/cpp): Orchestrates gameplay"
              << std::endl;
    std::cout << "   - Challenge System (Challenge.h/cpp): Defines problems"
              << std::endl;
    std::cout << "   - AI Interface (AIAgent.h/cpp): Pluggable solvers"
              << std::endl;
    std::cout << "   - Penalty System (PenaltyScoreboard.h/cpp): Error tracking"
              << std::endl;

    std::cout << "\n2. EXTENSIBILITY:" << std::endl;
    std::cout << "   - Add new challenges by inheriting Challenge"
              << std::endl;
    std::cout << "   - Add new AI agents by inheriting AIAgent" << std::endl;
    std::cout << "   - Customize penalties via setPenaltyWeight()" << std::endl;

    std::cout << "\n3. MEMORY SAFETY:" << std::endl;
    std::cout << "   - Smart pointers (unique_ptr) manage resources"
              << std::endl;
    std::cout << "   - No manual memory allocation/deallocation"
              << std::endl;
    std::cout << "   - RAII principles throughout" << std::endl;

    std::cout << "\n=============================================\n"
              << "         GAME SESSION COMPLETE\n"
              << "=============================================\n"
              << std::endl;

    return 0;
}

#include "AIGameEnvironment.hpp"

int main() {
    std::cout << "==================================================\n"
              << "   AI ALGORITHMIC CHALLENGE GAME ENVIRONMENT\n"
              << "             C++ Single-Header Implementation\n"
              << "==================================================\n"
              << std::endl;

    // Setup game
    Game::GameConfig gameConfig{"AI Algorithm Gauntlet", 3, true};
    Game game(gameConfig);

    // Challenge 1: Pathfinding
    {
        Challenge::ChallengeConfig pathConfig{
            0, 15, 1024, "Pathfinding",
            "Navigate grid from (0,0) to goal, avoiding obstacles"};
        game.addChallenge(std::make_unique<PathfindingChallenge>(pathConfig, 10, 10));
    }

    // Challenge 2: Sorting
    {
        Challenge::ChallengeConfig sortConfig{
            1, 50, 2048, "Sorting",
            "Sort array [64,34,25,12,22,11,90] with minimum operations"};
        game.addChallenge(std::make_unique<SortingChallenge>(sortConfig));
    }

    // Challenge 3: Resource Allocation
    {
        Challenge::ChallengeConfig resourceConfig{
            2, 20, 4096, "Resource Allocation",
            "Optimally allocate 100 resource units across 5 tasks"};
        game.addChallenge(std::make_unique<ResourceAllocationChallenge>(resourceConfig));
    }

    game.printGameInfo();

    // Test agents
    SimpleGreedyAgent greedyAgent;
    OptimizedAgent optimizedAgent;

    std::cout << "\n========== TESTING AGENTS ==========" << std::endl;

    std::cout << "\n--- Running SimpleGreedyAgent ---" << std::endl;
    auto greedyResult = game.playSession(greedyAgent);

    std::cout << "\nGreedy Agent Results:" << std::endl;
    std::cout << "  Levels Completed: " << greedyResult.levelsCompleted << std::endl;
    std::cout << "  Levels Failed: " << greedyResult.levelsFailed << std::endl;
    std::cout << "  Total Penalties: " << greedyResult.totalPenalties << std::endl;
    std::cout << "  Efficiency Score: " << std::fixed << std::setprecision(2)
              << greedyResult.efficiencyScore << "/100" << std::endl;

    std::cout << "\n--- Running OptimizedAgent ---" << std::endl;
    auto optimizedResult = game.playSession(optimizedAgent);

    std::cout << "\nOptimized Agent Results:" << std::endl;
    std::cout << "  Levels Completed: " << optimizedResult.levelsCompleted << std::endl;
    std::cout << "  Levels Failed: " << optimizedResult.levelsFailed << std::endl;
    std::cout << "  Total Penalties: " << optimizedResult.totalPenalties << std::endl;
    std::cout << "  Efficiency Score: " << std::fixed << std::setprecision(2)
              << optimizedResult.efficiencyScore << "/100" << std::endl;

    // Comparison
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
              << std::abs(greedyResult.efficiencyScore - optimizedResult.efficiencyScore)
              << " points" << std::endl;

    std::cout << "\n========== PENALTY SYSTEM DEMO ==========" << std::endl;
    std::cout << "The system tracks:\n"
              << "  • Algorithmic Inefficiency (exceeding step limits)\n"
              << "  • Invalid Moves (rule violations)\n"
              << "  • Resource Mismanagement (memory limit violations)\n"
              << "  • Logic Failures (incorrect solutions)\n"
              << "  • Time Limit Exceeded (execution timeout)\n" << std::endl;

    std::cout << "\n========== ARCHITECTURE HIGHLIGHTS ==========" << std::endl;
    std::cout << "✓ Modular design with separated components\n"
              << "✓ Smart pointers for memory safety\n"
              << "✓ Easy to extend with new challenges and agents\n"
              << "✓ Comprehensive error tracking system\n" << std::endl;

    std::cout << "=============================================\n"
              << "         GAME SESSION COMPLETE\n"
              << "=============================================\n" << std::endl;

    return 0;
}

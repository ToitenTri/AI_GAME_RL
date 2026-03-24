#include "Challenge.h"

Challenge::Challenge(const ChallengeConfig& config)
    : config(config), stepCount(0), memoryUsage(0), solved(false) {}

Challenge::~Challenge() = default;

void Challenge::incrementStepCount() { stepCount++; }

bool Challenge::isStepLimitExceeded() const {
    return stepCount > config.maxStepsAllowed;
}

int Challenge::getStepCount() const { return stepCount; }

int Challenge::getMaxStepsAllowed() const { return config.maxStepsAllowed; }

void Challenge::recordMemoryUsage(size_t bytes) { memoryUsage = bytes; }

size_t Challenge::getMemoryUsage() const { return memoryUsage; }

size_t Challenge::getMemoryLimit() const { return config.maxMemoryBytes; }

bool Challenge::isMemoryLimitExceeded() const {
    return memoryUsage > config.maxMemoryBytes;
}

const Challenge::ChallengeConfig& Challenge::getConfig() const {
    return config;
}

std::string Challenge::getDescription() const {
    return config.description;
}

void Challenge::reset() {
    stepCount = 0;
    memoryUsage = 0;
    solved = false;
    initialize();
}

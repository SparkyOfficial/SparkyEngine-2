#include "../include/LevelProgressionSystem.h"
#include "../include/QuestManager.h"
#include "../include/Quest.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Game Progression Demo\n";
    std::cout << "===================\n\n";
    
    // Get systems
    auto& levelSystem = Sparky::LevelProgressionSystem::getInstance();
    auto& questManager = Sparky::QuestManager::getInstance();
    
    // Set up callbacks for demonstration
    levelSystem.setOnLevelStartCallback([](const std::string& levelName) {
        std::cout << ">>> Starting level: " << levelName << " <<<\n";
    });
    
    levelSystem.setOnLevelCompleteCallback([](const std::string& levelName) {
        std::cout << ">>> Completed level: " << levelName << " <<<\n";
    });
    
    levelSystem.setOnLevelFailCallback([](const std::string& levelName) {
        std::cout << ">>> Failed level: " << levelName << " <<<\n";
    });
    
    levelSystem.setOnProgressUpdateCallback([]() {
        const auto& progress = Sparky::LevelProgressionSystem::getInstance().getPlayerProgress();
        std::cout << "Progress update - Experience: " << progress.experience 
                  << ", Kills: " << progress.totalKills << "\n";
    });
    
    // Create a campaign with multiple levels
    std::cout << "--- Creating Campaign ---\n";
    
    // Level 1: Introduction
    Sparky::LevelInfo introLevel;
    introLevel.name = "Introduction";
    introLevel.description = "Learn the basics";
    introLevel.levelNumber = 1;
    introLevel.difficulty = Sparky::LevelDifficulty::EASY;
    introLevel.sceneFile = "levels/intro.json";
    introLevel.completionCondition = Sparky::LevelCompletionCondition::ALL_OBJECTIVES;
    
    levelSystem.addLevel(introLevel);
    
    // Level 2: The Facility
    Sparky::LevelInfo facilityLevel;
    facilityLevel.name = "Black Mesa Facility";
    facilityLevel.description = "Investigate the research facility";
    facilityLevel.levelNumber = 2;
    facilityLevel.difficulty = Sparky::LevelDifficulty::NORMAL;
    facilityLevel.sceneFile = "levels/facility.json";
    facilityLevel.requiredLevels = {"Introduction"};
    facilityLevel.completionCondition = Sparky::LevelCompletionCondition::MAIN_QUEST_COMPLETED;
    
    levelSystem.addLevel(facilityLevel);
    
    // Level 3: Escape
    Sparky::LevelInfo escapeLevel;
    escapeLevel.name = "Surface Escape";
    escapeLevel.description = "Escape to the surface";
    escapeLevel.levelNumber = 3;
    escapeLevel.difficulty = Sparky::LevelDifficulty::HARD;
    escapeLevel.sceneFile = "levels/escape.json";
    escapeLevel.requiredLevels = {"Black Mesa Facility"};
    escapeLevel.completionCondition = Sparky::LevelCompletionCondition::SURVIVAL;
    
    levelSystem.addLevel(escapeLevel);
    
    std::cout << "Created campaign with 3 levels\n\n";
    
    // Create quests for each level
    std::cout << "--- Creating Quests ---\n";
    
    // Introduction Quest
    auto introQuest = std::make_unique<Sparky::Quest>("Welcome to Black Mesa", "Learn the basics of surviving in the facility");
    auto introObj1 = std::make_unique<Sparky::QuestObjective>("Move around the facility", 1);
    auto introObj2 = std::make_unique<Sparky::QuestObjective>("Pick up a health pack", 1);
    auto introObj3 = std::make_unique<Sparky::QuestObjective>("Defeat 3 security guards", 3);
    
    introQuest->addObjective(std::move(introObj1));
    introQuest->addObjective(std::move(introObj2));
    introQuest->addObjective(std::move(introObj3));
    introQuest->setRewardExperience(100);
    introQuest->setRewardCurrency(25);
    
    questManager.addQuest(std::move(introQuest));
    levelSystem.linkQuestToLevel("Welcome to Black Mesa", "Introduction");
    
    // Facility Quest
    auto facilityQuest = std::make_unique<Sparky::Quest>("The Black Mesa Incident", "Investigate the mysterious incident");
    auto facilityObj1 = std::make_unique<Sparky::QuestObjective>("Find the security office", 1);
    auto facilityObj2 = std::make_unique<Sparky::QuestObjective>("Collect security tapes", 5);
    auto facilityObj3 = std::make_unique<Sparky::QuestObjective>("Activate emergency protocols", 1);
    
    facilityQuest->addObjective(std::move(facilityObj1));
    facilityQuest->addObjective(std::move(facilityObj2));
    facilityQuest->addObjective(std::move(facilityObj3));
    facilityQuest->setRewardExperience(300);
    facilityQuest->setRewardCurrency(75);
    
    questManager.addQuest(std::move(facilityQuest));
    levelSystem.linkQuestToLevel("The Black Mesa Incident", "Black Mesa Facility");
    
    // Escape Quest
    auto escapeQuest = std::make_unique<Sparky::Quest>("Escape from Black Mesa", "Find a way to the surface");
    auto escapeObj1 = std::make_unique<Sparky::QuestObjective>("Find the tram station", 1);
    auto escapeObj2 = std::make_unique<Sparky::QuestObjective>("Survive the underground tunnels", 1);
    auto escapeObj3 = std::make_unique<Sparky::QuestObjective>("Reach the surface", 1);
    
    escapeQuest->addObjective(std::move(escapeObj1));
    escapeQuest->addObjective(std::move(escapeObj2));
    escapeQuest->addObjective(std::move(escapeObj3));
    escapeQuest->setRewardExperience(500);
    escapeQuest->setRewardCurrency(150);
    
    questManager.addQuest(std::move(escapeQuest));
    levelSystem.linkQuestToLevel("Escape from Black Mesa", "Surface Escape");
    
    std::cout << "Created quests for all levels\n\n";
    
    // Demonstrate progression
    std::cout << "--- Campaign Progression ---\n";
    
    // Start with Introduction level
    std::cout << "\n1. Starting Introduction Level\n";
    levelSystem.startLevel("Introduction");
    
    // Complete some objectives
    std::cout << "Completing Introduction Quest objectives...\n";
    questManager.addObjectiveProgress("Welcome to Black Mesa", "Move around the facility", 1);
    questManager.addObjectiveProgress("Welcome to Black Mesa", "Pick up a health pack", 1);
    questManager.addObjectiveProgress("Welcome to Black Mesa", "Defeat 3 security guards", 3);
    
    // Complete the quest and level
    std::cout << "Completing Introduction Quest...\n";
    questManager.completeQuest("Welcome to Black Mesa");
    levelSystem.completeLevel("Introduction");
    
    // Show progression
    levelSystem.addKill();
    levelSystem.addKill();
    levelSystem.addKill();
    
    std::cout << "\nPlayer stats after Introduction:\n";
    const auto& progress = levelSystem.getPlayerProgress();
    std::cout << "- Experience: " << progress.experience << "\n";
    std::cout << "- Kills: " << progress.totalKills << "\n";
    std::cout << "- Deaths: " << progress.totalDeaths << "\n";
    
    // Check available levels
    std::cout << "\nAvailable levels after completing Introduction:\n";
    auto available = levelSystem.getAvailableLevels();
    for (const auto& level : available) {
        std::cout << "- " << level << "\n";
    }
    
    // Start Facility level
    std::cout << "\n2. Starting Black Mesa Facility Level\n";
    levelSystem.startLevel("Black Mesa Facility");
    
    // Complete some objectives
    std::cout << "Completing Facility Quest objectives...\n";
    questManager.addObjectiveProgress("The Black Mesa Incident", "Find the security office", 1);
    questManager.addObjectiveProgress("The Black Mesa Incident", "Collect security tapes", 5);
    questManager.addObjectiveProgress("The Black Mesa Incident", "Activate emergency protocols", 1);
    
    // Complete the quest and level
    std::cout << "Completing Facility Quest...\n";
    questManager.completeQuest("The Black Mesa Incident");
    levelSystem.completeLevel("Black Mesa Facility");
    
    // Add more stats
    for (int i = 0; i < 5; i++) {
        levelSystem.addKill();
    }
    levelSystem.addDeath();
    
    std::cout << "\nPlayer stats after Facility:\n";
    const auto& progress2 = levelSystem.getPlayerProgress();
    std::cout << "- Experience: " << progress2.experience << "\n";
    std::cout << "- Kills: " << progress2.totalKills << "\n";
    std::cout << "- Deaths: " << progress2.totalDeaths << "\n";
    
    // Check available levels
    std::cout << "\nAvailable levels after completing Facility:\n";
    available = levelSystem.getAvailableLevels();
    for (const auto& level : available) {
        std::cout << "- " << level << "\n";
    }
    
    // Start Escape level
    std::cout << "\n3. Starting Surface Escape Level\n";
    levelSystem.startLevel("Surface Escape");
    
    // Complete some objectives
    std::cout << "Completing Escape Quest objectives...\n";
    questManager.addObjectiveProgress("Escape from Black Mesa", "Find the tram station", 1);
    questManager.addObjectiveProgress("Escape from Black Mesa", "Survive the underground tunnels", 1);
    questManager.addObjectiveProgress("Escape from Black Mesa", "Reach the surface", 1);
    
    // Complete the quest and level
    std::cout << "Completing Escape Quest...\n";
    questManager.completeQuest("Escape from Black Mesa");
    levelSystem.completeLevel("Surface Escape");
    
    // Add final stats
    for (int i = 0; i < 3; i++) {
        levelSystem.addKill();
    }
    
    std::cout << "\nFinal player stats:\n";
    const auto& finalProgress = levelSystem.getPlayerProgress();
    std::cout << "- Experience: " << finalProgress.experience << "\n";
    std::cout << "- Kills: " << finalProgress.totalKills << "\n";
    std::cout << "- Deaths: " << finalProgress.totalDeaths << "\n";
    std::cout << "- Play Time: " << finalProgress.playTime << " seconds\n";
    
    std::cout << "\nCompleted levels:\n";
    auto completed = levelSystem.getCompletedLevels();
    for (const auto& level : completed) {
        std::cout << "- " << level << "\n";
    }
    
    // Test save/load
    std::cout << "\n--- Testing Save/Load ---\n";
    bool saved = levelSystem.saveProgress("demo_progress.dat");
    std::cout << "Progress saved: " << (saved ? "Yes" : "No") << "\n";
    
    std::cout << "\nCampaign progression demo completed successfully!\n";
    std::cout << "This demonstrates a complete game progression system with:\n";
    std::cout << "- Level dependencies and unlocking\n";
    std::cout << "- Quest integration with levels\n";
    std::cout << "- Player statistics tracking\n";
    std::cout << "- Save/load functionality\n";
    std::cout << "- Progress callbacks and events\n";
    
    return 0;
}
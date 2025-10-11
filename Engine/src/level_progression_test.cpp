#include "../include/LevelProgressionSystem.h"
#include "../include/QuestManager.h"
#include "../include/Quest.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing Level Progression System\n";
    std::cout << "===============================\n\n";
    
    // Get the level progression system instance
    auto& levelSystem = Sparky::LevelProgressionSystem::getInstance();
    
    // Create some levels
    std::cout << "--- Creating Levels ---\n";
    
    Sparky::LevelInfo tutorialLevel;
    tutorialLevel.name = "Tutorial";
    tutorialLevel.description = "Learn the basics of the game";
    tutorialLevel.levelNumber = 1;
    tutorialLevel.difficulty = Sparky::LevelDifficulty::EASY;
    tutorialLevel.sceneFile = "levels/tutorial.json";
    tutorialLevel.completionCondition = Sparky::LevelCompletionCondition::ALL_OBJECTIVES;
    
    levelSystem.addLevel(tutorialLevel);
    
    Sparky::LevelInfo firstLevel;
    firstLevel.name = "Facility";
    firstLevel.description = "Black Mesa Research Facility";
    firstLevel.levelNumber = 2;
    firstLevel.difficulty = Sparky::LevelDifficulty::NORMAL;
    firstLevel.sceneFile = "levels/facility.json";
    firstLevel.requiredLevels = {"Tutorial"}; // Must complete tutorial first
    firstLevel.completionCondition = Sparky::LevelCompletionCondition::MAIN_QUEST_COMPLETED;
    
    levelSystem.addLevel(firstLevel);
    
    Sparky::LevelInfo secondLevel;
    secondLevel.name = "Surface";
    secondLevel.description = "Escape to the surface";
    secondLevel.levelNumber = 3;
    secondLevel.difficulty = Sparky::LevelDifficulty::HARD;
    secondLevel.sceneFile = "levels/surface.json";
    secondLevel.requiredLevels = {"Facility"}; // Must complete facility first
    secondLevel.completionCondition = Sparky::LevelCompletionCondition::ALL_OBJECTIVES;
    
    levelSystem.addLevel(secondLevel);
    
    std::cout << "Created 3 levels\n\n";
    
    // Test level access
    std::cout << "--- Testing Level Access ---\n";
    std::cout << "Can access Tutorial: " << (levelSystem.canAccessLevel("Tutorial") ? "Yes" : "No") << "\n";
    std::cout << "Can access Facility: " << (levelSystem.canAccessLevel("Facility") ? "Yes" : "No") << "\n";
    std::cout << "Can access Surface: " << (levelSystem.canAccessLevel("Surface") ? "Yes" : "No") << "\n\n";
    
    // Get available levels
    std::cout << "--- Available Levels ---\n";
    auto available = levelSystem.getAvailableLevels();
    for (const auto& level : available) {
        std::cout << "- " << level << "\n";
    }
    std::cout << "\n";
    
    // Start and complete tutorial
    std::cout << "--- Completing Tutorial ---\n";
    levelSystem.startLevel("Tutorial");
    levelSystem.completeLevel("Tutorial");
    
    std::cout << "Tutorial completed!\n";
    std::cout << "Experience: " << levelSystem.getPlayerProgress().experience << "\n\n";
    
    // Test level access again
    std::cout << "--- Testing Level Access After Tutorial ---\n";
    std::cout << "Can access Tutorial: " << (levelSystem.canAccessLevel("Tutorial") ? "Yes" : "No") << "\n";
    std::cout << "Can access Facility: " << (levelSystem.canAccessLevel("Facility") ? "Yes" : "No") << "\n";
    std::cout << "Can access Surface: " << (levelSystem.canAccessLevel("Surface") ? "Yes" : "No") << "\n\n";
    
    // Get available levels again
    std::cout << "--- Available Levels After Tutorial ---\n";
    available = levelSystem.getAvailableLevels();
    for (const auto& level : available) {
        std::cout << "- " << level << "\n";
    }
    std::cout << "\n";
    
    // Test Quest Integration
    std::cout << "--- Testing Quest Integration ---\n";
    auto& questManager = Sparky::QuestManager::getInstance();
    
    // Create a quest for the facility level
    auto facilityQuest = std::make_unique<Sparky::Quest>("Black Mesa Incident", "Investigate the mysterious incident at Black Mesa");
    auto objective1 = std::make_unique<Sparky::QuestObjective>("Find the security office", 1);
    auto objective2 = std::make_unique<Sparky::QuestObjective>("Collect security tapes", 3);
    auto objective3 = std::make_unique<Sparky::QuestObjective>("Escape the facility", 1);
    
    facilityQuest->addObjective(std::move(objective1));
    facilityQuest->addObjective(std::move(objective2));
    facilityQuest->addObjective(std::move(objective3));
    facilityQuest->setRewardExperience(200);
    facilityQuest->setRewardCurrency(50);
    
    questManager.addQuest(std::move(facilityQuest));
    
    // Link quest to level
    levelSystem.linkQuestToLevel("Black Mesa Incident", "Facility");
    
    // Check linked quests
    auto quests = levelSystem.getQuestsForLevel("Facility");
    std::cout << "Quests for Facility level:\n";
    for (const auto& quest : quests) {
        std::cout << "- " << quest << "\n";
    }
    std::cout << "\n";
    
    // Test player progression
    std::cout << "--- Testing Player Progression ---\n";
    levelSystem.addKill();
    levelSystem.addKill();
    levelSystem.addDeath();
    levelSystem.addExperience(50);
    
    const auto& progress = levelSystem.getPlayerProgress();
    std::cout << "Total Kills: " << progress.totalKills << "\n";
    std::cout << "Total Deaths: " << progress.totalDeaths << "\n";
    std::cout << "Experience: " << progress.experience << "\n";
    std::cout << "Play Time: " << progress.playTime << " seconds\n\n";
    
    // Test save/load
    std::cout << "--- Testing Save/Load ---\n";
    bool saveResult = levelSystem.saveProgress("test_progress.dat");
    std::cout << "Save result: " << (saveResult ? "Success" : "Failed") << "\n";
    
    // Create a new level system instance to test loading
    auto& levelSystem2 = Sparky::LevelProgressionSystem::getInstance();
    bool loadResult = levelSystem2.loadProgress("test_progress.dat");
    std::cout << "Load result: " << (loadResult ? "Success" : "Failed") << "\n";
    
    if (loadResult) {
        const auto& progress2 = levelSystem2.getPlayerProgress();
        std::cout << "Loaded Experience: " << progress2.experience << "\n";
        std::cout << "Loaded Kills: " << progress2.totalKills << "\n";
    }
    
    std::cout << "\nLevel progression system test completed successfully!\n";
    return 0;
}
#include "../include/QuestManager.h"
#include "../include/Quest.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>

namespace Sparky {

    QuestManager::QuestManager() {
    }

    QuestManager::~QuestManager() {
    }

    QuestManager& QuestManager::getInstance() {
        static QuestManager instance;
        return instance;
    }

    void QuestManager::addQuest(std::unique_ptr<Quest> quest) {
        if (quest) {
            const std::string& name = quest->getName();
            quests[name] = std::move(quest);
            SPARKY_LOG_DEBUG("Added quest: " + name);
        }
    }

    void QuestManager::removeQuest(const std::string& questName) {
        auto it = quests.find(questName);
        if (it != quests.end()) {
            quests.erase(it);
            SPARKY_LOG_DEBUG("Removed quest: " + questName);
        }
    }

    Quest* QuestManager::getQuest(const std::string& questName) {
        auto it = quests.find(questName);
        if (it != quests.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void QuestManager::startQuest(const std::string& questName) {
        Quest* quest = getQuest(questName);
        if (quest) {
            quest->start();
            SPARKY_LOG_DEBUG("Started quest: " + questName);
        }
    }

    void QuestManager::completeQuest(const std::string& questName) {
        Quest* quest = getQuest(questName);
        if (quest) {
            quest->complete();
            SPARKY_LOG_DEBUG("Completed quest: " + questName);
        }
    }

    void QuestManager::failQuest(const std::string& questName) {
        Quest* quest = getQuest(questName);
        if (quest) {
            quest->fail();
            SPARKY_LOG_DEBUG("Failed quest: " + questName);
        }
    }

    void QuestManager::addObjectiveProgress(const std::string& questName, const std::string& objectiveDesc, int amount) {
        Quest* quest = getQuest(questName);
        if (quest) {
            quest->addObjectiveProgress(objectiveDesc, amount);
            SPARKY_LOG_DEBUG("Added progress to quest " + questName + ": " + objectiveDesc);
        }
    }

    std::vector<Quest*> QuestManager::getActiveQuests() {
        std::vector<Quest*> activeQuests;
        for (auto& pair : quests) {
            if (pair.second->getStatus() == QuestStatus::IN_PROGRESS) {
                activeQuests.push_back(pair.second.get());
            }
        }
        return activeQuests;
    }

    std::vector<Quest*> QuestManager::getCompletedQuests() {
        std::vector<Quest*> completedQuests;
        for (auto& pair : quests) {
            if (pair.second->getStatus() == QuestStatus::COMPLETED) {
                completedQuests.push_back(pair.second.get());
            }
        }
        return completedQuests;
    }

    std::vector<Quest*> QuestManager::getFailedQuests() {
        std::vector<Quest*> failedQuests;
        for (auto& pair : quests) {
            if (pair.second->getStatus() == QuestStatus::FAILED) {
                failedQuests.push_back(pair.second.get());
            }
        }
        return failedQuests;
    }

    bool QuestManager::saveToFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Saving quests to file: " + filepath);
        
        try {
            std::ofstream file(filepath);
            if (!file.is_open()) {
                SPARKY_LOG_ERROR("Failed to open quests file for writing: " + filepath);
                return false;
            }

            file << quests.size() << "\n";
            
            for (auto& pair : quests) {
                Quest* quest = pair.second.get();
                file << quest->getName() << "\n";
                file << quest->getDescription() << "\n";
                file << static_cast<int>(quest->getStatus()) << "\n";
                file << quest->getRewardExperience() << "\n";
                file << quest->getRewardCurrency() << "\n";
                
                // Save objectives
                const auto& objectives = quest->getObjectives();
                file << objectives.size() << "\n";
                for (const auto& objective : objectives) {
                    file << objective->getDescription() << "\n";
                    file << objective->getRequiredAmount() << "\n";
                    file << objective->getCurrentAmount() << "\n";
                }
            }
            
            file.close();
            SPARKY_LOG_DEBUG("Quests saved successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to save quests: " + std::string(e.what()));
            return false;
        }
    }

    bool QuestManager::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading quests from file: " + filepath);
        
        try {
            if (!FileUtils::fileExists(filepath)) {
                SPARKY_LOG_WARNING("Quests file does not exist: " + filepath);
                return false;
            }

            std::vector<char> data = FileUtils::readFile(filepath);
            std::string content(data.begin(), data.end());
            
            std::istringstream iss(content);
            std::string line;
            
            // Read quest count
            if (!std::getline(iss, line)) {
                SPARKY_LOG_ERROR("Invalid quests file format");
                return false;
            }
            
            int questCount = std::stoi(line);
            
            // Read quests
            for (int i = 0; i < questCount; i++) {
                std::string name, description;
                int status, exp, currency, objectiveCount;
                
                if (!std::getline(iss, name) || 
                    !std::getline(iss, description) ||
                    !(iss >> status) || 
                    !(iss >> exp) || 
                    !(iss >> currency) || 
                    !(iss >> objectiveCount)) {
                    SPARKY_LOG_ERROR("Invalid quest data in file");
                    return false;
                }
                
                // Consume the newline character after reading integers
                iss.ignore();
                
                auto quest = std::make_unique<Quest>(name, description);
                quest->setRewardExperience(exp);
                quest->setRewardCurrency(currency);
                
                // Read objectives
                for (int j = 0; j < objectiveCount; j++) {
                    std::string objDesc;
                    int reqAmount, curAmount;
                    
                    if (!std::getline(iss, objDesc) ||
                        !(iss >> reqAmount) || 
                        !(iss >> curAmount)) {
                        SPARKY_LOG_ERROR("Invalid objective data in file");
                        return false;
                    }
                    
                    // Consume the newline character
                    iss.ignore();
                    
                    auto objective = std::make_unique<QuestObjective>(objDesc, reqAmount);
                    objective->setCurrentAmount(curAmount);
                    quest->addObjective(std::move(objective));
                }
                
                quests[name] = std::move(quest);
            }
            
            SPARKY_LOG_DEBUG("Quests loaded successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load quests: " + std::string(e.what()));
            return false;
        }
    }

    void QuestManager::update(float deltaTime) {
        // Update quest logic if needed
        // For now, just log that we're updating
        SPARKY_LOG_DEBUG("Updating quest manager");
    }
}
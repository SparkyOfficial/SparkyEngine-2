#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace Sparky {
    class Quest;

    class QuestManager {
    public:
        static QuestManager& getInstance();

        // Quest management
        void addQuest(std::unique_ptr<Quest> quest);
        void removeQuest(const std::string& questName);
        Quest* getQuest(const std::string& questName);

        // Quest progression
        void startQuest(const std::string& questName);
        void completeQuest(const std::string& questName);
        void failQuest(const std::string& questName);

        // Progress tracking
        void addObjectiveProgress(const std::string& questName, const std::string& objectiveDesc, int amount = 1);

        // Quest lists
        std::vector<Quest*> getActiveQuests();
        std::vector<Quest*> getCompletedQuests();
        std::vector<Quest*> getFailedQuests();

        // Serialization
        bool saveToFile(const std::string& filepath);
        bool loadFromFile(const std::string& filepath);

        // Update
        void update(float deltaTime);

    private:
        QuestManager();
        ~QuestManager();

        std::unordered_map<std::string, std::unique_ptr<Quest>> quests;
    };
}
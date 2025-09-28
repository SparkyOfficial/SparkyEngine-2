#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace Sparky {
    enum class QuestStatus {
        NOT_STARTED,
        IN_PROGRESS,
        COMPLETED,
        FAILED
    };

    class QuestObjective {
    public:
        QuestObjective(const std::string& description, int requiredAmount = 1);
        ~QuestObjective();

        const std::string& getDescription() const { return description; }
        int getRequiredAmount() const { return requiredAmount; }
        int getCurrentAmount() const { return currentAmount; }
        bool isCompleted() const { return currentAmount >= requiredAmount; }

        void setCurrentAmount(int amount);
        void addProgress(int amount = 1);

    private:
        std::string description;
        int requiredAmount;
        int currentAmount;
    };

    class Quest {
    public:
        Quest(const std::string& name, const std::string& description);
        ~Quest();

        // Quest info
        const std::string& getName() const { return name; }
        const std::string& getDescription() const { return description; }
        QuestStatus getStatus() const { return status; }

        // Objectives
        void addObjective(std::unique_ptr<QuestObjective> objective);
        const std::vector<std::unique_ptr<QuestObjective>>& getObjectives() const { return objectives; }
        bool areAllObjectivesCompleted() const;

        // Status management
        void start();
        void complete();
        void fail();

        // Progress
        void addObjectiveProgress(const std::string& objectiveDesc, int amount = 1);

        // Rewards
        void setRewardExperience(int exp);
        void setRewardCurrency(int currency);
        int getRewardExperience() const { return rewardExperience; }
        int getRewardCurrency() const { return rewardCurrency; }

        // Callbacks
        void setOnStartCallback(std::function<void()> callback);
        void setOnCompleteCallback(std::function<void()> callback);
        void setOnFailCallback(std::function<void()> callback);

    private:
        std::string name;
        std::string description;
        QuestStatus status;

        std::vector<std::unique_ptr<QuestObjective>> objectives;

        int rewardExperience;
        int rewardCurrency;

        std::function<void()> onStartCallback;
        std::function<void()> onCompleteCallback;
        std::function<void()> onFailCallback;
    };
}
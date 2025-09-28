#include "../include/Quest.h"
#include "../include/Logger.h"

namespace Sparky {

    // QuestObjective implementation
    QuestObjective::QuestObjective(const std::string& description, int requiredAmount) :
        description(description), requiredAmount(requiredAmount), currentAmount(0) {
    }

    QuestObjective::~QuestObjective() {
    }

    void QuestObjective::setCurrentAmount(int amount) {
        currentAmount = amount;
        if (currentAmount > requiredAmount) {
            currentAmount = requiredAmount;
        }
    }

    void QuestObjective::addProgress(int amount) {
        currentAmount += amount;
        if (currentAmount > requiredAmount) {
            currentAmount = requiredAmount;
        }
    }

    // Quest implementation
    Quest::Quest(const std::string& name, const std::string& description) :
        name(name), description(description), status(QuestStatus::NOT_STARTED),
        rewardExperience(0), rewardCurrency(0) {
        SPARKY_LOG_DEBUG("Created quest: " + name);
    }

    Quest::~Quest() {
    }

    void Quest::addObjective(std::unique_ptr<QuestObjective> objective) {
        if (objective) {
            objectives.push_back(std::move(objective));
            SPARKY_LOG_DEBUG("Added objective to quest: " + name);
        }
    }

    bool Quest::areAllObjectivesCompleted() const {
        for (const auto& objective : objectives) {
            if (!objective->isCompleted()) {
                return false;
            }
        }
        return true;
    }

    void Quest::start() {
        if (status == QuestStatus::NOT_STARTED) {
            status = QuestStatus::IN_PROGRESS;
            SPARKY_LOG_DEBUG("Quest started: " + name);
            
            if (onStartCallback) {
                onStartCallback();
            }
        }
    }

    void Quest::complete() {
        if (status == QuestStatus::IN_PROGRESS && areAllObjectivesCompleted()) {
            status = QuestStatus::COMPLETED;
            SPARKY_LOG_DEBUG("Quest completed: " + name);
            
            if (onCompleteCallback) {
                onCompleteCallback();
            }
        }
    }

    void Quest::fail() {
        if (status == QuestStatus::IN_PROGRESS) {
            status = QuestStatus::FAILED;
            SPARKY_LOG_DEBUG("Quest failed: " + name);
            
            if (onFailCallback) {
                onFailCallback();
            }
        }
    }

    void Quest::addObjectiveProgress(const std::string& objectiveDesc, int amount) {
        if (status != QuestStatus::IN_PROGRESS) return;

        for (auto& objective : objectives) {
            if (objective->getDescription() == objectiveDesc) {
                objective->addProgress(amount);
                
                // Check if all objectives are completed
                if (areAllObjectivesCompleted()) {
                    complete();
                }
                
                break;
            }
        }
    }

    void Quest::setRewardExperience(int exp) {
        rewardExperience = exp;
        SPARKY_LOG_DEBUG("Quest reward experience set to: " + std::to_string(exp));
    }

    void Quest::setRewardCurrency(int currency) {
        rewardCurrency = currency;
        SPARKY_LOG_DEBUG("Quest reward currency set to: " + std::to_string(currency));
    }

    void Quest::setOnStartCallback(std::function<void()> callback) {
        onStartCallback = callback;
    }

    void Quest::setOnCompleteCallback(std::function<void()> callback) {
        onCompleteCallback = callback;
    }

    void Quest::setOnFailCallback(std::function<void()> callback) {
        onFailCallback = callback;
    }
}
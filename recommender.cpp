#include "recommender.h"
#include <queue>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <algorithm>

void Recommender::addQuestion(const Question& q) {
    allQuestions.insert({q.id, q});
}

void Recommender::addRelationship(int id1, int id2) {
    adjList[id1].push_back(id2);
    adjList[id2].push_back(id1);
}

void Recommender::loadQuestionsFromFile(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) return;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, title, diff, topic;
        // Parsing CSV: ID, Title, Difficulty, Topic
        if (std::getline(ss, idStr, ',') && std::getline(ss, title, ',') &&
            std::getline(ss, diff, ',') && std::getline(ss, topic, ',')) {
            addQuestion(Question(std::stoi(idStr), title, diff, topic));
        }
    }
}

Question Recommender::getQuestion(int id) {
    return allQuestions.at(id);
}

// Optimized BFS for Topic and Difficulty-based Ranking
std::vector<int> Recommender::getRecommendations(int startID) {
    std::vector<int> recommendations;
    std::queue<int> q;
    std::unordered_set<int> visited;

    if (allQuestions.find(startID) == allQuestions.end()) return recommendations;

    Question startQ = allQuestions.at(startID);
    q.push(startID);
    visited.insert(startID);

    // Sorting buckets for multi-criteria prioritization
    std::vector<int> highPriority;   // Same Topic AND Same Difficulty
    std::vector<int> mediumPriority; // Same Topic but Different Difficulty
    std::vector<int> lowPriority;    // Different Topic

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current != startID) {
            Question nextQ = allQuestions.at(current);
            
            if (nextQ.topic == startQ.topic) {
                if (nextQ.difficulty == startQ.difficulty)
                    highPriority.push_back(current); 
                else
                    mediumPriority.push_back(current);
            } else {
                lowPriority.push_back(current);
            }
        }

        for (int neighbor : adjList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    // Merge buckets in order of priority
    recommendations.insert(recommendations.end(), highPriority.begin(), highPriority.end());
    recommendations.insert(recommendations.end(), mediumPriority.begin(), mediumPriority.end());
    recommendations.insert(recommendations.end(), lowPriority.begin(), lowPriority.end());

    return recommendations;
}

std::vector<int> Recommender::getQuestionsByTopic(std::string topic) {
    std::vector<int> matches;
    for (auto const& [id, q] : allQuestions) {
        if (q.topic == topic) matches.push_back(id);
    }
    return matches;
}

std::vector<int> Recommender::searchByTitle(std::string keyword) {
    std::vector<int> matches;
    for (auto const& [id, q] : allQuestions) {
        // String::find returns npos if the keyword isn't found
        if (q.title.find(keyword) != std::string::npos) {
            matches.push_back(id);
        }
    }
    return matches;
}

void Recommender::printGraph() {
    for (auto const& [id, neighbors] : adjList) {
        std::cout << "Problem [" << allQuestions.at(id).title << "] links to: ";
        for (int n : neighbors) std::cout << allQuestions.at(n).title << ", ";
        std::cout << "\n";
    }
}

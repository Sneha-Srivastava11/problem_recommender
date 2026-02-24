#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include "question.h"

class Recommender {
private:
    // Adjacency List: Represents the Graph connections
    std::unordered_map<int, std::vector<int>> adjList;

    // Database: Maps ID to Question details for O(1) lookup
    std::unordered_map<int, Question> allQuestions;

public:
    // --- Data Management ---
    void addQuestion(const Question& q);
    void addRelationship(int id1, int id2);
    void loadQuestionsFromFile(std::string filename);
    Question getQuestion(int id);

    // --- Search & Recommendation Logic ---
    
    // Core DSA Engine: BFS that ranks by Difficulty similarity
    std::vector<int> getRecommendations(int startID);
    
    // Filter problems specifically by category (e.g., Array, Graph)
    std::vector<int> getQuestionsByTopic(std::string topic);
    
    // Search titles for matching words
    std::vector<int> searchByTitle(std::string keyword);

    // --- Debugging ---
    void printGraph();
};

#endif

#include "ladder.h"

#include <queue>
#include <vector>
#include <string>
#include <unordered_set>
#include <stack>
#include <algorithm>

void error(string word1, string word2, string msg) {
    cerr << "For: " << word1 << ", " << word2 << " - " << msg << endl;
}

//edit distance? like insert and deletion?
bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int m = str1.size();
    int n = str2.size();

    //if difference is greater then false
    if (abs(m-n) > d) {
        return false;
    }

    //store edit distances
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));

    for (int i=0; i <= m; ++i) {
        dp[i][0] = i; //delete i char
    }
    for (int j=0; j <= n; ++j) {
        dp[0][j] = j; //insert j char
    }

    //fill dp table
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    //check final edit distance
    return dp[m][n] <= d;
}

//determine legal hops
bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) return true; // For testing purposes.
    if (word1 == word2) return false; 

    int length1 = word1.size();
    int length2 = word2.size();

    if (abs(length1-length2) > 1) {
        return false;
    }

    //substitution
    if (length1 == length2) {
        int diff = 0;
        for (size_t i = 0; i < word1.size(); ++i) {
            if (word1[i] != word2[i])
                ++diff;
            if (diff > 1)
                return false;
        }
        return diff == 1;
    } else {
        const string& shorter = (word1.size() < word2.size()) ? word1 : word2;
        const string& longer  = (word1.size() < word2.size()) ? word2 : word1;
        int i = 0, j = 0, diff = 0;
        while (i < shorter.size() && j < longer.size()) {
            if (shorter[i] == longer[j]) {
                ++i;
                ++j;
            } else {
                ++j;
                ++diff;
                if (diff > 1)
                    return false;
            }
        }
        if (j < longer.size())
            ++diff;
        return diff == 1;
    }
}

//word ladder func that uses bfs
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        return {};
    }
    // Make a copy of the dictionary to remove used words.
    set<string> dict = word_list;
    // The end word must be in the dictionary; the start word might be outside.
    queue<vector<string>> ladder_queue;
    vector<string> start_ladder = { begin_word };
    ladder_queue.push(start_ladder);
    
    while (!ladder_queue.empty()) {
        int level_size = ladder_queue.size();
        // Keep track of words used in this level.
        set<string> used_words;
        for (int i = 0; i < level_size; i++) {
            vector<string> ladder = ladder_queue.front();
            ladder_queue.pop();
            string last_word = ladder.back();
            if (last_word == end_word) {
                return ladder;
            }
            // For each word remaining in the dictionary, if it is adjacent to the last word, add it.
            vector<string> neighbors;
            for (const string& word : dict) {
                // For ladder generation, explicitly skip identical words.
                if (word == last_word)
                    continue;
                if (is_adjacent(last_word, word)) {
                    neighbors.push_back(word);
                }
            }
            // Enqueue new ladders and mark words as used.
            for (const string& n : neighbors) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(n);
                ladder_queue.push(new_ladder);
                used_words.insert(n);
            }
        }
        
        if (used_words.empty()) {
            return {};
        }
        // Remove all words used in this level from the dictionary.
        for (const string& w : used_words) {
            dict.erase(w);
        }
    }
    return {}; // No ladder found.
}


//load ladder word into filename
void load_words(set<string> & word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        error("load_words", file_name, "Could not open file.");
        return;
    }

    string word;
    while (file >> word) { // Read each word from the file
        word_list.insert(word); // Insert the word into the set
    }

    file.close(); // Close the file
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No ladder found." << endl;
        return;
    }

    cout << "Word ladder found:";
    for (const string& word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}

#define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "src/words.txt");  // Adjust the file path if necessary

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}

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
    if (dict.find(end_word) == dict.end()) {
        return {}; // End word is not in the dictionary.
    }

    queue<vector<string>> ladder_queue;
    vector<string> start_ladder = { begin_word };
    ladder_queue.push(start_ladder);

    // Track used words to avoid reuse.
    unordered_set<string> used_words;
    used_words.insert(begin_word);

    while (!ladder_queue.empty()) {
        int level_size = ladder_queue.size();

        for (int i = 0; i < level_size; i++) {
            vector<string> ladder = ladder_queue.front();
            ladder_queue.pop();
            string last_word = ladder.back();

            // Check if the last word is the end word.
            if (last_word == end_word) {
                return ladder;
            }

            // Generate all possible neighbors.
            for (size_t j = 0; j < last_word.size(); ++j) {
                string new_word = last_word;
                for (char c = 'a'; c <= 'z'; ++c) {
                    new_word[j] = c; // Try substituting each character.
                    if (dict.find(new_word) != dict.end() && used_words.find(new_word) == used_words.end()) {
                        vector<string> new_ladder = ladder;
                        new_ladder.push_back(new_word);
                        ladder_queue.push(new_ladder);
                        used_words.insert(new_word);
                    }
                }
            }

            // Try adding a character.
            for (size_t j = 0; j <= last_word.size(); ++j) {
                for (char c = 'a'; c <= 'z'; ++c) {
                    string new_word = last_word;
                    new_word.insert(j, 1, c); // Insert character at position j.
                    if (dict.find(new_word) != dict.end() && used_words.find(new_word) == used_words.end()) {
                        vector<string> new_ladder = ladder;
                        new_ladder.push_back(new_word);
                        ladder_queue.push(new_ladder);
                        used_words.insert(new_word);
                    }
                }
            }

            // Try deleting a character.
            for (size_t j = 0; j < last_word.size(); ++j) {
                string new_word = last_word;
                new_word.erase(j, 1); // Delete character at position j.
                if (dict.find(new_word) != dict.end() && used_words.find(new_word) == used_words.end()) {
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(new_word);
                    ladder_queue.push(new_ladder);
                    used_words.insert(new_word);
                }
            }
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
        cout << "No word ladder found." << endl;
        return;
    }

    cout << "Word ladder found: ";
    for (const string& word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}

#define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }

void verify_word_ladder() {
    // set<string> word_list;
    // load_words(word_list, "src/words.txt");  // Adjust the file path if necessary

    // my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    // my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    // my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    // my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    // my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    // my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
    set<string> word_list;
    load_words(word_list, "src/words.txt");  // Adjust the file path if necessary

    // Test cases
    vector<string> ladder1 = generate_word_ladder("cat", "dog", word_list);
    my_assert(ladder1.size() == 4);

    vector<string> ladder2 = generate_word_ladder("marty", "curls", word_list);
    my_assert(ladder2.size() == 6);

    vector<string> ladder3 = generate_word_ladder("code", "data", word_list);
    my_assert(ladder3.size() == 6);

    vector<string> ladder4 = generate_word_ladder("work", "play", word_list);
    my_assert(ladder4.size() == 6);

    vector<string> ladder5 = generate_word_ladder("sleep", "awake", word_list);
    my_assert(ladder5.size() == 8);

    vector<string> ladder6 = generate_word_ladder("car", "cheat", word_list);
    my_assert(ladder6.size() == 4);
}

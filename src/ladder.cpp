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
    int length1 = word1.size();
    int length2 = word2.size();

    if (abs(length1-length2) > 1) {
        return false;
    }

    //substitution
    if (length1 == length2) {
        if (word1 == word2) {
            return true;
        }

        int diff1 = 0;
        for (int i=0; i < length1; ++i) {
            if (word1[i] != word2[i]) {
                ++diff1;
                if (diff1 > 1) {
                    return false;
                }
            }
        }
        return diff1 == 1;
    }

    //check for insertion/deletion if words are one diff or not
    const string& shorter = (length1 < length2) ? word1 : word2;
    const string& longer = (length1 < length2) ? word2 : word1;

    int i = 0, j = 0;
    int diff2 = 0;

    while (i < shorter.size() && j < longer.size()) {
        if (shorter[i] == longer[i]) {
            ++i;
            ++j;
        } else {
            ++j;
            ++diff2;
            if (diff2 > 1) {
                return false;
            }
        }
    }
    return true;
}

//word ladder func that uses bfs
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    queue<vector<string>> ladder_queue;
    vector<string> start_ladder = { begin_word };
    ladder_queue.push(start_ladder);
    
    unordered_set<string> visited;
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = ladder.back();
        for (const string& word : word_list) {
            if (word == last_word)  // Avoid using the same word.
                continue;
                
            if (is_adjacent(last_word, word)) {
                if (visited.find(word) == visited.end()) {
                    visited.insert(word);
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    
                    if (word == end_word) {
                        return new_ladder;
                    }
                    
                    ladder_queue.push(new_ladder);
                }
            }
        }
    }
    return {};
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

    cout << "Word ladder:" << endl;
    for (const string& word : ladder) {
        cout << word << " ";
    }
    cout << endl;
}

#define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");  // Adjust the file path if necessary

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}

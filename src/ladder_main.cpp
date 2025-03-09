#include "ladder.h"
#include <algorithm>

int main() {
    set<string> word_list;
    load_words(word_list, "src/words.txt"); // Load the dictionary

    string begin_word, end_word;

    // Get user input for start and end words
    cout << "Enter the start word: ";
    cin >> begin_word;
    cout << "Enter the end word: ";
    cin >> end_word;

    // Convert both words to lowercase so that letter cases are ignored.
    transform(begin_word.begin(), begin_word.end(), begin_word.begin(), ::tolower);
    transform(end_word.begin(), end_word.end(), end_word.begin(), ::tolower);

    if (word_list.find(begin_word) != word_list.end()) {
        cerr << "Error: The start word must not be in the dictionary." << endl;
        return 1;
    }

     if (word_list.find(end_word) == word_list.end()) {
        cerr << "Error: The end word must be in the dictionary." << endl;
        return 1;
    }

    // Generate the word ladder
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);

    // Print the word ladder
    print_word_ladder(ladder);

    // Verify the word ladder
    verify_word_ladder();

    return 0;
    
}
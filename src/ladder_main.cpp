#include "ladder.h"

int main() {
    set<string> word_list;
    load_words(word_list, "src/words.txt"); // Load the dictionary

    string begin_word, end_word;

    // Get user input for start and end words
    cout << "Enter the start word: ";
    cin >> begin_word;
    cout << "Enter the end word: ";
    cin >> end_word;

    // Generate the word ladder
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);

    // Print the word ladder
    print_word_ladder(ladder);

    // Verify the word ladder
    verify_word_ladder();

    return 0;
    
}
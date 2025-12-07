#include "include/WordPieceTokenizer.hpp"
#include <iostream>
#include <string>
#include <vector>

/**
 * Main function demonstrating WordPiece Tokenizer usage
 */
int main()
{
	WordPieceTokenizer tokenizer;
	
	std::cout << "=== WordPiece Tokenizer Demo ===" << std::endl;
	std::cout << std::endl;

	// Train the tokenizer
	std::cout << "Training tokenizer from file.txt..." << std::endl;
	if (!tokenizer.train_from_file("file.txt", 2000))
	{
		std::cerr << "Error: Training failed!" << std::endl;
		return (1);
	}
	std::cout << "Training completed!" << std::endl;
	std::cout << std::endl;

	// Save vocabulary
	std::cout << "Saving vocabulary to vocab.txt..." << std::endl;
	std::cout << "Vocabulary size: " << tokenizer.get_vocab_size() << " tokens" << std::endl;
	if (!tokenizer.save_vocab("vocab.txt"))
	{
		std::cerr << "Error: Failed to save vocabulary!" << std::endl;
		return (1);
	}
	std::cout << "Vocabulary saved!" << std::endl;
	std::cout << std::endl;

	// Test tokenization
	std::cout << "=== Tokenization Tests ===" << std::endl;
	std::cout << std::endl;

	// Test 1: Normal sentence
	std::string sentence1 = "Hello world, this is a test.";
	std::cout << "Input: \"" << sentence1 << "\"" << std::endl;
	
	std::vector<std::string> tokens1 = tokenizer.tokenize(sentence1);
	std::cout << "Tokens: ";
	for (const std::string& token: tokens1)
	{
		std::cout << token << " ";
	}
	std::cout << std::endl;

	std::vector<int> ids1 = tokenizer.tokens_to_ids(tokens1);
	std::cout << "Token IDs: ";
	for (int id: ids1)
	{
		std::cout << id << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// Test 2: Another sentence
	std::string sentence2 = "Tokenization is amazing!";
	std::cout << "Input: \"" << sentence2 << "\"" << std::endl;
	
	std::vector<std::string> tokens2 = tokenizer.tokenize(sentence2);
	std::cout << "Tokens: ";
	for (const std::string& token: tokens2)
	{
		std::cout << token << " ";
	}
	std::cout << std::endl;

	std::vector<int> ids2 = tokenizer.tokens_to_ids(tokens2);
	std::cout << "Token IDs: ";
	for (int id: ids2)
	{
		std::cout << id << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// Test 3: Edge case - empty string
	std::string sentence3 = "";
	std::cout << "Input: \"" << sentence3 << "\" (empty)" << std::endl;
	std::vector<std::string> tokens3 = tokenizer.tokenize(sentence3);
	std::cout << "Tokens count: " << tokens3.size() << std::endl;
	std::cout << std::endl;

	// Test 4: Single character
	std::string sentence4 = "a";
	std::cout << "Input: \"" << sentence4 << "\" (single char)" << std::endl;
	std::vector<std::string> tokens4 = tokenizer.tokenize(sentence4);
	std::cout << "Tokens: ";
	for (const std::string& token: tokens4)
	{
		std::cout << token << " ";
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// Demonstrate ID to Token conversion
	std::cout << "=== ID to Token Conversion ===" << std::endl;
	std::vector<std::string> recovered_tokens = tokenizer.ids_to_tokens(ids1);
	std::cout << "Recovered tokens from IDs: ";
	for (const std::string& token: recovered_tokens)
	{
		std::cout << token << " ";
	}
	std::cout << std::endl;

	return (0);
}

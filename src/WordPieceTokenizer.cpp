#include "../include/WordPieceTokenizer.hpp"

WordPieceTokenizer::WordPieceTokenizer()
{
}

/**
 * Save vocabulary to file
 * @param path: Path to save the vocabulary
 * @return true if successful, false otherwise
 */
bool WordPieceTokenizer::save_vocab(const std::string& path) const
{
	std::ofstream	file(path);

	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open file for writing: " << path << std::endl;
		return (false);
	}
	// Fixed: Use const reference instead of copying
	for (const std::string& token: _vocab)
	{
		file << token << "\n";
	}
	file.close();
	return (true);
}

/**
 * Load vocabulary from file
 * @param path: Path to load the vocabulary from
 * @return true if successful, false otherwise
 */
bool WordPieceTokenizer::load_vocab(const std::string& path)
{
	std::ifstream	file(path);
	std::string		line;

	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open file for reading: " << path << std::endl;
		return (false);
	}
	_vocab.clear();
	_token_to_id.clear();
	_id_to_token.clear();

	int id = 0;
	while (std::getline(file, line))
	{
		if (!line.empty())
		{
			_vocab.push_back(line);
			_token_to_id[line] = id;
			_id_to_token[id] = line;
			id++;
		}
	}
	file.close();
	return (true);
}

/**
 * Merge a pair of tokens in the corpus
 * Fixed: Proper ## prefix logic based on position in word
 * @param pair: The pair to merge (format: "token1 token2")
 * @param corpus: The corpus to update
 */
void WordPieceTokenizer::merge_pair(const std::string& pair, std::vector<std::vector<std::string>>& corpus)
{
	size_t		split_idx;
	std::string	left;
	std::string	right;

	split_idx = pair.find(' ');
	
	// Fixed: Add validation for invalid pair format
	if (split_idx == std::string::npos)
	{
		std::cerr << "Error: Invalid pair format: " << pair << std::endl;
		return;
	}
	
	left = pair.substr(0, split_idx);
	right = pair.substr(split_idx + 1);

	for (std::vector<std::string>& word: corpus)
	{
		std::vector<std::string> new_word;
		for (size_t i = 0; i < word.size();)
		{
			if (i + 1 < word.size() && word[i] == left && word[i + 1] == right)
			{
				// Fixed: Proper ## prefix handling
				// Remove ## from both parts if present
				std::string clean_left = left;
				std::string clean_right = right;
				
				if (left.length() >= 2 && left.substr(0, 2) == "##")
					clean_left = left.substr(2);
				if (right.length() >= 2 && right.substr(0, 2) == "##")
					clean_right = right.substr(2);
				
				std::string clean_merged = clean_left + clean_right;
				std::string merged_token;
				
				// Add ## prefix if this merged token is NOT at word start
				if (i != 0)
					merged_token = "##" + clean_merged;
				else
					merged_token = clean_merged;
				
				new_word.push_back(merged_token);
				i += 2;
			}
			else
			{
				new_word.push_back(word[i]);
				i++;
			}
		}
		word = std::move(new_word);
	}
}

/**
 * Count all adjacent token pairs in the corpus
 * @param corpus: The corpus to analyze
 * @return Map of pairs to their frequencies
 */
std::unordered_map<std::string, int> WordPieceTokenizer::count_pairs(const std::vector<std::vector<std::string>>& corpus) const
{
	std::unordered_map<std::string, int> pairs;

	for (const std::vector<std::string>& word: corpus)
	{
		for (size_t i = 0; i < word.size() - 1; i++)
		{
			// Fixed: More efficient string concatenation
			std::string pair;
			pair.reserve(word[i].size() + word[i + 1].size() + 1);
			pair = word[i] + " " + word[i + 1];
			pairs[pair]++;
		}
	}
	return (pairs);
}

/**
 * Split text into words (whitespace-separated)
 * @param text: Input text to split
 * @return Vector of words
 */
std::vector<std::string> WordPieceTokenizer::split_words(const std::string& text) const
{
	std::vector<std::string>	words;
	std::istringstream			iss(text);
	std::string					word;

	while (iss >> word)
	{
		words.push_back(word);
	}
	return (words);
}

/**
 * Build initial corpus with character-level tokens
 * Characters at word start have no prefix, others get ## prefix
 * @param words: Vector of words to process
 * @return Initial corpus with character-level tokenization
 */
std::vector<std::vector<std::string>> WordPieceTokenizer::build_initial_corpus(const std::vector<std::string>& words) const
{
	std::vector<std::vector<std::string>> corpus;

	for (const std::string& word: words)
	{
		std::vector<std::string> chars;
		for (size_t i = 0; i < word.length(); i++)
		{
			std::string c(1, word[i]);
			// First character has no prefix, others get ##
			if (i == 0)
				chars.push_back(c);
			else
				chars.push_back("##" + c);
		}
		if (!chars.empty())
			corpus.push_back(chars);
	}
	return (corpus);
}

/**
 * Initialize vocabulary from corpus (extract all unique tokens)
 * @param corpus: The corpus to extract tokens from
 */
void WordPieceTokenizer::init_vocab_from_corpus(std::vector<std::vector<std::string>>& corpus)
{
	std::unordered_map<std::string, bool> unique_tokens;

	// Add special tokens first
	_vocab.clear();
	_vocab.push_back("[UNK]");
	_vocab.push_back("[PAD]");
	_vocab.push_back("[CLS]");
	_vocab.push_back("[SEP]");
	_vocab.push_back("[MASK]");

	for (const std::vector<std::string>& word: corpus)
	{
		for (const std::string& token: word)
		{
			unique_tokens[token] = true;
		}
	}

	for (const auto& pair: unique_tokens)
	{
		_vocab.push_back(pair.first);
	}

	// Rebuild token-to-id mappings
	_token_to_id.clear();
	_id_to_token.clear();
	for (size_t i = 0; i < _vocab.size(); i++)
	{
		_token_to_id[_vocab[i]] = static_cast<int>(i);
		_id_to_token[static_cast<int>(i)] = _vocab[i];
	}
}

/**
 * Train the tokenizer on provided text
 * @param text: Training corpus text
 * @param vocab_size: Target vocabulary size
 * @return true if successful, false otherwise
 */
bool WordPieceTokenizer::train(const std::string& text, int vocab_size)
{
	std::vector<std::string>				words;
	std::vector<std::vector<std::string>>	corpus;

	// Input validation
	if (text.empty())
	{
		std::cerr << "Error: Training text is empty" << std::endl;
		return (false);
	}
	
	if (vocab_size <= 0)
	{
		std::cerr << "Error: Vocabulary size must be positive" << std::endl;
		return (false);
	}

	words = split_words(text);
	
	if (words.empty())
	{
		std::cerr << "Error: No words found in training text" << std::endl;
		return (false);
	}
	
	corpus = build_initial_corpus(words);
	init_vocab_from_corpus(corpus);

	if (corpus.empty())
	{
		std::cerr << "Error: Failed to build initial corpus" << std::endl;
		return (false);
	}

	while (_vocab.size() < static_cast<size_t>(vocab_size))
	{
		std::unordered_map<std::string, int> pairs = count_pairs(corpus);
		
		if (pairs.empty())
			break;

		// Find most frequent pair
		std::string best_pair;
		int max_freq = 0;
		// Fixed: Use const reference
		for (const auto& pair: pairs)
		{
			if (pair.second > max_freq)
			{
				max_freq = pair.second;
				best_pair = pair.first;
			}
		}

		merge_pair(best_pair, corpus);
		init_vocab_from_corpus(corpus);

		// Progress reporting
		if (_vocab.size() % 100 == 0)
		{
			std::cout << "Vocabulary size: " << _vocab.size() << " / " << vocab_size << std::endl;
		}
	}

	return (true);
}

/**
 * Train tokenizer from file
 * @param filepath: Path to training corpus file
 * @param vocab_size: Target vocabulary size
 * @return true if successful, false otherwise
 */
bool WordPieceTokenizer::train_from_file(const std::string& filepath, int vocab_size)
{
	std::ifstream	file(filepath);
	std::string		line;
	std::string		text;

	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open file: " << filepath << std::endl;
		return (false);
	}

	// Read entire file and convert to lowercase
	while (std::getline(file, line))
	{
		// Convert to lowercase for consistency
		for (char& c: line)
		{
			c = std::tolower(static_cast<unsigned char>(c));
		}
		text += line + " ";
	}
	file.close();

	return (train(text, vocab_size));
}

/**
 * Tokenize a single word using the learned vocabulary
 * Uses greedy longest-match-first algorithm
 * @param word: Word to tokenize
 * @return Vector of subword tokens
 */
std::vector<std::string> WordPieceTokenizer::tokenize_word(const std::string& word) const
{
	std::vector<std::string> tokens;

	if (word.empty())
		return (tokens);

	size_t start = 0;
	while (start < word.length())
	{
		size_t end = word.length();
		std::string sub;
		bool found = false;

		// Greedy longest-match-first
		while (start < end)
		{
			sub = word.substr(start, end - start);
			// Add ## prefix for non-word-initial subwords
			if (start != 0)
				sub = "##" + sub;

			// Fixed: More efficient lookup using find()
			auto it = _token_to_id.find(sub);
			if (it != _token_to_id.end())
			{
				tokens.push_back(sub);
				found = true;
				break;
			}
			end--;
		}

		if (!found)
		{
			tokens.push_back("[UNK]");
			start++;
		}
		else
		{
			// Fixed: Calculate actual character length correctly
			// If token starts with ##, remove 2 chars from length
			size_t actual_length = sub.length();
			if (sub.length() >= 2 && sub[0] == '#' && sub[1] == '#')
				actual_length -= 2;
			start += actual_length;
		}
	}

	return (tokens);
}

/**
 * Tokenize input text into subword tokens
 * @param text: Input text to tokenize
 * @return Vector of tokens
 */
std::vector<std::string> WordPieceTokenizer::tokenize(const std::string& text) const
{
	std::vector<std::string>	tokens;
	std::vector<std::string>	words;
	std::string					preprocessed_text;

	if (text.empty())
		return (tokens);
	
	// Check if tokenizer is trained
	if (!is_trained())
	{
		std::cerr << "Warning: Tokenizer not trained. Please train or load vocabulary first." << std::endl;
		return (tokens);
	}

	// Preprocess: convert to lowercase
	preprocessed_text = text;
	for (char& c: preprocessed_text)
	{
		c = std::tolower(static_cast<unsigned char>(c));
	}

	words = split_words(preprocessed_text);

	for (const std::string& word: words)
	{
		std::vector<std::string> word_tokens = tokenize_word(word);
		tokens.insert(tokens.end(), word_tokens.begin(), word_tokens.end());
	}

	return (tokens);
}

/**
 * Convert tokens to their integer IDs
 * @param tokens: Vector of token strings
 * @return Vector of token IDs
 */
std::vector<int> WordPieceTokenizer::tokens_to_ids(const std::vector<std::string>& tokens) const
{
	std::vector<int> ids;

	for (const std::string& token: tokens)
	{
		// Fixed: More efficient lookup
		auto it = _token_to_id.find(token);
		if (it != _token_to_id.end())
			ids.push_back(it->second);
		else
			ids.push_back(_unk_id);
	}

	return (ids);
}

/**
 * Convert token IDs back to token strings
 * @param ids: Vector of token IDs
 * @return Vector of token strings
 */
std::vector<std::string> WordPieceTokenizer::ids_to_tokens(const std::vector<int>& ids) const
{
	std::vector<std::string> tokens;

	for (int id: ids)
	{
		auto it = _id_to_token.find(id);
		if (it != _id_to_token.end())
			tokens.push_back(it->second);
		else
			tokens.push_back("[UNK]");
	}

	return (tokens);
}

/**
 * Get the current vocabulary size
 * @return Size of the vocabulary
 */
size_t WordPieceTokenizer::get_vocab_size() const noexcept
{
	return (_vocab.size());
}

/**
 * Check if the tokenizer has been trained or loaded
 * @return true if vocabulary is available, false otherwise
 */
bool WordPieceTokenizer::is_trained() const noexcept
{
	return (!_vocab.empty() && !_token_to_id.empty());
}

#ifndef WORDPIECE_TOKENIZER_HPP
# define WORDPIECE_TOKENIZER_HPP

# include <string>
# include <unordered_map>
# include <vector>
# include <fstream>
# include <iostream>
# include <sstream>
# include <utility>

/**
 * @class WordPieceTokenizer
 * @brief A C++ implementation of the WordPiece tokenization algorithm
 * 
 * This tokenizer is used in models like BERT. It learns a vocabulary
 * of subword units from training text and can tokenize new text using
 * this vocabulary. The algorithm uses a greedy longest-match-first approach.
 */
class WordPieceTokenizer
{
	public:
		// Constructor
		WordPieceTokenizer();

		// Vocabulary I/O
		/**
		 * @brief Load vocabulary from a file
		 * @param path Path to the vocabulary file
		 * @return true if successful, false otherwise
		 */
		bool									load_vocab(const std::string& path);
		
		/**
		 * @brief Save vocabulary to a file
		 * @param path Path to save the vocabulary
		 * @return true if successful, false otherwise
		 */
		bool									save_vocab(const std::string& path) const;

		// Training
		/**
		 * @brief Train tokenizer from a text file
		 * @param filepath Path to training corpus file
		 * @param vocab_size Target vocabulary size
		 * @return true if successful, false otherwise
		 */
		bool									train_from_file(const std::string& filepath, int vocab_size);
		
		/**
		 * @brief Train tokenizer from text string
		 * @param text Training corpus text
		 * @param vocab_size Target vocabulary size
		 * @return true if successful, false otherwise
		 */
		bool									train(const std::string& text, int vocab_size);

		// Tokenization
		/**
		 * @brief Tokenize text into subword tokens
		 * @param text Input text to tokenize
		 * @return Vector of tokens
		 */
		std::vector<std::string>				tokenize(const std::string& text) const;
		
		/**
		 * @brief Convert tokens to their integer IDs
		 * @param tokens Vector of token strings
		 * @return Vector of token IDs
		 */
		std::vector<int>						tokens_to_ids(const std::vector<std::string>& tokens) const;
		
		/**
		 * @brief Convert token IDs back to token strings
		 * @param ids Vector of token IDs
		 * @return Vector of token strings
		 */
		std::vector<std::string>				ids_to_tokens(const std::vector<int>& ids) const;
		
		/**
		 * @brief Get the current vocabulary size
		 * @return Size of the vocabulary
		 */
		size_t									get_vocab_size() const noexcept;
		
		/**
		 * @brief Check if the tokenizer has been trained or loaded
		 * @return true if vocabulary is available, false otherwise
		 */
		bool									is_trained() const noexcept;

	private:
		// Helper methods
		/**
		 * @brief Split text into words by whitespace
		 * @param text Input text
		 * @return Vector of words
		 */
		std::vector<std::string>				split_words(const std::string& text) const;
		
		/**
		 * @brief Count all adjacent token pairs in corpus
		 * @param corpus The corpus to analyze
		 * @return Map of pairs to their frequencies
		 */
		std::unordered_map<std::string, int>	count_pairs(const std::vector<std::vector<std::string>>& corpus) const;
		
		/**
		 * @brief Merge a specific pair of tokens in the corpus
		 * @param pair The pair to merge (format: "token1 token2")
		 * @param corpus The corpus to update
		 */
		void 									merge_pair(const std::string& pair, std::vector<std::vector<std::string>>& corpus);
		
		/**
		 * @brief Tokenize a single word using learned vocabulary
		 * @param word Word to tokenize
		 * @return Vector of subword tokens
		 */
		std::vector<std::string>				tokenize_word(const std::string& word) const;
		
		/**
		 * @brief Build initial corpus with character-level tokens
		 * @param words Vector of words to process
		 * @return Initial corpus with character-level tokenization
		 */
		std::vector<std::vector<std::string>>	build_initial_corpus(const std::vector<std::string>& words) const;
		
		/**
		 * @brief Initialize vocabulary from corpus tokens
		 * @param corpus The corpus to extract tokens from
		 */
		void									init_vocab_from_corpus(std::vector<std::vector<std::string>>& corpus);

	private:
		// Member variables
		std::vector<std::string>				_vocab;          ///< Vocabulary list (index is token ID)
		std::unordered_map<std::string, int>	_token_to_id;    ///< Token to ID mapping
		int										_unk_id{0};      ///< ID for unknown tokens
};

# endif
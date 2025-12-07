#ifndef WORDPIECE_TOKENIZER_HPP
# define WORDPIECE_TOKENIZER_HPP

# include <string>
# include <unordered_map>
# include <vector>
# include <fstream>
# include <iostream>
# include <sstream>
# include <utility>

class WordPieceTokenizer
{
	public:
		// constructor
		WordPieceTokenizer();

		// Vocabulary I/O
		bool									load_vocab(const std::string& path);
		bool									save_vocab(const std::string& path) const;

		// train
		bool									train_from_file(const std::string& filepath, int vocab_size);
		bool									train(const std::string& text, int vocab_size);

		// Tokenize
		std::vector<std::string>				tokenize(const std::string& text) const;
		std::vector<int>						tokens_to_ids(const std::vector<std::string>& tokens) const;
		std::vector<std::string>				ids_to_tokens(const std::vector<int>& ids) const;

	private:
		std::vector<std::string>				split_words(const std::string& text) const;
		std::unordered_map<std::string, int>	count_pairs(const std::vector<std::vector<std::string>>& corpus) const;
		void 									merge_pair(const std::string& pair, std::vector<std::vector<std::string>>& corpus);
		std::vector<std::string>				tokenize_word(const std::string& word) const;
		std::vector<std::vector<std::string>>	build_initial_corpus(const std::vector<std::string>& words) const;
		void									init_vocab_from_corpus(std::vector<std::vector<std::string>>& corpus);

	private:
		std::vector<std::string>				_vocab;
		std::unordered_map<std::string, int>	token_to_id_;
		std::unordered_map<int, std::string>	id_to_token_;
		int										unk_id_{0};
};

# endif
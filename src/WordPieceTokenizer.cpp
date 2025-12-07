#include "../include/WordPieceTokenizer.hpp"

WordPieceTokenizer::WordPieceTokenizer():unk_id_(0)
{
	token_to_id_.clear();
	_vocab.clear();
}

bool	WordPieceTokenizer::load_vocab(const std::string& file_path)
{
	std::ifstream		file(file_path);
	std::string			token;

	if (!file)
	{
		std::cerr << "Error opening file: " << file_path << " to read!\n";
		return (false);
	}
	this->_vocab.clear();
	this->token_to_id_.clear();
	while (std::getline(file, token))
	{
		if (!token.empty())
			this->_vocab.push_back(token);
	}
	for (size_t i=0; i < _vocab.size(); i++)
	{
		token_to_id_[_vocab[i]] = static_cast<int>(i);
	}
	file.close();
	return (true);
}

bool	WordPieceTokenizer::save_vocab(const std::string& file_path) const
{
	std::ofstream		file(file_path);
	
	if (!file)
	{
		std::cerr << "Error opening file: " << file_path << " to write!\n";
		return (false);
	}
	for (const std::string token: this->_vocab)
	{
		file << token << "\n";
	}
	file.close();
	return (true);
}

std::vector<std::string>	WordPieceTokenizer::split_words(const std::string& text) const
{
	std::vector<std::string>	tokens;
	std::stringstream			stext(text);
	std::string					token;

	while (stext >> token)
	{
		if (!token.empty())
			tokens.push_back(token);
	}
	return (tokens);
}

std::unordered_map<std::string, int>	WordPieceTokenizer::count_pairs(const std::vector<std::vector<std::string>>& corpus) const
{
	std::unordered_map<std::string, int>	pair_count;

	for (const std::vector<std::string>& word: corpus)
	{
		for (size_t i=0; i + 1 < word.size(); i++)
		{
			std::string pair = word[i] + " " + word[i+1];
			pair_count[pair]++;
		}
	}
	return (pair_count);
}

void	WordPieceTokenizer::merge_pair(const std::string& pair, std::vector<std::vector<std::string>>& corpus)
{
	size_t			split_idx = pair.find(' ');
	std::string		left = pair.substr(0, split_idx);
	std::string		right = pair.substr(split_idx + 1);
	std::string		merged = left + right;


	for (std::vector<std::string>& word: corpus)
	{
		std::vector<std::string>	new_word;
		for (size_t i = 0; i < word.size();)
		{
			if (i + 1 < word.size() && word[i] == left && word[i+1] == right)
			{
				new_word.push_back(merged);
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

std::vector<std::string>	WordPieceTokenizer::tokenize_word(const std::string& word) const
{
	std::vector<std::string>	sub_tokens;
	size_t						word_len = word.size();
	size_t						start{0};

	while (start < word_len)
	{
		size_t		end = word_len;
		std::string	current_substr;
		bool		matched{false};
		while (end > start)
		{
			std::string		sub = word.substr(start, end - start);

			if (start > 0)
				sub = "##" + sub;
			if (this->token_to_id_.count(sub))
			{
				matched = true;
				current_substr = sub;
				break;
			}
			end--;
		}
		if (!matched)
		{
			sub_tokens.push_back(this->_vocab[this->unk_id_]);
			return (sub_tokens);
		}
		sub_tokens.push_back(current_substr);
		start = end;
	}
	return (sub_tokens);
}

std::vector<std::vector<std::string>>	WordPieceTokenizer::build_initial_corpus(const std::vector<std::string>& words) const
{
	std::vector<std::vector<std::string>>	corpus;

	for (const std::string& word: words)
	{
		std::vector<std::string>	chars;
		for (char c: word)
		{
			chars.push_back(std::string(1, c));
		}
		corpus.push_back(chars);
	}
	return (corpus);
}

void	WordPieceTokenizer::init_vocab_from_corpus(std::vector<std::vector<std::string>>& corpus)
{
	size_t	idx{1};

	_vocab.clear();
	token_to_id_.clear();
	_vocab.push_back("[UNK]");
	token_to_id_["[UNK]"] = 0;
	for (std::vector<std::string>& word: corpus)
	{
		for (std::string& token: word)
		{
			if (!token_to_id_.count(token))
			{
				_vocab.push_back(token);
				token_to_id_[token] = idx++;
			}
		}
	}
}

std::vector<std::string>	WordPieceTokenizer::tokenize(const std::string& text) const
{
	std::vector<std::string>	sub_tokens;
	std::vector<std::string>	words = split_words(text);

	for (std::string word: words)
	{
		std::vector<std::string>	word_sub_tokens = tokenize_word(word);
		sub_tokens.insert(sub_tokens.end(), word_sub_tokens.begin(), word_sub_tokens.end());
	}
	return (sub_tokens);
}

std::vector<int>	WordPieceTokenizer::tokens_to_ids(const std::vector<std::string>& tokens) const
{
	std::vector<int>	ids;

	for (const std::string& token: tokens)
	{
		auto it = token_to_id_.find(token);
		if (it != token_to_id_.end())
			ids.push_back(it->second);
		else
			ids.push_back(unk_id_);
	}
	return(ids);
}

std::vector<std::string>	WordPieceTokenizer::ids_to_tokens(const std::vector<int>& ids) const
{
	std::vector<std::string>	tokens;

	for (const int& id: ids)
	{
		auto it = tokens_to_ids()
	}
	return (tokens);
}
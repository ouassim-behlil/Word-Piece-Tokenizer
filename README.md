# WordPiece Tokenizer

A simple C++ implementation of the WordPiece tokenization algorithm used in models like BERT.

## Overview

This project provides a WordPiece tokenizer that can:
- Train a vocabulary from text files
- Tokenize text into subword units
- Convert tokens to IDs and vice versa
- Save and load vocabularies

## What is WordPiece Tokenization?

WordPiece is a subword tokenization algorithm that:
1. Starts with a character-level vocabulary
2. Iteratively merges the most frequent adjacent token pairs
3. Uses a greedy longest-match-first approach for tokenization
4. Handles out-of-vocabulary words by breaking them into known subwords

This approach balances vocabulary size with the ability to handle rare or unseen words.

## Quick Start

### Build

```bash
make
```

### Run

```bash
./tokenizer
```

### Clean

```bash
make clean      # Remove build artifacts
make fclean     # Remove build artifacts and vocabulary
```

## Usage Example

```cpp
#include "WordPieceTokenizer.hpp"


int main() {
    WordPieceTokenizer tokenizer;
    
    // Train from file
    tokenizer.train_from_file("file.txt", 1000);
    tokenizer.save_vocab("vocab.txt");
    
    // Tokenize text
    auto tokens = tokenizer.tokenize("Hello World");
    
    // Convert to IDs
    auto ids = tokenizer.tokens_to_ids(tokens);
    
    // Convert back to tokens
    auto recovered = tokenizer.ids_to_tokens(ids);
    
    return 0;
}
```

## API Reference

### Training

- `bool train_from_file(const std::string& filepath, int vocab_size)` - Train tokenizer from a text file
- `bool train(const std::string& text, int vocab_size)` - Train tokenizer from a text string

### Tokenization

- `std::vector<std::string> tokenize(const std::string& text)` - Tokenize input text into subword tokens
- `std::vector<int> tokens_to_ids(const std::vector<std::string>& tokens)` - Convert tokens to IDs
- `std::vector<std::string> ids_to_tokens(const std::vector<int>& ids)` - Convert IDs back to tokens

### Vocabulary Management

- `bool save_vocab(const std::string& path)` - Save vocabulary to file
- `bool load_vocab(const std::string& path)` - Load vocabulary from file

## Special Tokens

The tokenizer includes these special tokens:
- `[UNK]` - Unknown token (for out-of-vocabulary items)
- `[PAD]` - Padding token
- `[CLS]` - Classification token
- `[SEP]` - Separator token
- `[MASK]` - Mask token (for masked language modeling)

## How It Works

1. **Initialization**: Text is split into words and characters
2. **Training**: The algorithm iteratively merges the most frequent adjacent pairs
3. **Tokenization**: New text is tokenized using a greedy longest-match-first approach
4. **Subword Prefix**: Tokens that don't start a word are prefixed with `##`

## Example

Input: "Hello World"

After training, this might tokenize to:
- `["Hello", "##W", "##o", "##r", "##l", "##d"]` (if "Hello" is common but "World" is rare)
- Or `["Hello", "World"]` (if both are common in training data)

## Requirements

- C++17 compatible compiler
- Make

## Makefile Targets

- `make` or `make all` - Build the project
- `make debug` - Build with debug symbols and AddressSanitizer
- `make release` - Build with optimizations
- `make clean` - Remove build artifacts
- `make fclean` - Remove all generated files including vocabulary
- `make re` - Rebuild from scratch
- `make run` - Build and run the program
- `make valgrind` - Run with valgrind memory checker

## Learning Resources

This implementation is designed for educational purposes to understand:
- Subword tokenization algorithms
- Byte Pair Encoding (BPE) variants
- BERT-style preprocessing
- C++ best practices

## License

Educational purposes.

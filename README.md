# WordPiece Tokenizer

A simple C++ implementation of the WordPiece tokenization algorithm used in models like BERT.

## Overview

This project provides a WordPiece tokenizer that can:
- Train a vocabulary from text files
- Tokenize text into subword units
- Convert tokens to IDs and vice versa
- Save and load vocabularies

## Quick Start

### Build

```bash
make
```

### Run

```bash
./tokenizer
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
    
    return 0;
}
```

## Requirements

- C++17 compatible compiler
- Make

## License

Educational purposes.

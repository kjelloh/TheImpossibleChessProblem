//
//  main.cpp
//  TheImpossibleChessProblem
//
//  Created by Kjell-Olov Högdal on 2020-07-26.
//  Copyright © 2020 Kjell-Olov Högdal. All rights reserved.
//

#include <iostream>
#include <vector>
#include <bitset>
#include <random>

struct chess_board_square {
    unsigned char index;
    bool head;
};

using ChessBoard = std::vector<chess_board_square>;

void flip_coin(ChessBoard& chess_board,int index) {
    chess_board[index].head = chess_board[index].head xor 1;
}

void print_chess_board(ChessBoard const& chess_board) {
    for (unsigned char i = 0; i < 64 ; ++i) {
        if (not (i % 8)) {
            std::cout << "\n\t";
        }
        std::cout << " " << (chess_board[i].head?"H":"T");
    }
}

using Encoding = std::bitset<6>;

unsigned char column_of(unsigned char index) {return index % 8;};
unsigned char row_of(unsigned char index) {return index / 8;};

bool is_odd(unsigned char index) {return (index % 2) > 0;};
bool is_odd_pair(unsigned char index) {return is_odd(index / 2);};
bool is_odd_quadruple(unsigned char index) {return is_odd(index / 4);};

bool is_odd_column(unsigned char index) {return is_odd(column_of(index));};
bool is_odd_column_pair(unsigned char index) {return is_odd_pair(column_of(index));};
bool is_odd_column_quadruple(unsigned char index) {return is_odd_quadruple(column_of(index));};
bool is_odd_row(unsigned char index) {return is_odd(row_of(index));};
bool is_odd_row_pair(unsigned char index) {return is_odd_pair(row_of(index));};
bool is_odd_row_quadruple(unsigned char index) {return is_odd_quadruple(row_of(index));};

Encoding decode_board(ChessBoard const& chess_board) {
    Encoding result;
    for (unsigned char i = 0; i < 64 ; ++i) {
        // Decode the chess board
        // b0 = parity or odd columns 1,3,5,7
        result[0] = is_odd_column(i) ? result[0] xor chess_board[i].head : result[0];
        // b1 = parity of odd column pairs 2,3 6,7
        result[1] = is_odd_column_pair(i) ? result[1] xor chess_board[i].head : result[1];
        // b2 = parity of of odd column quadruples
        result[2] = is_odd_column_quadruple(i) ? result[2] xor chess_board[i].head : result[2];
        // b3 = parity of odd rows
        result[3] = is_odd_row(i) ? result[3] xor chess_board[i].head: result[3];
        // b4 = parity of odd row pairs
        result[4] = is_odd_row_pair(i) ? result[4] xor chess_board[i].head: result[4];
        // b5 = parity of odd row quadruples
        result[5] = is_odd_row_quadruple(i) ? result[5] xor chess_board[i].head: result[5];
    }
    return result;
}

void print_encoding(Encoding const& encoding) {
    std::cout << encoding.to_string();
}

void init_chess_board(ChessBoard& chess_board) {

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 64);
    
       for (int n=0; n<10; ++n)
           //Use `distrib` to transform the random unsigned int generated by gen into an int in [1, 6]
           std::cout << distrib(gen) << ' ';
       std::cout << '\n';

    // Randomized board
    for (unsigned char i = 0; i < 64; ++i) {
        chess_board.push_back({i,distrib(gen) > 32});
    }

}

Encoding flipping_pattern_for(ChessBoard const& chess_board,unsigned char key_position) {
    Encoding result;
    result = decode_board(chess_board).to_ulong() xor key_position;
    return result;
}

bool test_board(ChessBoard const& chess_board) {
    bool result = false;
    std::cout << "\n<TEST of Board>";
    print_chess_board(chess_board);
    std::cout << "\n\nEncodes:";
    print_encoding(decode_board(chess_board));
    // test key positions 0..63
    // for each test we can encode the key position with a coin flip
    std::cout << "\n";
    for (unsigned char key_position = 0; key_position < 64; ++key_position) {
        std::cout << "\nKey Position:" << static_cast<int>(key_position);
        Encoding flipping_pattern = flipping_pattern_for(chess_board,key_position);
        std::cout << "\n\t" << "Required Flipping Pattern:";
        print_encoding(flipping_pattern);
        // Flip the coin with index as required by flipping pattern
        std::cout << "\n\t" << "Flipping Coin " << flipping_pattern.to_ulong() << " (Pattern " << flipping_pattern << ")";
        ChessBoard test_board = chess_board;
        flip_coin(test_board,static_cast<unsigned char>(flipping_pattern.to_ulong()));
        std::cout << "\n\t" << "Board now encodes:";
        auto current_encoding = decode_board(test_board);
        print_encoding(current_encoding);
        std::cout << " Test " << (current_encoding == key_position ? "OK" : "FAILED");
    }
    return result;
}

int main(int argc, const char * argv[]) {
    ChessBoard chess_board {};
    init_chess_board(chess_board);
    test_board(chess_board);
    
    
    // Flip a coin on the board
    flip_coin(chess_board, 63); // Should encode 111111
    // print out the chess board
    print_chess_board(chess_board);
    // Chess board encoding storage
    Encoding encoding {};
    encoding = decode_board(chess_board);
    print_encoding(encoding);
    
    std::cout << "\n\n";
    return 0;
}

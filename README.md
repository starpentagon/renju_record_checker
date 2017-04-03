# Renju record checker

The program checks and modify the renju game records
* normality of the moves
   * put moves in open position or pass
   * avoid the forbidden move if the black player cannot make the five
   * guard the opponent four
   * resign if the opponent makes open-four, double-four or forbiddened four guard move
* terminate the game record if following move is appeared:
   * open four
   * double four
   * forbiddened four guard

# Supported format
The csv renju game record file with the following columns:
* game_date
* event
* black_player
* black_rank
* white_player
* white_rank
* opening_rule
* game_end_status
* game_result
* game_record
* alternative_moves

Note that the Renju SGF parser(https://github.com/starpentagon/renju_sgf_parser) 
outputs the renju game record in this format.

## Requirement
* CMake 3.5.1 or newer
* boost 1.63 or newer
* C++ compiler
   * clang 3.8 or newer
* ccache 3.3.4 or newer(optional)

## How to build

Move to the root directory of the Renju record checker, and run

`./build.sh`

You can find the program in build/renju_record_checker

## Usage

1. Check the record game record
Run:

`./renju_record_checker --db (Game record file)`

Output:
   * Modified game records in standard output
   * Check result log in standard error

2. Show help
Run:

`./renju_record_checker --help`

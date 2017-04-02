#include <memory>
#include <iostream>

#include <boost/program_options.hpp>

#include "CSVReader.h"
#include "MoveList.h"
#include "Board.h"

using namespace std;
using namespace boost::program_options;
using namespace realcore;

int main(int argc, char* argv[])
{
  // オプション設定
  options_description option;

  option.add_options()
    ("db", value<string>(), "棋譜データベース(csv)")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("help") || !arg_map.count("db")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;
    cout << "The checker outputs:" << endl;
    cout << "\tModified game records in standard output" << endl;
    cout << "\tChecking long in standard error" << endl;

    return 0;
  }

  // 棋譜データベースの読込
  const string game_record_db_file = arg_map["db"].as<string>();

  map<string, StringVector> game_record_db;
  ReadCSV(game_record_db_file, &game_record_db);

  const auto game_date_list = game_record_db["game_date"];
  const auto event_list = game_record_db["event"];
  const auto black_player_list = game_record_db["black_player"];
  const auto black_rank_list = game_record_db["black_rank"];
  const auto white_player_list = game_record_db["white_player"];
  const auto white_rank_list = game_record_db["white_rank"];
  const auto opening_rule_list = game_record_db["opening_rule"];
  const auto game_end_status_list = game_record_db["game_end_status"];
  const auto game_result_list = game_record_db["game_result"];
  const auto game_record_list = game_record_db["game_record"];
  const auto alternative_moves_list = game_record_db["alternative_moves"];

  cout << "game_date,event,black_player,black_rank,white_player,white_rank,opening_rule,game_end_status,game_result,game_record,alternative_moves" << endl;
  cerr << "reason,original_record,modified_record,move,same/diff_result" << endl;

  // 正規性のチェック
  for(size_t i=0, size=game_record_list.size(); i<size; i++){
    auto game_record_str = game_record_list[i];
    auto game_result = game_result_list[i];
    auto game_end_status = game_end_status_list[i];

    MoveList board_move, game_record(game_record_str);
    Board board;

    bool is_modified = false;

    for(const auto move : game_record){
      const bool is_black = board_move.IsBlackTurn();
      const bool is_normal_move = is_black ? board.IsNormalMove<kBlackTurn>(move) : board.IsNormalMove<kWhiteTurn>(move);
      
      if(!is_normal_move){
        // 正規手ではない(空点でない or 相手の四ノビを防いでいない)
        is_modified = true;
        const bool is_same_result = is_black ? (game_result == "WhiteWin") : (game_result == "BlackWin");
        game_end_status = is_same_result ? game_end_status : "Modified";
        game_result = is_black ? "WhiteWin" : "BlackWin";

        cerr << "Not normal move," << game_record_str << "," << board_move.str() << "," << MoveList(move).str() << ",";
        cerr << (is_same_result ? "same_result" : "diff_result") << endl;
        break;
      }

      const bool is_terminate_move = is_black ? board.IsTerminateMove<kBlackTurn>(move) : board.IsTerminateMove<kWhiteTurn>(move);
      const bool last_move = game_record.GetLastMove() == move;

      if(is_terminate_move && !last_move){
        // 最終手以外で終端手
        is_modified = true;
        const bool is_same_result = is_black ? (game_result == "BlackWin") : (game_result == "WhiteWin");
        game_end_status = is_same_result ? game_end_status : "Modified";
        game_result = is_black ? "BlackWin" : "WhiteWin";
        board_move += move;

        cerr << "Terminating move," << game_record_str << "," << board_move.str() << "," << MoveList(move).str() << ",";
        cerr << (is_same_result ? "same_result" : "diff_result") << endl;
        break;
      }

      board.MakeMove(move);
      board_move += move;
    }

    cout << game_date_list[i] << ",";
    cout << event_list[i] << ",";
    cout << black_player_list[i] << ",";
    cout << black_rank_list[i] << ",";
    cout << white_player_list[i] << ",";
    cout << white_rank_list[i] << ",";
    cout << opening_rule_list[i] << ",";
    cout << game_end_status_list[i] << ",";
    cout << game_result << ",";
    cout << board_move.str() << ",";
    cout << alternative_moves_list[i] << endl;
  }
  
  return 0;
}

# Renju record checker

The program checks the renju game records in [a-o] style format
* normality of the moves
   * put moves in open position or pass
   * avoid the forbidden move if the black player cannot make the five
   * guard the opponent four
   * resign if the opponent makes open-four, double-four or forbiddened four guard move
* terminate the game record if following move is appeared:
   * open four
   * double four
   * forbiddened four guard

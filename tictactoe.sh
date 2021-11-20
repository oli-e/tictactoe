#!/usr/bin/bash

# Do poprawienia
# gdy wygrywasz w ostatnim ruchu to on uznaje to za DRAW
# gdy wybierasz już wybrany kub to zwraca not valid

''' 
    Added colors:
        \e[34m - Blue
        \e[35m - Magenta
        
        '''

player_a="X" 
player_b="O"

turn=1
game_on=true

moves=( 1 2 3 4 5 6 7 8 9 )

welcome_message() {
  clear
  echo "******************************"
  echo "*** WELCOME TO TIC TAC TOE ***"
  echo "******************************"
  sleep 1
}
print_board () {
#   clear
  echo "  *** BOARD ***"
  echo -e "    ${moves[0]} | ${moves[1]} | ${moves[2]} "
  echo "   -----------"
  echo -e "    ${moves[3]} | ${moves[4]} | ${moves[5]} "
  echo "   -----------"
  echo -e "    ${moves[6]} | ${moves[7]} | ${moves[8]} "
  echo "============="
}

player_pick(){
  if [[ $(($turn % 2)) == 0 ]]
  then
    play=$player_b
    echo -n "PLAYER B PICK A SQUARE: "
  else
    echo -n "PLAYER A PICK A SQUARE: "
    play=$player_a
  fi

  read square

  space=${moves[($square -1)]} 

  if [[ ! $square =~ ^-?[0-9]+$ ]] || [[ ! $space =~ ^[0-9]+$  ]]
  then 
    echo "Not a valid square."
    player_pick
  else
    moves[($square -1)]=$play
    ((turn=turn+1))
  fi
  space=${moves[($square-1)]} 
}

check_match() {
  echo " ${moves[$1]}"
  if  [[ ${moves[$1]} == ${moves[$2]} ]]&& \
      [[ ${moves[$2]} == ${moves[$3]} ]]; then
    game_on=false
  fi
  if [ $game_on == false ]; then
    # echo " ${moves[$1]}"
    if [ ${moves[$1]} == "x"  ];then
      echo "Player A wins!"
      return 
    else
      echo "player B wins!"
      return 
    fi
  fi
}

check_winner(){
  if [ $game_on == false ]; then return; fi
  check_match 0 1 2
  if [ $game_on == false ]; then return; fi
  check_match 3 4 5
  if [ $game_on == false ]; then return; fi
  check_match 6 7 8
  if [ $game_on == false ]; then return; fi
  check_match 0 4 8
  if [ $game_on == false ]; then return; fi
  check_match 2 4 6
  if [ $game_on == false ]; then return; fi
  check_match 0 3 6
  if [ $game_on == false ]; then return; fi
  check_match 1 4 7
  if [ $game_on == false ]; then return; fi
  check_match 2 5 8
  if [ $game_on == false ]; then return; fi

  if [ $turn -gt 9 ]; 
  then 
    game_on=false
    echo "Its a draw!"
  fi
}

welcome_message
print_board
while $game_on
do
  player_pick
  print_board
  check_winner
done
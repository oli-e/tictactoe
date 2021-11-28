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
  clear
  echo "  *** BOARD ***"
  echo -e "    ${moves[0]} | ${moves[1]} | ${moves[2]} "
  echo "   -----------"
  echo -e "    ${moves[3]} | ${moves[4]} | ${moves[5]} "
  echo "   -----------"
  echo -e "    ${moves[6]} | ${moves[7]} | ${moves[8]} "
  echo "============="
  echo " "
  echo "Press Ctrl + C to exit "

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
  # echo " ${moves[$1]}"
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
read_array(){
  mapfile -t moves < ./.plik.txt
  export ARRAY="${moves[$@]}"
}

run_game(){
while $game_on
do
  trap term SIGINT
  player_pick
  print_board
  check_winner
done
}

term() {
  printf "\n  Do you want to quit save your game? (yes/no) "
  read CHOICE
  if [ $CHOICE == "yes" ];then
  # export OLD_GAME="${moves[$@]}"
  # echo "${OLD_GAME}"
  printf " %s " "${moves[@]}" > ./.plik.txt
  exit 1
  else exit 1 
  # run_game
  fi
}

  # perform cleanup - don't exit immediately



welcome_message
if [[ -f "./.plik.txt" ]];
then
    printf "You have unfinished game do you want to finish it? \n yes - for old game\n no - for new one\n:"
    read GAME
    if [ ${GAME} == "yes" ];then
    read_array
    moves=( ${ARRAY} )
    # echo ${moves[$@]}
    fi
fi
print_board
while $game_on
do
  trap term SIGINT
  player_pick
  print_board
  check_winner
done

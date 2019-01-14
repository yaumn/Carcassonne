#include <assert.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "road.h"
#include "board.h"
#include "clients_loading.h"
#include "move.h"
#include "player.h"

#ifdef ENABLE_GUI
#include "gui.h"
#endif


static void *copy_player(void *e)
{
  struct player *p = e;
  struct player *new = malloc(sizeof(struct player));
  assert( new != NULL );
  *new = *p;
  return new;
}


static void free_lib(void *e)
{
  dlclose(e);
}


int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-g] [FILE(S)]\n", argv[0]);
    return 1;
  }

  /* Load the clients */

  const int seed = time(NULL);
  srand(seed);
  printf("Seed : %d\n", seed);

  struct list players;
  list__init(&players, copy_player, free);
  struct list libs;
  list__init(&libs, NULL, free_lib);

  #ifdef ENABLE_GUI
  int use_gui = 0;

  if (strcmp(argv[1], "-g") == 0) {
    use_gui = 1;
    argv++;
    argc--;
  }
  #endif

  load_clients(argc - 1, argv + 1, &players, &libs);

  if (list__size(&players) == 0) {
    printf(BOLD(RED("ERROR: No client could be loaded.\n")));
    exit(1);
  } else if (list__size(&players) == 1) {
    printf(BOLD(RED("ERROR: Only one client could be loaded.\n")));
    exit(1);
  }


  /* Initialize the clients */

  for (size_t i = 0 ; i < list__size(&players) ; i++) {
    ((struct player *)players.d[i])->initialize_player(i, list__size(&players));
  }

  /* Initialize board */

  struct board board;
  board__init(&board);

  /* Initialize GUI */

  #ifdef ENABLE_GUI
  struct gui gui;

  if (use_gui) {
    use_gui = gui__init(&gui);
    if (!use_gui) {
      printf("Couldn't load GUI : %s\n", gui__error_message());
    } else {
      gui__load_tile(&gui, &board.moves[0]);
    }
  }
  #endif

  /* Main loop */

  while (!board__game_is_finished(&board)) {
    for (size_t i = 0 ; i < list__size(&players) && !board__game_is_finished(&board); i++) {
      struct player *p = players.d[i];
      enum card_id c;

      struct list possible_moves;
      list__init(&possible_moves, move_copy, free);

      // Cards are drawn until it is possible to make a valid move with the card
      while (list__size(&possible_moves) == 0 && !board__game_is_finished(&board)) {
        c = board__draw_card(&board);
        printf("%s draws card %s (%d)", p->get_name(), tile__card_to_str(c), c);
        move__get_all_possible_moves(&board, &possible_moves, c, i);
        if (list__size(&possible_moves) == 0) {
          printf(" but the card is not playable !");
        }
        printf("\n");
      }
      list__free(&possible_moves);

      if (board__game_is_finished(&board) && list__size(&possible_moves) == 0) {
        break;
      }


      struct move move = p->play_move(c, board.moves, board.moves_nb);
      move.onto.x -= 72;
      move.onto.y -= 72;


      if (move__is_valid(&board, &move)) {
        move.check = VALID;
        board__add_move(&board, move);
        // Be sure to add the tile with the move which is in board.moves
        // and not a move created in this functions ! I spent a entire day
        // correcting a bug linked to this
        board__add_tile(&board, &board.moves[board.moves_nb - 1]);
        printf("%s has %d meeples left \n", p->get_name(), board.followers_left[i]);
        printf("%s places his card in (%d, %d) with orientation %s ",
	       p->get_name(), move.onto.x, move.onto.y, tile__direction_to_str(move.dir));
        if(move.place != NO_MEEPLE){
          printf("with a meeple in %s position\n", tile__meeple_place_to_str(move.place));
        } else {
          printf("without any meeple\n");
        }
        #ifdef ENABLE_GUI
        if (use_gui) {
          gui__load_tile(&gui, &move);
        }
        #endif
      } else {
        move.check = FAILED;
        board__add_move(&board, move);
        printf("%s made an invalid move\n", p->get_name());
      }
    }
    printf("\n");
  }

  printf("The game is finished ! Here are the Points :\n\n");

  board__update_points(&board);

  int max_name_length = 0;
  for (size_t i = 0 ; i < list__size(&players) ; i++) {
    struct player *p = list__get(&players, i);
    const int length = strlen(p->get_name());
    if (length > max_name_length) {
      max_name_length = length;
    }
  }

  printf("\t┌─");

  for (int i = 0 ; i < max_name_length ; i++) {
    printf("─");
  }

  printf("─┬─────┐\n");

  for (unsigned int i = 0 ; i < list__size(&players) ; i++) {
    struct player *p = list__get(&players, i);
    const int length = strlen(p->get_name());
    int nb_spacs_needed = 1 + max_name_length - length;

    printf("\t│ %s", p->get_name());
    for (int j = 0 ; j < nb_spacs_needed ; j++) {
      printf(" ");
    }

    if (board.points[i] < 10) {
      printf("│   %d │\n", board.points[i]);
    } else if (board.points[i] < 100) {
      printf("│  %d │\n", board.points[i]);
    } else {
      printf("│ %d │\n", board.points[i]);
    }
  }
  printf("\t└─");

  for (int i = 0 ; i < max_name_length ; i++) {
    printf("─");
  }
  printf("─┴─────┘\n\n");

  printf("gagnant : %s\n", board.points[0] > board.points[1] ? ((struct player *)players.d[0])->get_name()
: ((struct player *)players.d[1])->get_name());


  /* Finalize */

  for (size_t i = 0 ; i < list__size(&players) ; i++) {
    ((struct player *)players.d[i])->finalize_player();
  }

  free_clients(&players, &libs);

  #ifdef ENABLE_GUI
  if (use_gui) {
    gui__wait_for_quit_event();
    gui__free();
  }
  #endif

  board__free(&board);

  printf("Seed : %d\n", seed);

  return 0;
}

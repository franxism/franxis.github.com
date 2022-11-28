#ifndef GP32_MENU_H
#define GP32_MENU_H

void game_list_init(void);
void game_list_view(int *pos);
void game_list_select (int index, char *game);
void select_game(char *argv[]);
void intro_screen(void);
void Delay( int millisec );

#endif

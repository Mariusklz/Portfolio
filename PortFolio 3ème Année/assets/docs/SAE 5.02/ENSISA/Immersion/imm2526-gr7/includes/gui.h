#ifndef GUI_H
#define GUI_H

#include "../includes/header_commun.h"

void gui_activate(GtkApplication *app, gpointer user_data);

gboolean is_local_player_turn();

#endif

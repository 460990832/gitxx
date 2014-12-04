/*
Birl is Ruijie Linux

作者：木瓜无衣 （1401570404@qq.com)

*/



#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <gtk/gtk.h>
#include "common.h"

GtkWidget *main_window;
GtkWidget *main_frame;
GtkWidget *login_button;
GtkWidget *exit_button;
GtkWidget *username_label;
GtkWidget *password_label;
GtkWidget *username_comobox;
GtkWidget *password_entry;
GtkWidget *main_window_backimage;
GtkWidget *save_checkbutton;
GList *items_username_comobox;
GtkWidget *mesg_window;
gchar guiUsername[64];
gchar guiPassword[64];

GtkWidget *connect_window;
GtkWidget *connect_frame;
GtkWidget *connect_label;
GtkWidget *connect_exit_button;

void main_window_init();
void connect_window_init();

#endif // CONFIG_H_INCLUDED

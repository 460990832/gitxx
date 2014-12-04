/*

Birl is Ruijie Linux

作者：木瓜无衣：（1401570404@qq.com)
*/



#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "gtk/gtk.h"
#include "trayicon.h"

extern GtkWidget *connect_window;
extern GtkWidget *main_window;

GdkPixbuf *create_pixbuf(const gchar *filename);
gboolean connect_login();
void connect_logoff();
void connect_exit();
void showMesg(char *mesg);

#endif // COMMON_H_INCLUDED

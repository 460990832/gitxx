/*
Birl is Ruijie Linux

作者：JessonChan（chinachendongpo@gmail.com//OR//i@moreblue.cn）木瓜无衣 （1401570404@qq.com)

*/

#include "trayicon.h"

void trayIconActivated()
{
    gtk_widget_show(GTK_WIDGET(main_window));
    gtk_window_deiconify(GTK_WINDOW(main_window));
}

void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 active_time, gpointer popUpMenu)
{
    gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, active_time);
}
void trayicon_init()
{
    trayicon = gtk_status_icon_new_from_file("/opt/birl/birl.ico");
    trayicon_menu = gtk_menu_new();
    trayicon_menuitem_logoff = gtk_menu_item_new_with_label("断开链接");
    trayicon_menuitem_exit = gtk_menu_item_new_with_label("退出");

    g_signal_connect(G_OBJECT(trayicon_menuitem_exit), "activate", G_CALLBACK(connect_exit), NULL);
    g_signal_connect(G_OBJECT(trayicon_menuitem_logoff), "activate", G_CALLBACK(connect_logoff), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(trayicon_menu), trayicon_menuitem_exit);
    gtk_menu_shell_append(GTK_MENU_SHELL(trayicon_menu), trayicon_menuitem_logoff);
    gtk_widget_show_all(trayicon_menu);
    gtk_status_icon_set_tooltip (trayicon, "Birl");
    //gtk_tooltip_set_icon_from_icon_name(trayicon, "icon.png", 100);
    gtk_status_icon_set_visible(trayicon, FALSE);
    g_signal_connect(GTK_STATUS_ICON (trayicon), "popup-menu", GTK_SIGNAL_FUNC (trayIconPopup), trayicon_menu);
}

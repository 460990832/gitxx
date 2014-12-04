/*
Birl is Ruijie Linux

作者：瓜无衣 （1401570404@qq.com)

*/



#include "common.h"
#include "eggtrayicon.h"
#include <pthread.h>
#include "mentohust.h"
#include "config.h"

int argc;
char **argv;

GdkPixbuf *create_pixbuf(const gchar *filename)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if(!pixbuf)
    {
        g_error_free(error);
    }

    return pixbuf;
}

gboolean connect_login()
{
    gtk_widget_hide_all(main_window);
    gtk_widget_show_all(connect_window);

    gchar* tU = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(username_comobox)->entry));
    gchar* tP = gtk_entry_get_text(password_entry);
    initConfig(tU, tP);
    startConnet();
    if(statasConnet() == 1)
    {
        //gtk_widget_hide_all(main_window);
        gtk_status_icon_set_visible(trayicon, TRUE);
        g_thread_create(mentohust, NULL, FALSE, NULL);
    }
    else
    {
        gchar* mesg = getServerMsg();
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,             //跟随父窗口关闭
                                        GTK_MESSAGE_INFO,                        //显示图标
                                        GTK_BUTTONS_NONE,                             //不显示按钮
                                        mesg);
        gtk_window_set_title(GTK_WINDOW(dialog), ("birl提示："));//对话框的标题
        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));//运行对话框
        gtk_widget_destroy(dialog);//删除对话
        gtk_widget_show_all(main_window);
        return FALSE;
    }

    gtk_widget_hide_all(connect_window);
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(save_checkbutton)))
    {
            saveConfig(tU, tP);
    }
    else
    {}

    return TRUE;
}

void connect_logoff()
{
    //gdk_threads_leave();
    //disConnet();
    gtk_widget_show_all(main_window);
}

void connect_exit()
{
    gdk_threads_leave();
    gtk_main_quit();
}

void showMesg(char *mesg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,             //跟随父窗口关闭
                                    GTK_MESSAGE_INFO,                        //显示图标
                                    GTK_BUTTONS_NONE,                             //不显示按钮
                                    mesg);
    gtk_window_set_title(GTK_WINDOW(dialog), ("birl提示："));//对话框的标题
    gtk_dialog_run(GTK_DIALOG(dialog));//运行对话框
    gtk_widget_destroy(dialog);//删除对话框
}
void create_tray (GtkWidget * tray)
{
    GtkWidget *image;
    GtkWidget *event_box;
    GtkTooltips *tooltips;
    EggTrayIcon *tray_icon;

    tooltips = gtk_tooltips_new ();
    tray_icon = egg_tray_icon_new ("Tray icon demo");
    event_box = gtk_event_box_new ();
    gtk_container_add (GTK_CONTAINER (tray_icon), event_box);
    g_signal_connect (G_OBJECT (event_box), "button-press-event",
                      G_CALLBACK (showMesg), tray);
    gtk_tooltips_set_tip (GTK_TOOLTIPS(tooltips), event_box, ("Try a left/right click"),NULL);

    //image = load_image("/home/yuechu/icons/Alert1.ico");
    gtk_container_add (GTK_CONTAINER (event_box), image);

    gtk_widget_show_all (GTK_WIDGET (tray_icon));

    return;
}

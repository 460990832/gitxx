/*
Birl is Ruijie Linux

作者：木瓜无衣 （1401570404@qq.com)

*/



#include "config.h"

#define USERNAME_SIZE 65
#define PASSWORD_SIZE 64
#define ICO_PATH "/opt/birl/birl.ico"
#define BACKIMAGE_PATH "/opt/birl/backimage.jpeg"

void main_window_init()
{
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "birl");
    gtk_widget_set_size_request(GTK_WINDOW(main_window), 400, 280);
    gtk_window_set_policy(GTK_WINDOW(main_window), FALSE, FALSE, TRUE);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    gtk_window_set_icon(GTK_WINDOW(main_window), create_pixbuf(ICO_PATH));

    main_frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(main_window), main_frame);

    main_window_backimage = gtk_image_new_from_file(BACKIMAGE_PATH);
    gtk_fixed_put(GTK_WINDOW(main_frame), main_window_backimage, 0, 0);

    login_button = gtk_button_new_with_label("登 录");
    gtk_widget_set_size_request(login_button, 70, 30);
    gtk_fixed_put(GTK_FIXED(main_frame), login_button, 80, 220);

    exit_button = gtk_button_new_with_label("退 出");
    gtk_widget_set_size_request(exit_button, 70, 30);
    gtk_fixed_put(GTK_FIXED(main_frame), exit_button, 250, 220);

    username_label = gtk_label_new("用户名");
    gtk_fixed_put(GTK_FIXED(main_frame), username_label, 100, 115);

    password_label = gtk_label_new("密   码");
    gtk_fixed_put(GTK_FIXED(main_frame), password_label, 100, 165);


    int length = 0;
    getUserInfo(guiUsername, guiPassword);
    g_printf("%s", guiUsername);

    username_comobox = gtk_combo_new();
    gtk_widget_set_size_request(username_comobox, 130, 30);
    gtk_fixed_put(GTK_FIXED(main_frame), username_comobox, 170, 110);

    GList *usernamelist = NULL;
	usernamelist = g_list_append(usernamelist, guiUsername);
	gtk_combo_set_popdown_strings(GTK_COMBO(username_comobox), usernamelist);

    password_entry = gtk_entry_new();
    gtk_widget_set_size_request(password_entry, 100, 30);
    gtk_fixed_put(GTK_FIXED(main_frame), password_entry, 170, 160);

    save_checkbutton = gtk_check_button_new_with_label("保存密码");
    gtk_fixed_put(GTK_FIXED(main_frame), save_checkbutton, 275, 165);
    //gtk_toggle_button_set_active(GTK_TOGGLT_BUTTON(save_checkbutton), TRUE);
     gtk_toggle_button_set_active(save_checkbutton,TRUE);
    GTK_WIDGET_UNSET_FLAGS(save_checkbutton, GTK_CAN_FOCUS);
    gtk_entry_set_visibility(password_entry, FALSE);
    gtk_entry_set_text(password_entry, guiPassword);

    mesg_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mesg_window), "birl notify:");
    gtk_widget_set_size_request(GTK_WINDOW(mesg_window), 200, 100);
    gtk_window_set_position(GTK_WINDOW(mesg_window), GTK_WIN_POS_CENTER);

    gtk_widget_show_all(main_window);
    g_signal_connect(G_OBJECT(main_window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(login_button, "clicked", G_CALLBACK(connect_login), NULL);
}

void connect_window_init()
{
    connect_window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_set_size_request(GTK_WINDOW(connect_window), 300, 160);
    gtk_window_set_policy(GTK_WINDOW(connect_window), FALSE, FALSE, TRUE);
    gtk_window_set_position(GTK_WINDOW(connect_window), GTK_WIN_POS_CENTER);
    connect_frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(connect_window), connect_frame);
    connect_label = gtk_label_new("connecting...");
    gtk_fixed_put(GTK_FIXED(connect_frame), connect_label, 110, 60);

    connect_exit_button = gtk_button_new_with_label("断开链接");
    gtk_widget_set_size_request(connect_exit_button, 70, 30);
    gtk_fixed_put(GTK_FIXED(connect_frame), connect_exit_button, 120, 100);
    g_signal_connect(connect_exit_button, "clicked", G_CALLBACK(connect_logoff), NULL);
}

/*
Birl is Ruijie Linux

作者：JessonChan（chinachendongpo@gmail.com//OR//i@moreblue.cn）木瓜无衣 （1401570404@qq.com)

*/



#include "common.h"
#include "config.h"
#include "trayicon.h"
#include "gtk/gtk.h"

int main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    connect_window_init();
    trayicon_init();
    main_window_init();

    gtk_main();




    return 0;
}

#include "global_variables.h"

int main (int argc, char *argv[])
{
    //mysql_init
    printf("The MySQL client version is : %s\n", mysql_get_client_info());
    connection = mysql_init(NULL);
    if(mysql_real_connect(connection, "localhost", "root", "", "guestbook", 0, NULL, 0) == NULL)//0
    {
        printf("%s\n", mysql_error(connection));
        return 1;
    }

    //gtk_init
    GtkApplicationWindow *window;
    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "GuestBook.glade", NULL) == 0)
    {
        return 1;
    }
    gtk_builder_connect_signals(builder, NULL);

    //get objects defined in .glade file
    window = gtk_builder_get_object(builder, "ApplicationWindow");
    entry_main = gtk_builder_get_object(builder, "Entry_main");
    buffer_main = gtk_builder_get_object(builder, "Buffer_main");
    search_main = gtk_builder_get_object(builder, "Search_main");
    buffer_words = gtk_builder_get_object(builder, "Buffer_words");
    buffer_pivot = gtk_builder_get_object(builder, "Buffer_pivot");


    gtk_widget_show(window);
    gtk_main();


    mysql_close(connection);
    return 0;
}

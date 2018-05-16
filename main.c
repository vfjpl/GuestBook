#include <gtk/gtk.h>
#include <mysql/mysql.h>

GtkApplicationWindow *window;
GtkBuilder *builder;
GtkTextView *text_field;
GtkEntry *entry_field;

GtkTextBuffer *text_buffer;

MYSQL *connection;
MYSQL_RES *query_result;

int main (int argc, char *argv[])
{
    //mysql_init
    printf("The MySQL client version is : %s\n", mysql_get_client_info());
    connection = mysql_init(NULL);
    if(mysql_real_connect(connection, "localhost", "root", "", "guestbook", NULL, NULL, NULL) == NULL)
    {
        printf("%s\n", mysql_error(connection));
        return EXIT_FAILURE;
    }

    //gtk_init
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "GuestBook.glade", NULL) == 0)
    {
        return EXIT_FAILURE;
    }
    gtk_builder_connect_signals(builder, NULL);

    //get objects defined in .glade file
    window = gtk_builder_get_object(builder, "ApplicationWindow");
    text_field = gtk_builder_get_object(builder, "TextView_main");
    entry_field = gtk_builder_get_object(builder, "Entry_main");
    text_buffer = gtk_text_view_get_buffer(text_field);


    gtk_widget_show(window);
    gtk_main();


    mysql_free_result(query_result);
    mysql_close(connection);
    return EXIT_SUCCESS;
}

void on_button_send()
{
    gtk_text_buffer_set_text(text_buffer, gtk_entry_get_text(entry_field), -1);
    gtk_entry_set_text(entry_field, "");
    gtk_text_view_set_buffer(text_field, text_buffer);
}

void on_button_refresh()
{
    mysql_query(connection, "SELECT comment FROM comments ORDER BY id DESC");
    query_result = mysql_use_result(connection);//mysql_store_result

    MYSQL_ROW row;
    if( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_set_text(text_buffer, row[0], -1);
    }
    while( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_insert_at_cursor(text_buffer, "\n\n", -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, row[0], -1);
    }
}

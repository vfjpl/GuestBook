#include <gtk/gtk.h>
#include <mysql/mysql.h>

GtkTextBuffer *text_buffer;
GtkEntry *entry_field;
MYSQL *connection;

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
    GtkApplicationWindow *window;
    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "GuestBook.glade", NULL) == 0)
    {
        return EXIT_FAILURE;
    }
    gtk_builder_connect_signals(builder, NULL);

    //get objects defined in .glade file
    window = gtk_builder_get_object(builder, "ApplicationWindow");
    entry_field = gtk_builder_get_object(builder, "Entry_main");
    text_buffer = gtk_builder_get_object(builder, "text_buffer");


    gtk_widget_show(window);
    gtk_main();


    mysql_close(connection);
    return EXIT_SUCCESS;
}

void on_button_send()
{
    GtkEntryBuffer *temp = gtk_entry_buffer_new(NULL, NULL);
    gtk_entry_buffer_set_text(temp, "INSERT INTO comments VALUE(NULL,'", -1);//VALUES
    gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), gtk_entry_get_text(entry_field), -1);
    gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), "',NULL)", -1);

    mysql_query(connection, gtk_entry_buffer_get_text(temp));
}

void on_button_refresh()
{
    mysql_query(connection, "SELECT * FROM comments ORDER BY id DESC");
    MYSQL_RES *query_result = mysql_use_result(connection);//mysql_store_result
    MYSQL_ROW row;
    if( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_set_text(text_buffer, row[1], -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, "\n", -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, row[2], -1);
    }
    while( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_insert_at_cursor(text_buffer, "\n\n", -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, row[1], -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, "\n", -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, row[2], -1);
    }
}

#include <gtk/gtk.h>
#include <mysql/mysql.h>

GtkTextBuffer *text_buffer;
GtkEntry *entry_field;
GtkSearchEntry *search_entry;
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
    if(gtk_builder_add_from_file(builder, "GuestBook.glade", NULL) == NULL)
    {
        return EXIT_FAILURE;
    }
    gtk_builder_connect_signals(builder, NULL);

    //get objects defined in .glade file
    window = gtk_builder_get_object(builder, "ApplicationWindow");
    entry_field = gtk_builder_get_object(builder, "Entry_main");
    text_buffer = gtk_builder_get_object(builder, "text_buffer");
    search_entry = gtk_builder_get_object(builder, "Search_main");


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
    if( (mysql_query(connection, gtk_entry_buffer_get_text(temp))) )
    {
        printf("%s\n", mysql_error(connection));
        return;
    }
    gtk_entry_set_text(entry_field, "");
    on_button_refresh();//refresh comments
}

void on_button_refresh()
{
    search_query("SELECT * FROM comments ORDER BY id DESC");
}

void on_search_entry()
{
    GtkEntryBuffer *temp = gtk_entry_buffer_new(NULL, NULL);
    gtk_entry_buffer_set_text(temp, "SELECT * FROM comments WHERE comment LIKE '%", -1);//everything
    gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), gtk_entry_get_text(search_entry), -1);
    gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), "%' ORDER BY id DESC", -1);

    search_query(gtk_entry_buffer_get_text(temp));
}

void search_query(char *query_statement)
{
    if( (mysql_query(connection, query_statement)) )
    {
        printf("%s\n", mysql_error(connection));
        return;
    }
    MYSQL_RES *query_result = mysql_use_result(connection);//mysql_store_result
    MYSQL_ROW row;

    gtk_text_buffer_set_text(text_buffer, "", NULL);
    if( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_insert_at_cursor(text_buffer, row[1], -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, "\n", -1);
        gtk_text_buffer_insert_at_cursor(text_buffer, row[2], -1);
        while( (row = mysql_fetch_row(query_result)) )
        {
            gtk_text_buffer_insert_at_cursor(text_buffer, "\n\n", -1);
            gtk_text_buffer_insert_at_cursor(text_buffer, row[1], -1);
            gtk_text_buffer_insert_at_cursor(text_buffer, "\n", -1);
            gtk_text_buffer_insert_at_cursor(text_buffer, row[2], -1);
        }
    }
}

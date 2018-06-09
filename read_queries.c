#include "global_variables.h"

void show_query_main(const char *query_statement)
{
    if( mysql_query(connection, query_statement) )
    {
        printf("%s\n", mysql_error(connection));
        return;
    }
    MYSQL_RES *query_result = mysql_use_result(connection);//mysql_store_result
    MYSQL_ROW row;

    if( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_set_text(buffer_main, row[1], -1);
        gtk_text_buffer_insert_at_cursor(buffer_main, "\n", -1);
        gtk_text_buffer_insert_at_cursor(buffer_main, row[2], -1);
        while( (row = mysql_fetch_row(query_result)) )
        {
            gtk_text_buffer_insert_at_cursor(buffer_main, "\n\n", -1);
            gtk_text_buffer_insert_at_cursor(buffer_main, row[1], -1);
            gtk_text_buffer_insert_at_cursor(buffer_main, "\n", -1);
            gtk_text_buffer_insert_at_cursor(buffer_main, row[2], -1);
        }
    }
    else
    {
        gtk_text_buffer_set_text(buffer_main, "", 0);
    }
}

void on_search_main()
{
    GtkEntryBuffer *temp = gtk_entry_buffer_new(NULL, 0);
    gtk_entry_buffer_set_text(temp, "SELECT * FROM comments WHERE comment LIKE '%", -1);//search everywhere
    gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), gtk_entry_get_text(search_main), -1);
    gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), "%' ORDER BY id DESC", -1);

    show_query_main(gtk_entry_buffer_get_text(temp));
}

void on_refresh_main()
{
    show_query_main("SELECT * FROM comments ORDER BY id DESC");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void on_refresh_words()
{
    if( mysql_query(connection, ("SELECT word, COUNT(*) AS count "
    "FROM (SELECT SUBSTRING_INDEX(SUBSTRING_INDEX(comment, ' ', numbers.n), ' ', -1) AS word "
    "FROM (SELECT id AS n FROM comments) AS numbers "
    "INNER JOIN comments ON CHAR_LENGTH(comment) - CHAR_LENGTH(REPLACE(comment, ' ', '')) >= numbers.n-1) AS unused "
    "GROUP BY word ORDER BY count DESC")) )
    {
        printf("%s\n", mysql_error(connection));
        return;
    }
    MYSQL_RES *query_result = mysql_use_result(connection);//mysql_store_result
    MYSQL_ROW row;

    if( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_set_text(buffer_words, row[1], -1);
        gtk_text_buffer_insert_at_cursor(buffer_words, " ", -1);
        gtk_text_buffer_insert_at_cursor(buffer_words, row[0], -1);
        while( (row = mysql_fetch_row(query_result)) )
        {
            gtk_text_buffer_insert_at_cursor(buffer_words, "\n", -1);
            gtk_text_buffer_insert_at_cursor(buffer_words, row[1], -1);
            gtk_text_buffer_insert_at_cursor(buffer_words, " ", -1);
            gtk_text_buffer_insert_at_cursor(buffer_words, row[0], -1);
        }
    }
    else
    {
        gtk_text_buffer_set_text(buffer_words, "", 0);
    }
}

void on_refresh_pivot()
{
    if( mysql_query(connection, ("SELECT word, COUNT(*) AS count "
    "FROM (SELECT SUBSTRING_INDEX(SUBSTRING_INDEX(comment, ' ', numbers.n), ' ', -1) AS word "
    "FROM (SELECT id AS n FROM comments) AS numbers "
    "INNER JOIN comments ON CHAR_LENGTH(comment) - CHAR_LENGTH(REPLACE(comment, ' ', '')) >= numbers.n-1) AS unused "
    "GROUP BY word ORDER BY count DESC")) )
    {
        printf("%s\n", mysql_error(connection));
        return;
    }
    MYSQL_RES *query_result = mysql_use_result(connection);//mysql_store_result
    MYSQL_ROW row;

    if( (row = mysql_fetch_row(query_result)) )
    {
        gtk_text_buffer_set_text(buffer_pivot, row[1], -1);
        gtk_text_buffer_insert_at_cursor(buffer_pivot, " ", -1);
        gtk_text_buffer_insert_at_cursor(buffer_pivot, row[0], -1);
        while( (row = mysql_fetch_row(query_result)) )
        {
            gtk_text_buffer_insert_at_cursor(buffer_pivot, "\n", -1);
            gtk_text_buffer_insert_at_cursor(buffer_pivot, row[1], -1);
            gtk_text_buffer_insert_at_cursor(buffer_pivot, " ", -1);
            gtk_text_buffer_insert_at_cursor(buffer_pivot, row[0], -1);
        }
    }
    else
    {
        gtk_text_buffer_set_text(buffer_pivot, "", 0);
    }
}

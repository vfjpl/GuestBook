#include "global_variables.h"

void on_send_main()
{
    if( gtk_entry_get_text_length(entry_main) != 0 )//check on database level?
    {
        GtkEntryBuffer *temp = gtk_entry_buffer_new(NULL, 0);
        gtk_entry_buffer_set_text(temp, "INSERT INTO comments VALUE(NULL,'", -1);//VALUES
        gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), gtk_entry_get_text(entry_main), -1);
        gtk_entry_buffer_insert_text(temp, gtk_entry_buffer_get_length(temp), "',NULL)", -1);

        if( mysql_query(connection, gtk_entry_buffer_get_text(temp)) )
        {
            printf("%s\n", mysql_error(connection));
            return;
        }

        gtk_entry_set_text(entry_main, "");
    }
}

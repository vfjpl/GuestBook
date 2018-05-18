#ifndef GLOBAL_VARIABLES_H_INCLUDED
#define GLOBAL_VARIABLES_H_INCLUDED

#include <gtk/gtk.h>
#include <mysql/mysql.h>

GtkTextBuffer *buffer_main;
GtkTextBuffer *buffer_words;
GtkEntry *entry_main;
GtkSearchEntry *search_main;
MYSQL *connection;

#endif // GLOBAL_VARIABLES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <glib.h>
#include <gtk-3.0/gdk/gdk.h>
#include <math.h>
#include <gtk-3.0/gdk/gdkconfig.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gtk/gtkx.h>
#include <string.h>

void checkMode();

GtkWidget* window;
GtkBuilder* builder;

GtkWidget* exitMenuButton;
GtkWidget* cancelButton;
GtkWidget* sourceEntry;
GtkWidget* destinationEntry;
GtkWidget* sourceSelectFile;
GtkWidget* destinationSelectFile;
GtkWidget* runicToLatinRadio;
GtkWidget* latinToRunicRadio;
GtkWidget* printButton;
GtkWidget* tutorialButton;
GtkWidget* tutorialsDialog;
GtkWidget* infoButton;
GtkWidget* infoDialog;
GtkWidget* autorsLink;

gboolean state= False;
char source[1024];
char destination[1024];
char* getSource(char* source);

int main(int argc, char **argv){
    gtk_init(&argc, &argv);
    builder=gtk_builder_new_from_file("window.glade");
    window=GTK_WIDGET(gtk_builder_get_object(builder,"window"));
    gtk_window_set_resizable(GTK_WINDOW(window), False);
    gtk_window_set_default_size(GTK_WINDOW(window), 333, 200);
    g_signal_connect(GTK_WINDOW(window),"destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);

    exitMenuButton= GTK_WIDGET(gtk_builder_get_object(builder, "Exit"));
    cancelButton= GTK_WIDGET(gtk_builder_get_object(builder, "Cancel"));
    sourceEntry= GTK_WIDGET(gtk_builder_get_object(builder, "sourceEntry"));
    destinationEntry= GTK_WIDGET(gtk_builder_get_object(builder, "destinationEntry"));
    sourceSelectFile= GTK_WIDGET(gtk_builder_get_object(builder, "sourceSelectFile"));
    destinationSelectFile= GTK_WIDGET(gtk_builder_get_object(builder, "destinationSelectFile"));
    latinToRunicRadio= GTK_WIDGET(gtk_builder_get_object(builder,"mode_latinToRunic"));
    runicToLatinRadio= GTK_WIDGET(gtk_builder_get_object(builder,"mode_runicToLatin"));
    printButton= GTK_WIDGET(gtk_builder_get_object(builder,"print"));
    tutorialButton= GTK_WIDGET(gtk_builder_get_object(builder,"Tutorials"));
    tutorialsDialog= GTK_WIDGET(gtk_builder_get_object(builder, "TutorialsDialog"));
    infoButton= GTK_WIDGET(gtk_builder_get_object(builder,"Info"));
    infoDialog= GTK_WIDGET(gtk_builder_get_object(builder, "InfoDialog"));

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}

void on_Exit_activate(){
    gtk_main_quit();
}

void on_Cancel_clicked(){
    gtk_entry_set_text(GTK_ENTRY(sourceEntry),"");
    gtk_entry_set_text(GTK_ENTRY(destinationEntry),"");
}

void on_sourceSelectFile_file_set(GtkFileChooserButton* f){
    gchar c[1024*3];
    sprintf(c,"%s",gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f)));
    gtk_entry_set_text(GTK_ENTRY(sourceEntry),c);
}

void on_destinationSelectFile_file_set(GtkFileChooserButton* f){
    gchar c[1024*3];
    sprintf(c,"%s",gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(f)));
    gtk_entry_set_text(GTK_ENTRY(destinationEntry),c);
}

void on_mode_latinToRunic_toggled(){
    checkMode();
}

void on_mode_runicToLatin_toggled(){
    checkMode();
}

void checkMode(){
    state= gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(runicToLatinRadio));
    sprintf(source,"%s", gtk_entry_get_text(GTK_ENTRY(sourceEntry)));
    sprintf(destination,"%s", gtk_entry_get_text(GTK_ENTRY(destinationEntry)));
}

void on_print_clicked(){
    checkMode();
    void* func= dlopen("./translator.so", RTLD_LAZY);
    void (*ptr)(char*, char*);

    if(state)
        ptr= dlsym(func, "getLatin");
    else
        ptr= dlsym(func, "getRunic");

    ptr(source, destination);

    dlclose(func);
}

void on_Tutorials_activate(){
    gtk_widget_show(tutorialsDialog);
}

void on_Info_activate(){
    gtk_widget_show(infoDialog);
}
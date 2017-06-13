#include "ctap.h"

void init_window(ctap_t *game) {

  // Create a window to TOPLEVEL i.e. has a border and title bar
  game->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // Set properties of the window
  gtk_window_set_title(GTK_WINDOW(game->window), "Extension");
  gtk_window_set_default_size(GTK_WINDOW(game->window), WINDOW_SIZE, WINDOW_SIZE);
  gtk_window_set_position(GTK_WINDOW(game->window), GTK_WIN_POS_CENTER);
  gtk_window_fullscreen(GTK_WINDOW(game->window));
  gtk_window_set_icon_from_file(GTK_WINDOW(game->window), IMG_ICON, NULL);

  set_bg(game);

}

void init_container(ctap_t *game) {

  // Every visible object on the screen
  game->container = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(game->window), game->container);
  exit_button(game);

}

void init_startscreen(ctap_t *game) {

  game->content = gtk_fixed_new();

  start_screen_text(game);

  gtk_widget_show_all(game->window);

  // Press <ENTER> to start the game
  g_signal_connect(game->window, "key-release-event", G_CALLBACK(start_game), game);

  // When the user closes the window, exit the program
  g_signal_connect(game->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

}

void init_buttons(ctap_t *game) {

  game->num_buttons = 5;
  char *button_names[game->num_buttons];
  button_names[0] = "red";
  button_names[1] = "blue";
  button_names[2] = "green";
  button_names[3] = "yellow";
  button_names[4] = "purple";

  game->buttons = malloc(sizeof(ctap_button_t *) * game->num_buttons);

  for (int i=0; i<game->num_buttons; i++) {

    char *filename = malloc(sizeof(char)*100);
    char *filename_selected = malloc(sizeof(char)*100);
    sprintf(filename, "img/%s.png", button_names[i]);
    sprintf(filename_selected, "img/%s_selected.png", button_names[i]);
    GdkPixbuf* image = create_pixbuf(filename);
    GdkPixbuf* image_selected = create_pixbuf(filename_selected);
    image = gdk_pixbuf_scale_simple(image, BUTTONS_SIZE, BUTTONS_SIZE, GDK_INTERP_BILINEAR);
    image_selected = gdk_pixbuf_scale_simple(image_selected, BUTTONS_SIZE, BUTTONS_SIZE, GDK_INTERP_BILINEAR);

    game->buttons[i].key = button_names[i];
    game->buttons[i].widget = gtk_image_new_from_pixbuf(image);
    game->buttons[i].selected = gtk_image_new_from_pixbuf(image_selected);
    game->buttons[i].active = 0;

  }

  // Init dots as empty
  game->num_dots = 0;
  game->dots = malloc(sizeof(GtkWidget *));

}

void set_bg(ctap_t *game) {

  // Initialise bg pixmap
  GdkPixmap* bg;

  // Load bg image file into pixbuf
  GdkPixbuf* image = create_pixbuf(IMG_BG);

  // Image should fill screen size
  image = gdk_pixbuf_scale_simple(image, gdk_screen_width(), gdk_screen_height(), GDK_INTERP_BILINEAR);
  gdk_pixbuf_render_pixmap_and_mask(image, &bg, NULL, 0);

  // Create a new style
  GtkStyle* style = gtk_style_new();
  style->bg_pixmap[0] = bg;

  // Append the style to the window
  gtk_widget_set_style(GTK_WIDGET(game->window), GTK_STYLE(style));

}

void start_screen_text(ctap_t *game) {

  PangoFontDescription* font_desc;
  GdkColor color;

  GtkWidget* title = gtk_label_new("Welcome to Tap Tap!");
  GtkWidget* sub = gtk_label_new("Press <ENTER> to start.");

  /* Change default font throughout the widget */
  font_desc = pango_font_description_from_string("Serif 40");
  gtk_widget_modify_font(title, font_desc);
  pango_font_description_free(font_desc);

  font_desc = pango_font_description_from_string("Serif 25");
  gtk_widget_modify_font(sub, font_desc);
  pango_font_description_free(font_desc);

  /* Change default color throughout the widget */
  gdk_color_parse("white", &color);
  gtk_widget_modify_fg(title, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_fg(sub, GTK_STATE_NORMAL, &color);

  GtkWidget* align_title = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(align_title), title);

  gtk_fixed_put(GTK_FIXED(game->content), align_title, 300, 300);
  gtk_fixed_put(GTK_FIXED(game->content), sub, 300, 400);
  gtk_fixed_put(GTK_FIXED(game->container), game->content, 0, 0);

}


void exit_button(ctap_t *game) {

  GtkWidget* event_box = gtk_event_box_new();

  GdkColor bg;
  gdk_color_parse("black", &bg);
  gtk_widget_modify_bg(event_box, GTK_STATE_NORMAL, &bg);

  GdkPixbuf* icon = create_pixbuf(IMG_EXIT);
  GtkWidget* exit_image = gtk_image_new_from_pixbuf(icon);
  gtk_container_add(GTK_CONTAINER(event_box), exit_image);

  g_signal_connect(G_OBJECT(event_box), "button_press_event", G_CALLBACK(gtk_main_quit), NULL);

  gtk_fixed_put(GTK_FIXED(game->container), event_box, gdk_screen_width() - 128, 0);
  gtk_widget_set_size_request(event_box, 128, 128);

}

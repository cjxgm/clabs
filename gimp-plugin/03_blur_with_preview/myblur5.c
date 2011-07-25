#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

typedef struct
{
  gint     radius;
  gboolean preview;
} MyBlurVals;

static void query       (void);
static void run         (const gchar      *name,
                         gint              nparams,
                         const GimpParam  *param,
                         gint             *nreturn_vals,
                         GimpParam       **return_vals);

static void blur        (GimpDrawable     *drawable,
                         GimpPreview      *preview);

static void init_mem    (guchar         ***row,
                         guchar          **outrow,
                         gint              num_bytes);
static void process_row (guchar          **row,
                         guchar           *outrow,
                         gint              x1,
                         gint              y1,
                         gint              width,
                         gint              height,
                         gint              channels,
                         gint              i);
static void shuffle     (GimpPixelRgn     *rgn_in,
                         guchar          **row,
                         gint              x1,
                         gint              y1,
                         gint              width,
                         gint              height,
                         gint              ypos);

static gboolean blur_dialog (GimpDrawable *drawable);

/* Set up default values for options */
static MyBlurVals bvals =
{
  3,  /* radius */
  1   /* preview */
};

GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,
  NULL,
  query,
  run
};

MAIN()

static void
query (void)
{
  static GimpParamDef args[] =
  {
    {
      GIMP_PDB_INT32,
      "run-mode",
      "Run mode"
    },
    {
      GIMP_PDB_IMAGE,
      "image",
      "Input image"
    },
    {
      GIMP_PDB_DRAWABLE,
      "drawable",
      "Input drawable"
    }
  };

  gimp_install_procedure (
    "plug-in-myblur5",
    "My blur 5 (preview)",
    "Blurs the image",
    "David Neary",
    "Copyright David Neary",
    "2004",
    "_My blur 5 (preview)...",
    "RGB*, GRAY*",
    GIMP_PLUGIN,
    G_N_ELEMENTS (args), 0,
    args, NULL);

  gimp_plugin_menu_register ("plug-in-myblur5",
                             "<Image>/Filters/Blur");
}

static void
run (const gchar      *name,
     gint              nparams,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  static GimpParam  values[1];
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;
  GimpRunMode       run_mode;
  GimpDrawable     *drawable;

  /* Setting mandatory output values */
  *nreturn_vals = 1;
  *return_vals  = values;

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  /* Getting run_mode - we won't display a dialog if 
   * we are in NONINTERACTIVE mode */
  run_mode = param[0].data.d_int32;

  /*  Get the specified drawable  */
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
      /* Get options last values if needed */
      gimp_get_data ("plug-in-myblur", &bvals);

      /* Display the dialog */
      if (! blur_dialog (drawable))
        return;
      break;

    case GIMP_RUN_NONINTERACTIVE:
      if (nparams != 4)
        status = GIMP_PDB_CALLING_ERROR;
      if (status == GIMP_PDB_SUCCESS)
        bvals.radius = param[3].data.d_int32;
      break;

    case GIMP_RUN_WITH_LAST_VALS:
      /*  Get options last values if needed  */
      gimp_get_data ("plug-in-myblur", &bvals);
      break;

    default:
      break;
    }

  blur (drawable, NULL);

  gimp_displays_flush ();
  gimp_drawable_detach (drawable);

  /*  Finally, set options in the core  */
  if (run_mode == GIMP_RUN_INTERACTIVE)
    gimp_set_data ("plug-in-myblur", &bvals, sizeof (MyBlurVals));

  return;
}

static void
blur (GimpDrawable *drawable,
      GimpPreview  *preview)
{
  gint         i, ii, channels;
  gint         x1, y1, x2, y2;
  GimpPixelRgn rgn_in, rgn_out;
  guchar     **row;
  guchar      *outrow;
  gint         width, height;

  if (! preview)
    gimp_progress_init ("My Blur...");

  /* Gets upper left and lower right coordinates,
   * and layers number in the image */
  if (preview)
    {
      gimp_preview_get_position (preview, &x1, &y1);
      gimp_preview_get_size (preview, &width, &height);
      x2 = x1 + width;
      y2 = y1 + height;
    }
  else
    {
      gimp_drawable_mask_bounds (drawable->drawable_id,
                                 &x1, &y1,
                                 &x2, &y2);
      width = x2 - x1;
      height = y2 - y1;
    }

  channels = gimp_drawable_bpp (drawable->drawable_id);

  /* Allocate a big enough tile cache */
  gimp_tile_cache_ntiles (2 * (drawable->width / gimp_tile_width () + 1));

  /* Initialises two PixelRgns, one to read original data,
   * and the other to write output data. That second one will
   * be merged at the end by the call to
   * gimp_drawable_merge_shadow() */
  gimp_pixel_rgn_init (&rgn_in,
                       drawable,
                       x1, y1,
                       width, height,
                       FALSE, FALSE);
  gimp_pixel_rgn_init (&rgn_out,
                       drawable,
                       x1, y1,
                       width, height,
                       preview == NULL, TRUE);

  /* Allocate memory for input and output tile rows */
  init_mem (&row, &outrow, width * channels);

  for (ii = -bvals.radius; ii <= bvals.radius; ii++)
    {
      gimp_pixel_rgn_get_row (&rgn_in,
                              row[bvals.radius + ii],
                              x1, y1 + CLAMP (ii, 0, height - 1),
                              width);
    }

  for (i = 0; i < height; i++)
    {
      /* To be done for each tile row */
      process_row (row,
                   outrow,
                   x1, y1,
                   width, height,
                   channels,
                   i);
      gimp_pixel_rgn_set_row (&rgn_out,
                              outrow,
                              x1, i + y1,
                              width);
      /* shift tile rows to insert the new one at the end */
      shuffle (&rgn_in,
               row,
               x1, y1,
               width, height,
               i);

      if (! preview && i % 16 == 0)
        gimp_progress_update ((gdouble) i / (gdouble) height);
    }

  /* We could also put that in a separate function but it's
   * rather simple */
  for (ii = 0; ii < 2 * bvals.radius + 1; ii++)
    g_free (row[ii]);

  g_free (row);
  g_free (outrow);

  /*  Update the modified region */
  if (preview)
    {
      gimp_drawable_preview_draw_region (GIMP_DRAWABLE_PREVIEW (preview),
                                         &rgn_out);
    }
  else
    {
      gimp_drawable_flush (drawable);
      gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
      gimp_drawable_update (drawable->drawable_id,
                            x1, y1,
                            width, height);
    }
}

static void
init_mem (guchar ***row,
          guchar  **outrow,
          gint      num_bytes)
{
  gint i;

  /* Allocate enough memory for row and outrow */
  *row = g_new (guchar *, (2 * bvals.radius + 1));

  for (i = -bvals.radius; i <= bvals.radius; i++)
    (*row)[i + bvals.radius] = g_new (guchar, num_bytes);

  *outrow = g_new (guchar, num_bytes);
}

static void
process_row (guchar **row,
             guchar  *outrow,
             gint     x1,
             gint     y1,
             gint     width,
             gint     height,
             gint     channels,
             gint     i)
{
  gint j;

  for (j = 0; j < width; j++)
    {
      gint k, ii, jj;
      gint left = (j - bvals.radius),
           right = (j + bvals.radius);

      /* For each layer, compute the average of the
       * (2r+1)x(2r+1) pixels */
      for (k = 0; k < channels; k++)
        {
          gint sum = 0;

          for (ii = 0; ii < 2 * bvals.radius + 1; ii++)
            for (jj = left; jj <= right; jj++)
              sum += row[ii][channels * CLAMP (jj, 0, width - 1) + k];

          outrow[channels * j + k] =
            sum / (4 * bvals.radius * bvals.radius + 4 * bvals.radius + 1);
        }
    }
}

static void
shuffle (GimpPixelRgn *rgn_in,
         guchar      **row,
         gint          x1,
         gint          y1,
         gint          width,
         gint          height,
         gint          ypos)
{
  gint    i;
  guchar *tmp_row;

  /* Get tile row (i + radius + 1) into row[0] */
  gimp_pixel_rgn_get_row (rgn_in,
                          row[0],
                          x1, MIN (ypos + bvals.radius + y1, y1 + height - 1),
                          width);

  /* Permute row[i] with row[i-1] and row[0] with row[2r] */
  tmp_row = row[0];
  for (i = 1; i < 2 * bvals.radius + 1; i++)
    row[i - 1] = row[i];
  row[2 * bvals.radius] = tmp_row;
}

static gboolean
blur_dialog (GimpDrawable *drawable)
{
  GtkWidget *dialog;
  GtkWidget *main_vbox;
  GtkWidget *main_hbox;
  GtkWidget *preview;
  GtkWidget *frame;
  GtkWidget *radius_label;
  GtkWidget *alignment;
  GtkWidget *spinbutton;
  GtkObject *spinbutton_adj;
  GtkWidget *frame_label;
  gboolean   run;

  gimp_ui_init ("myblur", FALSE);

  dialog = gimp_dialog_new ("My blur", "myblur",
                            NULL, 0,
                            gimp_standard_help_func, "plug-in-myblur",

                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OK,     GTK_RESPONSE_OK,

                            NULL);

  main_vbox = gtk_vbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), main_vbox);
  gtk_widget_show (main_vbox);

  preview = gimp_drawable_preview_new (drawable, &bvals.preview);
  gtk_box_pack_start (GTK_BOX (main_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  frame = gtk_frame_new (NULL);
  gtk_widget_show (frame);
  gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame), 6);

  alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment);
  gtk_container_add (GTK_CONTAINER (frame), alignment);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 6, 6, 6, 6);

  main_hbox = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (main_hbox);
  gtk_container_add (GTK_CONTAINER (alignment), main_hbox);

  radius_label = gtk_label_new_with_mnemonic ("_Radius:");
  gtk_widget_show (radius_label);
  gtk_box_pack_start (GTK_BOX (main_hbox), radius_label, FALSE, FALSE, 6);
  gtk_label_set_justify (GTK_LABEL (radius_label), GTK_JUSTIFY_RIGHT);

  spinbutton = gimp_spin_button_new (&spinbutton_adj, bvals.radius, 
                                     1, 32, 1, 1, 1, 5, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox), spinbutton, FALSE, FALSE, 0);
  gtk_widget_show (spinbutton);

  frame_label = gtk_label_new ("<b>Modify radius</b>");
  gtk_widget_show (frame_label);
  gtk_frame_set_label_widget (GTK_FRAME (frame), frame_label);
  gtk_label_set_use_markup (GTK_LABEL (frame_label), TRUE);

  g_signal_connect_swapped (preview, "invalidated",
                            G_CALLBACK (blur),
                            drawable);
  g_signal_connect_swapped (spinbutton_adj, "value_changed",
                            G_CALLBACK (gimp_preview_invalidate),
                            preview);

  blur (drawable, GIMP_PREVIEW (preview));

  g_signal_connect (spinbutton_adj, "value_changed",
                    G_CALLBACK (gimp_int_adjustment_update),
                    &bvals.radius);
  gtk_widget_show (dialog);

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}


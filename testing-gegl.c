/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (scale, _("Saturation"), 1.0)
  value_range (0.0, 1000.0)
  ui_range (0.0, 20.0)
  ui_gamma (1.5)

property_double (contrast, _("Contrast"),  1.0)
   description  (_("Magnitude of contrast scaling >1.0 brighten < 1.0 darken"))
   value_range  (-5.0, 5.0)
   ui_range     (0.0, 2.0)

property_double (brightness, _("Brightness"), 0.0)
   description  (_("Amount to increase brightness"))
   value_range  (-3.0, 3.0)
   ui_range     (-1.0, 1.0)

property_double (ot, _("Original temperature"), 6500)
  description(_("Estimated temperature of the light source in Kelvin "
                "the image was taken with."))
  value_range (-1000, 12000)
  ui_meta     ("unit", "kelvin")

property_double (it, _("Intended temperature"), 6500)
  description(_("Corrected estimation of the temperature of the light "
                "source in Kelvin."))
  value_range (-1000, 12000)
  ui_meta     ("unit", "kelvin")

property_double (chroma, _("Chroma"), 0.0)
   description  (_("Chroma adjustment"))
   value_range  (-100.0, 100.0)

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-100.0, 100.0)

property_double (sharpenradius, _("Sharpen Radius"), 0.0)
    description(_("Expressed as standard deviation, in pixels"))
    value_range (0.0, 1500)
    ui_range    (0.0, 40.0)
    ui_gamma    (3.0)
    ui_meta     ("unit", "pixel-distance")

property_double (sharpenscale, _("Sharpen Amount"), 0.5)
    description(_("Scaling factor for unsharp-mask, the strength of effect"))
    value_range (0.0, 300.0)
    ui_range    (0.0, 10.0)
    ui_gamma    (3.0)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     testing_gegl
#define GEGL_OP_C_SOURCE testing-gegl.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *bc, *sat, *ct, *us, *hc;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  sat    = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

  bc = gegl_node_new_child (gegl,
                                  "operation", "gegl:brightness-contrast",
                                  NULL);

  ct = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-temperature",
                                  NULL);

  hc = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


  us = gegl_node_new_child (gegl,
                                  "operation", "gegl:unsharp-mask",
                                  NULL);


  gegl_node_link_many (input, sat, bc, ct, hc, us, output, NULL);


  gegl_operation_meta_redirect (operation, "brightness", bc, "brightness");

  gegl_operation_meta_redirect (operation, "contrast", bc, "contrast");


  gegl_operation_meta_redirect (operation, "scale", sat, "scale");

  gegl_operation_meta_redirect (operation, "ot", ct, "original-temperature");

  gegl_operation_meta_redirect (operation, "it", ct, "intended-temperature");

  gegl_operation_meta_redirect (operation, "lightness", hc, "lightness");

  gegl_operation_meta_redirect (operation, "chroma", hc, "chroma");

  gegl_operation_meta_redirect (operation, "sharpenradius", us, "std-dev");

  gegl_operation_meta_redirect (operation, "sharpenscale", us, "scale");





}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:common-adjustments",
    "title",       _("Common adjustment filters"),
    "categories",  "Generic",
    "reference-hash", "45ed5656a238a5125700fc254001b2ac",
    "description", _("Common photography filters all in one place. (pro tip using presets and back up layers is a smart idea "
                     ""),
    NULL);
}

#endif

/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gegl.h>

#include "core-types.h"

#include "base/gimplut.h"
#include "base/lut-funcs.h"

/* temp */
#include "gimp.h"
#include "gimpimage.h"

#include "gimpdrawable.h"
#include "gimpdrawable-invert.h"
#include "gimpdrawable-operation.h"
#include "gimpdrawable-process.h"
#include "gimpprogress.h"

#include "gimp-intl.h"


void
gimp_drawable_invert (GimpDrawable *drawable,
                      GimpProgress *progress)
{
  g_return_if_fail (GIMP_IS_DRAWABLE (drawable));
  g_return_if_fail (gimp_item_is_attached (GIMP_ITEM (drawable)));
  g_return_if_fail (progress == NULL || GIMP_IS_PROGRESS (progress));

  if (gimp_use_gegl (gimp_item_get_image (GIMP_ITEM (drawable))->gimp))
    {
      GeglNode *invert;

      invert = g_object_new (GEGL_TYPE_NODE, "operation", "gegl:invert", NULL);

      gimp_drawable_apply_operation (drawable, progress, _("Invert"),
                                     invert, TRUE);
      g_object_unref (invert);
    }
  else
    {
      GimpLut *lut = invert_lut_new (gimp_drawable_bytes (drawable));

      gimp_drawable_process_lut (drawable, progress, _("Invert"), lut);
      gimp_lut_free (lut);
    }
}
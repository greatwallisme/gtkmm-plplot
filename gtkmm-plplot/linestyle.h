#ifndef GTKMMPLPLOTLINESTYLE_H
#define GTKMMPLPLOTLINESTYLE_H

namespace Gtk {
  namespace PLplot {
    enum LineStyle {
      CONTINUOUS = 1,
      SHORT_DASH_SHORT_GAP,
      LONG_DASH_LONG_GAP,
      LONG_DASH_SHORT_GAP,
      LONG_DASH_SHORT_GAP_SHORT_DASH_SHORT_GAP,
      LONG_DASH_SHORT_GAP_LONG_DASH_SHORT_GAP,
      SHORT_DASH_SHORT_GAP_MEDIUM_DASH_MEDIUM_GAP_LONG_DASH_LONG_GAP,
      SHORT_DASH_SHORT_GAP_MEDIUM_DASH_SHORT_GAP_LONG_DASH_SHORT_GAP
    };
  }
}
#endif
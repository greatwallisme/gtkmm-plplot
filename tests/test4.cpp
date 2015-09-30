/*
Copyright (C) 2015 Tom Schoonjans

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtkmm-plplot/gtkmm-plplot.h>
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <iostream>
#include <glib.h>
#include <cmath>

namespace Test4 {
  class CheckButton : public Gtk::CheckButton {
  private:
    //we are not using any methods specific to Plot2D, so we can safely use Plot here
    Gtk::PLplot::Plot2D *plot;
  public:
    CheckButton(Glib::ustring text) : Gtk::CheckButton(text), plot(nullptr) {}
    void on_toggled() final {
      if (!plot)
        return;
      if (get_active())
        plot->show();
      else
        plot->hide();
    }
    void connect_plot(Gtk::PLplot::Plot2D *_plot) {
      plot = _plot;
      plot->hide_legend();
    }
  };

  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Test4::CheckButton checkbutton1;
    Test4::CheckButton checkbutton2;
    Test4::CheckButton checkbutton3;
  public:
    Window() : canvas(), checkbutton1("Plot 1"), checkbutton2("Plot 2"), checkbutton3("Plot 3") {
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test4");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      canvas.set_background_color(Gdk::RGBA("Light Gray"));
      grid.set_column_homogeneous(true);
      checkbutton1.set_vexpand(false);
      checkbutton1.set_hexpand(false);
      checkbutton1.set_halign(Gtk::ALIGN_CENTER);
      checkbutton2.set_vexpand(false);
      checkbutton2.set_hexpand(false);
      checkbutton2.set_halign(Gtk::ALIGN_CENTER);
      checkbutton3.set_vexpand(false);
      checkbutton3.set_hexpand(false);
      checkbutton3.set_halign(Gtk::ALIGN_CENTER);

      std::valarray<double> x_va = Gtk::PLplot::indgen_va(1000)/50.0 - 10.0;
      std::valarray<double> y_va1 = sinh(x_va);
      std::valarray<double> y_va2 = cosh(x_va);
      std::valarray<double> y_va3 = tanh(x_va);

      //generate the data, the plot, add them to the canvas and use the return value to pass it to the checkbutton
      checkbutton1.connect_plot(
        dynamic_cast<Gtk::PLplot::Plot2D *>(
          canvas.add_plot(
            Gtk::PLplot::Plot2D(
              Gtk::PLplot::PlotData2D(
                x_va,
                y_va1,
                Gdk::RGBA("blue"),
              Gtk::PLplot::LineStyle::CONTINUOUS,
              3.0),
              "X-axis",
              "Y-axis",
              "Hyperbolic sine",
              0.5,
              0.5,
              0.0,
              0.0
            )
          )
        )
      );
      checkbutton2.connect_plot(
        dynamic_cast<Gtk::PLplot::Plot2D *>(
          canvas.add_plot(
            Gtk::PLplot::Plot2D(
              Gtk::PLplot::PlotData2D(
                x_va,
                y_va2,
                Gdk::RGBA("red"),
                Gtk::PLplot::LineStyle::CONTINUOUS,
                3.0),
              "X-axis",
              "Y-axis",
              "Hyperbolic cosine",
              0.5,
              0.5,
              0.5,
              0.0
            )
          )
        )
      );
      checkbutton3.connect_plot(
        dynamic_cast<Gtk::PLplot::Plot2D *>(
          canvas.add_plot(
            Gtk::PLplot::Plot2D(
              Gtk::PLplot::PlotData2D(
                x_va,
                y_va3,
                Gdk::RGBA("green"),
                Gtk::PLplot::LineStyle::CONTINUOUS,
                3.0),
              "X-axis",
              "Y-axis",
              "Hyperbolic tangent",
              0.4,
              0.4,
              0.2,
              0.55
            )
          )
        )
      );

      checkbutton1.set_active();
      checkbutton2.set_active(false);
      checkbutton3.set_active();

      //let's override the background color of the second plot and hide it to make the surprise complete!!
      canvas.get_plot(1)->set_background_color(Gdk::RGBA("Yellow Green"));
      canvas.get_plot(1)->hide();
      //also let's disable the region selection, for no reason whatsoever!
      dynamic_cast<Gtk::PLplot::RegionSelection *>(canvas.get_plot(1))->set_region_selectable(false);
      //let's override the default region of this plot, this is NOT influenced by set_region_selectable()!!!
      dynamic_cast<Gtk::PLplot::RegionSelection *>(canvas.get_plot(1))->set_region(-10, 10, -10, 10);
      //we can also change some other colors
      canvas.get_plot(1)->set_axes_color(Gdk::RGBA("Blue"));
      canvas.get_plot(1)->set_titles_color(Gdk::RGBA("Purple"));
      dynamic_cast<Gtk::PLplot::Plot2D *>(canvas.get_plot(1))->set_box_style(Gtk::PLplot::BOX_TICKS_TICK_LABELS_MAIN_AXES_MAJOR_MINOR_TICK_GRID);

      //lets'give the third plot a nice opaque background color
      canvas.get_plot(2)->set_background_color(Gdk::RGBA("White"));

      grid.attach(checkbutton1, 0, 0, 1, 1);
      grid.attach(checkbutton2, 1, 0, 1, 1);
      grid.attach(checkbutton3, 2, 0, 1, 1);
      grid.attach(canvas, 0, 1, 3, 1);

      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}

int main(int argc, char *argv[]) {
  Glib::set_application_name("gtkmm-plplot-test4");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test4");
  Test4::Window window;

  return app->run(window);
}
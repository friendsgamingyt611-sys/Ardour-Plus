/* GTK Event Controller Abstraction Layer
 * Isolates widget event handling for GTK2 -> GTK4 migration.
 */

#ifndef __GTKMM2EXT_EVENT_CONTROLLER_H__
#define __GTKMM2EXT_EVENT_CONTROLLER_H__

#include <sigc++/sigc++.h>
#include <ydk/gdk.h>
#include "gtkmm2ext/visibility.h"

namespace Gtk {
	class Widget;
}

namespace Gtkmm2ext {

class LIBGTKMM2EXT_API EventController
{
public:
	enum EventType {
		Click,
		Motion,
		Key,
		Scroll
	};

	EventController(Gtk::Widget& widget);
	~EventController();

	static bool get_coords(GdkEvent const* ev, double& x, double& y);
	static bool get_state(GdkEvent const* ev, uint32_t& state);

	void attach();
	void detach();

	sigc::signal<bool, GdkEventButton*>& signal_button_press();
	sigc::signal<bool, GdkEventButton*>& signal_button_release();
	sigc::signal<bool, GdkEventKey*>&    signal_key_press();
	sigc::signal<bool, GdkEventMotion*>& signal_motion_notify();

private:
	Gtk::Widget& _widget;
	bool         _attached;

	sigc::signal<bool, GdkEventButton*> _button_press_signal;
	sigc::signal<bool, GdkEventButton*> _button_release_signal;
	sigc::signal<bool, GdkEventKey*>    _key_press_signal;
	sigc::signal<bool, GdkEventMotion*> _motion_notify_signal;

	bool on_button_press(GdkEventButton* ev);
	bool on_button_release(GdkEventButton* ev);
	bool on_key_press(GdkEventKey* ev);
	bool on_motion_notify(GdkEventMotion* ev);
};

} // namespace Gtkmm2ext

#endif /* __GTKMM2EXT_EVENT_CONTROLLER_H__ */

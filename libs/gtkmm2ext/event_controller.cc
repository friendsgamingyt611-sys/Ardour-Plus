/* GTK Event Controller Abstraction Layer
 * Implementation for widget event signal isolation.
 */

#include <sigc++/sigc++.h>
#include <ytkmm/widget.h>
#include "gtkmm2ext/event_controller.h"

namespace Gtkmm2ext {

EventController::EventController(Gtk::Widget& widget)
	: _widget(widget)
	, _attached(false)
{
	attach();
}

EventController::~EventController()
{
	detach();
}

bool
EventController::get_coords(GdkEvent const* ev, double& x, double& y)
{
	if (!ev) return false;
	return gdk_event_get_coords(ev, &x, &y) != 0;
}

bool
EventController::get_state(GdkEvent const* ev, uint32_t& state)
{
	if (!ev) return false;
	GdkModifierType mod_state;
	if (gdk_event_get_state(ev, &mod_state)) {
		state = static_cast<uint32_t>(mod_state);
		return true;
	}
	return false;
}

void
EventController::attach()
{
	if (_attached) {
		return;
	}
	_widget.signal_button_press_event().connect(sigc::mem_fun(*this, &EventController::on_button_press), false);
	_widget.signal_button_release_event().connect(sigc::mem_fun(*this, &EventController::on_button_release), false);
	_widget.signal_key_press_event().connect(sigc::mem_fun(*this, &EventController::on_key_press), false);
	_widget.signal_motion_notify_event().connect(sigc::mem_fun(*this, &EventController::on_motion_notify), false);
	_attached = true;
}

void
EventController::detach()
{
	_attached = false;
}

sigc::signal<bool, GdkEventButton*>&
EventController::signal_button_press()
{
	return _button_press_signal;
}

sigc::signal<bool, GdkEventButton*>&
EventController::signal_button_release()
{
	return _button_release_signal;
}

sigc::signal<bool, GdkEventKey*>&
EventController::signal_key_press()
{
	return _key_press_signal;
}

sigc::signal<bool, GdkEventMotion*>&
EventController::signal_motion_notify()
{
	return _motion_notify_signal;
}

bool
EventController::on_button_press(GdkEventButton* ev)
{
	return _button_press_signal.emit(ev);
}

bool
EventController::on_button_release(GdkEventButton* ev)
{
	return _button_release_signal.emit(ev);
}

bool
EventController::on_key_press(GdkEventKey* ev)
{
	return _key_press_signal.emit(ev);
}

bool
EventController::on_motion_notify(GdkEventMotion* ev)
{
	return _motion_notify_signal.emit(ev);
}

} // namespace Gtkmm2ext

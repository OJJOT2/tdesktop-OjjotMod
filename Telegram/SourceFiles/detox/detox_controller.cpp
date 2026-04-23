/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "detox/detox_controller.h"

#include "core/application.h"
#include "core/core_settings.h"
#include "window/window_session_controller.h"
#include "main/main_domain.h"

namespace Detox {

Controller::Controller(not_null<Window::SessionController*> window)
: _window(window)
, _timer([=] { onTimerExpired(); }) {
	updateStateFromSettings();

	Core::App().settings().detoxModeEnabledValue(
	) | rpl::start_with_next([=](bool) {
		updateStateFromSettings();
	}, _lifetime);
}

Mode Controller::mode() const {
	return _mode.current();
}

rpl::producer<Mode> Controller::modeValue() const {
	return _mode.value();
}

void Controller::startCheckUpdates(int minutes) {
	setMode(Mode::CheckUpdates);
	startTimer(minutes);
}

void Controller::startFileMode(int minutes) {
	setMode(Mode::FileMode);
	startTimer(minutes);
}

void Controller::startReplyMode(int minutes) {
	setMode(Mode::ReplyMode);
	startTimer(minutes);
}

void Controller::exitDetoxMode() {
	Core::App().settings().setDetoxModeEnabled(false);
	Core::App().domain().notifyUnreadBadgeChanged();
	updateStateFromSettings();
}

void Controller::returnToScreen() {
	_timer.cancel();
	setMode(Mode::Screen);
}

crl::time Controller::remainingTime() const {
	if (!_timer.isActive()) {
		return 0;
	}
	const auto now = crl::now();
	return (_timerEndTime > now) ? (_timerEndTime - now) : 0;
}

rpl::producer<crl::time> Controller::timeUpdates() const {
	return _timeUpdates.events();
}

void Controller::updateStateFromSettings() {
	if (Core::App().settings().detoxModeEnabled()) {
		if (_mode.current() == Mode::Idle) {
			setMode(Mode::Screen);
		}
	} else {
		_timer.cancel();
		setMode(Mode::Idle);
	}
}

void Controller::setMode(Mode mode) {
	if (_mode.current() != mode) {
		_mode = mode;
	}
}

void Controller::startTimer(int minutes) {
	const auto duration = crl::time(minutes) * 60 * 1000;
	_timerEndTime = crl::now() + duration;
	_timer.callOnce(duration);
	_timeUpdates.fire(remainingTime());
}

void Controller::onTimerExpired() {
	returnToScreen();
}

} // namespace Detox

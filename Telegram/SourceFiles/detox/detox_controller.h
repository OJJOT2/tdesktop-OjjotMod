/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/timer.h"
#include "ui/effects/animations.h"

namespace Window {
class SessionController;
} // namespace Window

namespace Detox {

enum class Mode {
	Idle,
	Screen,
	CheckUpdates,
	FileMode,
	ReplyMode,
};

class Controller final {
public:
	explicit Controller(not_null<Window::SessionController*> window);

	[[nodiscard]] not_null<Window::SessionController*> window() const {
		return _window;
	}

	[[nodiscard]] Mode mode() const;
	[[nodiscard]] rpl::producer<Mode> modeValue() const;

	void startCheckUpdates(int minutes);
	void startFileMode(int minutes);
	void startReplyMode(int minutes);
	void exitDetoxMode();
	void returnToScreen();

	[[nodiscard]] crl::time remainingTime() const;
	[[nodiscard]] rpl::producer<crl::time> timeUpdates() const;

	void updateStateFromSettings();

private:
	void setMode(Mode mode);
	void startTimer(int minutes);
	void onTimerExpired();

	const not_null<Window::SessionController*> _window;
	rpl::variable<Mode> _mode = Mode::Idle;
	
	crl::time _timerEndTime = 0;
	base::Timer _timer;
	rpl::event_stream<crl::time> _timeUpdates;
	
	rpl::lifetime _lifetime;
};

} // namespace Detox

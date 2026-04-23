/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/rp_widget.h"

namespace Window {
class SessionController;
} // namespace Window

namespace Detox {

class Screen final : public Ui::RpWidget {
public:
	Screen(QWidget *parent, not_null<Window::SessionController*> window);

private:
	void setupContent();

	const not_null<Window::SessionController*> _window;
};

} // namespace Detox

/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "detox/detox_screen.h"

#include "detox/detox_controller.h"
#include "window/window_session_controller.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/padding_wrap.h"
#include "lang/lang_keys.h"
#include "styles/style_detox.h"

namespace Detox {

Screen::Screen(QWidget *parent, not_null<Window::SessionController*> window)
: Ui::RpWidget(parent)
, _window(window) {
	setupContent();
}

void Screen::setupContent() {
	auto inner = Ui::CreateChild<Ui::VerticalLayout>(this);

	auto title = inner->add(
		object_ptr<Ui::FlatLabel>(
			inner,
			tr::lng_detox_screen_title(),
			st::detoxTitleFont),
		st::detoxTitlePadding);
	
	title->setTextColorOverride(st::detoxTitleColor->c);

	Ui::AddSkip(inner, st::detoxButtonSpacing);

	// TODO: Replace with custom styled buttons and confirmation box.
	auto btnCheck = inner->add(
		object_ptr<Ui::RoundButton>(
			inner,
			tr::lng_detox_check_updates(),
			st::detoxButton));
	
	Ui::AddSkip(inner, st::detoxButtonSpacing);

	auto btnFile = inner->add(
		object_ptr<Ui::RoundButton>(
			inner,
			tr::lng_detox_pick_file(),
			st::detoxButton));

	Ui::AddSkip(inner, st::detoxButtonSpacing);

	auto btnReply = inner->add(
		object_ptr<Ui::RoundButton>(
			inner,
			tr::lng_detox_reply(),
			st::detoxButton));

	Ui::AddSkip(inner, st::detoxButtonSpacing);

	auto btnExit = inner->add(
		object_ptr<Ui::RoundButton>(
			inner,
			tr::lng_detox_exit(),
			st::detoxButton));

	btnExit->setClickedCallback([=] {
		// Just for testing... we would trigger the exit or go to settings.
		// _window->detoxController().exitDetoxMode();
	});

	widthValue() | rpl::start_with_next([=](int newWidth) {
		inner->resizeToWidth(newWidth);
		inner->moveToLeft(
			(newWidth - inner->width()) / 2,
			(height() - inner->height()) / 2);
	}, inner->lifetime());

	heightValue() | rpl::start_with_next([=](int newHeight) {
		inner->moveToLeft(
			(width() - inner->width()) / 2,
			(newHeight - inner->height()) / 2);
	}, inner->lifetime());
}

} // namespace Detox

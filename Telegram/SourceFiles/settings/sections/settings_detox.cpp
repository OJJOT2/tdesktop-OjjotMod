/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/sections/settings_detox.h"

#include "core/application.h"
#include "core/core_settings.h"
#include "lang/lang_keys.h"
#include "main/main_domain.h"
#include "settings/settings_builder.h"
#include "settings/sections/settings_main.h"
#include "ui/widgets/buttons.h"
#include "ui/wrap/vertical_layout.h"

namespace Settings {

void BuildDetoxSection(Builder::SectionBuilder &builder) {
	builder.addDivider();
	builder.addSkip();

	auto &settings = Core::App().settings();

	// Detox Mode Toggle
	builder.addCheckbox({
		.title = tr::lng_detox_mode(),
		.about = tr::lng_detox_mode_about(),
		.value = settings.detoxModeEnabled(),
		.checkedCb = [=](bool checked) {
			auto &s = Core::App().settings();
			if (s.detoxModeEnabled() != checked) {
				s.setDetoxModeEnabled(checked);
				Core::App().domain().notifyUnreadBadgeChanged();
			}
		},
	});

	builder.addSkip();
	builder.addDivider();
	builder.addSkip();

	// Notification Counter Mode
	builder.addHeader(tr::lng_detox_counter_mode());

	builder.addRadio({
		.title = tr::lng_detox_counter_all(),
		.id = 0,
		.value = settings.detoxCounterImportantOnly() ? 1 : 0,
		.checkedCb = [=](int value) {
			auto &s = Core::App().settings();
			if (s.detoxCounterImportantOnly() != (value == 1)) {
				s.setDetoxCounterImportantOnly(value == 1);
				Core::App().domain().notifyUnreadBadgeChanged();
			}
		},
	});

	builder.addRadio({
		.title = tr::lng_detox_counter_important(),
		.id = 1,
		.value = settings.detoxCounterImportantOnly() ? 1 : 0,
		.checkedCb = [=](int value) {
			auto &s = Core::App().settings();
			if (s.detoxCounterImportantOnly() != (value == 1)) {
				s.setDetoxCounterImportantOnly(value == 1);
				Core::App().domain().notifyUnreadBadgeChanged();
			}
		},
	});

	builder.addSkip();
}

const auto kMeta = Builder::BuildHelper({
	.id = Detox::Id(),
	.parentId = MainId(),
	.title = &tr::lng_detox_mode,
	.icon = &st::menuIconPermissions,
}, [](Builder::SectionBuilder &builder) {
	BuildDetoxSection(builder);
});

const Builder::SectionBuildMethod kDetoxSection = kMeta.build;

Detox::Detox(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent(controller);
}

rpl::producer<QString> Detox::title() {
	return tr::lng_detox_mode();
}

void Detox::setupContent(not_null<Window::SessionController*> controller) {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kDetoxSection);
	Ui::ResizeFitChild(this, content);
}

Type DetoxId() {
	return Detox::Id();
}

} // namespace Settings

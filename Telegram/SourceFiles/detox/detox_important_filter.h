/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/flat_set.h"

namespace Main {
class Session;
} // namespace Main

namespace Detox {

class ImportantFilter final {
public:
	explicit ImportantFilter(not_null<Main::Session*> session);

	[[nodiscard]] bool isImportant(PeerId peerId) const;
	void setImportant(PeerId peerId, bool important);
	void toggle(PeerId peerId);

	[[nodiscard]] const base::flat_set<PeerId> &list() const;
	[[nodiscard]] rpl::producer<PeerId> changes() const;

private:
	void load();
	void save() const;

	const not_null<Main::Session*> _session;
	base::flat_set<PeerId> _peers;
	rpl::event_stream<PeerId> _changes;

};

} // namespace Detox

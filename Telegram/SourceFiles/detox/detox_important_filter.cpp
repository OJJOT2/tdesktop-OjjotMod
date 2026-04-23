/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "detox/detox_important_filter.h"

#include "core/application.h"
#include "core/core_settings.h"
#include "main/main_session.h"

#include <QtCore/QDataStream>

namespace Detox {
namespace {

constexpr auto kPrefKey = "detox-important-peers";

[[nodiscard]] QByteArray SerializePeers(
		const base::flat_set<PeerId> &peers) {
	auto result = QByteArray();
	auto stream = QDataStream(&result, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_5_3);
	stream << qint32(peers.size());
	for (const auto &peer : peers) {
		stream << quint64(peer.value);
	}
	return result;
}

[[nodiscard]] base::flat_set<PeerId> DeserializePeers(
		const QByteArray &data) {
	auto result = base::flat_set<PeerId>();
	if (data.isEmpty()) {
		return result;
	}
	auto stream = QDataStream(data);
	stream.setVersion(QDataStream::Qt_5_3);
	auto count = qint32(0);
	stream >> count;
	if (stream.status() != QDataStream::Ok || count < 0 || count > 100000) {
		return result;
	}
	result.reserve(count);
	for (auto i = 0; i < count; ++i) {
		auto raw = quint64(0);
		stream >> raw;
		if (stream.status() != QDataStream::Ok) {
			break;
		}
		result.emplace(PeerId(raw));
	}
	return result;
}

} // namespace

ImportantFilter::ImportantFilter(not_null<Main::Session*> session)
: _session(session) {
	load();
}

bool ImportantFilter::isImportant(PeerId peerId) const {
	return _peers.contains(peerId);
}

void ImportantFilter::setImportant(PeerId peerId, bool important) {
	if (important) {
		if (_peers.emplace(peerId).second) {
			save();
			_changes.fire_copy(peerId);
		}
	} else {
		if (_peers.remove(peerId)) {
			save();
			_changes.fire_copy(peerId);
		}
	}
}

void ImportantFilter::toggle(PeerId peerId) {
	setImportant(peerId, !isImportant(peerId));
}

const base::flat_set<PeerId> &ImportantFilter::list() const {
	return _peers;
}

rpl::producer<PeerId> ImportantFilter::changes() const {
	return _changes.events();
}

void ImportantFilter::load() {
	auto &settings = Core::App().settings();
	const auto data = settings.readPref<QByteArray>(kPrefKey);
	_peers = DeserializePeers(data);
}

void ImportantFilter::save() const {
	auto &settings = Core::App().settings();
	settings.writePref<QByteArray>(kPrefKey, SerializePeers(_peers));
	Core::App().saveSettingsDelayed();
}

} // namespace Detox

<?php
$names = file_get_contents(dirname(__FILE__) . '/event_type_names.json');
$names = json_decode($names, true);
?>// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_type_names.h
// Description: Event Type Names
//      Author: Ziming Li
//     Created: <?php echo date('Y-m-d') . "\n"; ?>
// -------------------------------------------------
// Copyright (C) <?php echo date('Y'); ?> MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_TYPE_NAMES_H
#define BLINKIT_BLINK_EVENT_TYPE_NAMES_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
namespace event_type_names {

<?php
foreach ($names as $v) {
    $var_name = isset($v['var_name']) ? $v['var_name'] : ucfirst($v['name']);
    echo "extern const WTF::AtomicString &k$var_name;\n";
}
?>

constexpr unsigned kNamesCount = <?php echo count($names); ?>;

void Init(void);

} // namespace event_type_names
} // namespace blink

#endif // BLINKIT_BLINK_EVENT_TYPE_NAMES_H

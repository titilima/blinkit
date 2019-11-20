<?php
$names = file_get_contents(dirname(__FILE__) . '/event_type_names.json');
$names = json_decode($names, true);
?>// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: event_type_names.cpp
// Description: Event Type Names
//      Author: Ziming Li
//     Created: <?php echo date('Y-m-d') . "\n"; ?>
// -------------------------------------------------
// Copyright (C) <?php echo date('Y'); ?> MingYang Software Technology.
// -------------------------------------------------

#include "event_type_names.h"

#include <iterator>

namespace blink {
namespace event_type_names {

static void* names_storage[kNamesCount * ((sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *))];

<?php
$i = 0;
foreach ($names as $v) {
    $var_name = isset($v['var_name']) ? $v['var_name'] : ucfirst($v['name']);
    echo "const AtomicString &k$var_name = reinterpret_cast<AtomicString *>(&names_storage)[$i];\n";
	++$i;
}
?>

void Init(void)
{
    static bool s_isLoaded = false;
    if (s_isLoaded)
        return;
    s_isLoaded = true;

    struct NameEntry {
        const char *name;
        unsigned hash;
        unsigned char length;
    };

    static const NameEntry kNames[] = {
<?php
foreach ($names as $v) {
    $name = $v['name'];
    $hash = $v['hash'];
    $len = strlen($v['name']);
    echo <<<STR
        { "$name", $hash, $len },

STR;
}
?>
    };

    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        void *address = reinterpret_cast<AtomicString *>(&names_storage) + i;
        new (address) AtomicString(impl);
    }
}

} // namespace event_type_names
} // namespace blink

<?php
$names = file_get_contents(dirname(__FILE__) . '/html_names.json');
$names = json_decode($names, true);
?>// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_names.h
// Description: HTML Names
//      Author: Ziming Li
//     Created: <?php echo date('Y-m-d') . "\n"; ?>
// -------------------------------------------------
// Copyright (C) <?php echo date('Y'); ?> MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_NAMES_H
#define BLINKIT_BLINK_HTML_NAMES_H

#pragma once

#include "third_party/blink/renderer/core/dom/qualified_name.h"

namespace blink {

class HTMLQualifiedName : public QualifiedName {};

namespace html_names {

extern const WTF::AtomicString &xhtmlNamespaceURI;

// Tags
<?php
$tag_count = 0;
foreach ($names as $v) {
    if ($v['is_tag']) {
        $var_name = isset($v['var_name']) ? $v['var_name'] : ucfirst($v['name']);
        echo "extern const blink::HTMLQualifiedName &k$var_name" . "Tag;\n";
        ++$tag_count;
    }
}
?>

constexpr unsigned kTagsCount = <?php echo $tag_count; ?>;

// Attributes
<?php
$attr_count = 0;
foreach ($names as $v) {
    if ($v['is_attr']) {
        $var_name = isset($v['var_name']) ? $v['var_name'] : ucfirst($v['name']);
        echo "extern const blink::QualifiedName &k$var_name" . "Attr;\n";
        ++$attr_count;
    }
}
?>

constexpr unsigned kAttrsCount = <?php echo $attr_count; ?>;

void Init(void);

} // namespace html_names
} // namespace blink

#endif // BLINKIT_BLINK_HTML_NAMES_H

<?php
$names = file_get_contents(dirname(__FILE__) . '/html_names.json');
$names = json_decode($names, true);
?>// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_names.cpp
// Description: HTML Names
//      Author: Ziming Li
//     Created: <?php echo date('Y-m-d') . "\n"; ?>
// -------------------------------------------------
// Copyright (C) <?php echo date('Y'); ?> MingYang Software Technology.
// -------------------------------------------------

#include "html_names.h"

#include <iterator>

namespace blink {
namespace html_names {

static void* ns_uri_storage[(sizeof(AtomicString) + sizeof(void *) - 1) / sizeof(void *)];
const AtomicString &xhtmlNamespaceURI = *reinterpret_cast<AtomicString *>(&ns_uri_storage);

// Tags
static void* tag_storage[kTagsCount * ((sizeof(HTMLQualifiedName) + sizeof(void *) - 1) / sizeof(void *))];
<?php
$i = 0;
foreach ($names as $v) {
    if ($v['is_tag']) {
        $var_name = isset($v['var_name']) ? $v['var_name'] : ucfirst($v['name']);
        echo "const HTMLQualifiedName &k$var_name" . "Tag = reinterpret_cast<HTMLQualifiedName *>(&tag_storage)[$i];\n";
        ++$i;
    }
}
?>

// Attributes
static void* attr_storage[kAttrsCount * ((sizeof(QualifiedName) + sizeof(void *) - 1) / sizeof(void *))];
<?php
$i = 0;
foreach ($names as $v) {
    if ($v['is_attr']) {
        $var_name = isset($v['var_name']) ? $v['var_name'] : ucfirst($v['name']);
        echo "const QualifiedName &k$var_name" . "Attr = reinterpret_cast<QualifiedName *>(&attr_storage)[$i];\n";
        ++$i;
    }
}
?>

void Init(void)
{
    struct NameEntry {
        const char* name;
        unsigned hash;
        unsigned char length;
        unsigned char isTag;
        unsigned char isAttr;
    };

    // Namespace
    // Use placement new to initialize the globals.
    AtomicString ns_uri("http://www.w3.org/1999/xhtml");
    void *dst = const_cast<AtomicString *>(&xhtmlNamespaceURI);
    new (dst) AtomicString(ns_uri);

    static const NameEntry kNames[] = {
<?php
foreach ($names as $v) {
    $name = $v['name'];
    $hash = $v['hash'];
    $len = strlen($v['name']);
    $tag = $v['is_tag'] ? 1 : 0;
    $attr = $v['is_attr'] ? 1 : 0;
    echo <<<STR
        { "$name", $hash, $len, $tag, $attr },

STR;
}
?>
    };

    size_t tag_i = 0, attr_i = 0;
    for (size_t i = 0; i < std::size(kNames); ++i)
    {
        StringImpl *impl = StringImpl::CreateStatic(kNames[i].name, kNames[i].length, kNames[i].hash);
        if (kNames[i].isTag)
        {
            void *address = reinterpret_cast<HTMLQualifiedName *>(&tag_storage) + tag_i;
            QualifiedName::CreateStatic(address, impl, ns_uri);
            ++tag_i;
        }

        if (!kNames[i].isAttr)
            continue;

        void *address = reinterpret_cast<QualifiedName *>(&attr_storage) + attr_i;
        QualifiedName::CreateStatic(address, impl);
        ++attr_i;
    }

    ASSERT(kTagsCount == tag_i);
    ASSERT(kAttrsCount == attr_i);
}

} // namespace html_names
} // namespace blink

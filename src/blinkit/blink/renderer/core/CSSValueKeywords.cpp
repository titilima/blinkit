/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf --key-positions='*' -P -n -m 50 -D  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "CSSValueKeywords.h"
#include "core/css/HashTools.h"
#include <string.h>

#ifdef _MSC_VER
// Disable the warnings from casting a 64-bit pointer to 32-bit long
// warning C4302: 'type cast': truncation from 'char (*)[28]' to 'long'
// warning C4311: 'type cast': pointer truncation from 'char (*)[18]' to 'long'
#pragma warning(disable : 4302 4311)
#endif

namespace blink {
static const char valueListStringPool[] = {
    "inherit\0"
    "initial\0"
    "none\0"
    "hidden\0"
    "inset\0"
    "groove\0"
    "outset\0"
    "ridge\0"
    "dotted\0"
    "dashed\0"
    "solid\0"
    "double\0"
    "caption\0"
    "icon\0"
    "menu\0"
    "message-box\0"
    "small-caption\0"
    "-webkit-mini-control\0"
    "-webkit-small-control\0"
    "-webkit-control\0"
    "status-bar\0"
    "italic\0"
    "oblique\0"
    "all\0"
    "small-caps\0"
    "common-ligatures\0"
    "no-common-ligatures\0"
    "discretionary-ligatures\0"
    "no-discretionary-ligatures\0"
    "historical-ligatures\0"
    "no-historical-ligatures\0"
    "contextual\0"
    "no-contextual\0"
    "normal\0"
    "bold\0"
    "bolder\0"
    "lighter\0"
    "100\0"
    "200\0"
    "300\0"
    "400\0"
    "500\0"
    "600\0"
    "700\0"
    "800\0"
    "900\0"
    "ultra-condensed\0"
    "extra-condensed\0"
    "condensed\0"
    "semi-condensed\0"
    "semi-expanded\0"
    "expanded\0"
    "extra-expanded\0"
    "ultra-expanded\0"
    "xx-small\0"
    "x-small\0"
    "small\0"
    "medium\0"
    "large\0"
    "x-large\0"
    "xx-large\0"
    "-webkit-xxx-large\0"
    "smaller\0"
    "larger\0"
    "serif\0"
    "sans-serif\0"
    "cursive\0"
    "fantasy\0"
    "monospace\0"
    "-webkit-body\0"
    "-webkit-pictograph\0"
    "swap\0"
    "fallback\0"
    "optional\0"
    "aqua\0"
    "black\0"
    "blue\0"
    "fuchsia\0"
    "gray\0"
    "green\0"
    "lime\0"
    "maroon\0"
    "navy\0"
    "olive\0"
    "orange\0"
    "purple\0"
    "red\0"
    "silver\0"
    "teal\0"
    "white\0"
    "yellow\0"
    "transparent\0"
    "-webkit-link\0"
    "-webkit-activelink\0"
    "activeborder\0"
    "activecaption\0"
    "appworkspace\0"
    "background\0"
    "buttonface\0"
    "buttonhighlight\0"
    "buttonshadow\0"
    "buttontext\0"
    "captiontext\0"
    "graytext\0"
    "highlight\0"
    "highlighttext\0"
    "inactiveborder\0"
    "inactivecaption\0"
    "inactivecaptiontext\0"
    "infobackground\0"
    "infotext\0"
    "menutext\0"
    "scrollbar\0"
    "threeddarkshadow\0"
    "threedface\0"
    "threedhighlight\0"
    "threedlightshadow\0"
    "threedshadow\0"
    "window\0"
    "windowframe\0"
    "windowtext\0"
    "-internal-active-list-box-selection\0"
    "-internal-active-list-box-selection-text\0"
    "-internal-inactive-list-box-selection\0"
    "-internal-inactive-list-box-selection-text\0"
    "-webkit-focus-ring-color\0"
    "currentcolor\0"
    "grey\0"
    "-webkit-text\0"
    "repeat\0"
    "repeat-x\0"
    "repeat-y\0"
    "no-repeat\0"
    "clear\0"
    "copy\0"
    "source-over\0"
    "source-in\0"
    "source-out\0"
    "source-atop\0"
    "destination-over\0"
    "destination-in\0"
    "destination-out\0"
    "destination-atop\0"
    "xor\0"
    "plus-lighter\0"
    "baseline\0"
    "middle\0"
    "sub\0"
    "super\0"
    "text-top\0"
    "text-bottom\0"
    "top\0"
    "bottom\0"
    "-webkit-baseline-middle\0"
    "-webkit-auto\0"
    "left\0"
    "right\0"
    "center\0"
    "justify\0"
    "-webkit-left\0"
    "-webkit-right\0"
    "-webkit-center\0"
    "-webkit-match-parent\0"
    "inter-word\0"
    "distribute\0"
    "outside\0"
    "inside\0"
    "disc\0"
    "circle\0"
    "square\0"
    "decimal\0"
    "decimal-leading-zero\0"
    "arabic-indic\0"
    "bengali\0"
    "cambodian\0"
    "khmer\0"
    "devanagari\0"
    "gujarati\0"
    "gurmukhi\0"
    "kannada\0"
    "lao\0"
    "malayalam\0"
    "mongolian\0"
    "myanmar\0"
    "oriya\0"
    "persian\0"
    "urdu\0"
    "telugu\0"
    "tibetan\0"
    "thai\0"
    "lower-roman\0"
    "upper-roman\0"
    "lower-greek\0"
    "lower-alpha\0"
    "lower-latin\0"
    "upper-alpha\0"
    "upper-latin\0"
    "cjk-earthly-branch\0"
    "cjk-heavenly-stem\0"
    "ethiopic-halehame\0"
    "ethiopic-halehame-am\0"
    "ethiopic-halehame-ti-er\0"
    "ethiopic-halehame-ti-et\0"
    "hangul\0"
    "hangul-consonant\0"
    "korean-hangul-formal\0"
    "korean-hanja-formal\0"
    "korean-hanja-informal\0"
    "hebrew\0"
    "armenian\0"
    "lower-armenian\0"
    "upper-armenian\0"
    "georgian\0"
    "cjk-ideographic\0"
    "simp-chinese-formal\0"
    "simp-chinese-informal\0"
    "trad-chinese-formal\0"
    "trad-chinese-informal\0"
    "hiragana\0"
    "katakana\0"
    "hiragana-iroha\0"
    "katakana-iroha\0"
    "inline\0"
    "block\0"
    "list-item\0"
    "inline-block\0"
    "table\0"
    "inline-table\0"
    "table-row-group\0"
    "table-header-group\0"
    "table-footer-group\0"
    "table-row\0"
    "table-column-group\0"
    "table-column\0"
    "table-cell\0"
    "table-caption\0"
    "-webkit-box\0"
    "-webkit-inline-box\0"
    "flex\0"
    "inline-flex\0"
    "grid\0"
    "inline-grid\0"
    "-webkit-flex\0"
    "-webkit-inline-flex\0"
    "auto\0"
    "crosshair\0"
    "default\0"
    "pointer\0"
    "move\0"
    "vertical-text\0"
    "cell\0"
    "context-menu\0"
    "alias\0"
    "progress\0"
    "no-drop\0"
    "not-allowed\0"
    "zoom-in\0"
    "zoom-out\0"
    "e-resize\0"
    "ne-resize\0"
    "nw-resize\0"
    "n-resize\0"
    "se-resize\0"
    "sw-resize\0"
    "s-resize\0"
    "w-resize\0"
    "ew-resize\0"
    "ns-resize\0"
    "nesw-resize\0"
    "nwse-resize\0"
    "col-resize\0"
    "row-resize\0"
    "text\0"
    "wait\0"
    "help\0"
    "all-scroll\0"
    "-webkit-grab\0"
    "-webkit-grabbing\0"
    "-webkit-zoom-in\0"
    "-webkit-zoom-out\0"
    "ltr\0"
    "rtl\0"
    "capitalize\0"
    "uppercase\0"
    "lowercase\0"
    "visible\0"
    "collapse\0"
    "a3\0"
    "a4\0"
    "a5\0"
    "above\0"
    "absolute\0"
    "always\0"
    "avoid\0"
    "b4\0"
    "b5\0"
    "below\0"
    "bidi-override\0"
    "blink\0"
    "both\0"
    "close-quote\0"
    "embed\0"
    "fixed\0"
    "hand\0"
    "hide\0"
    "isolate\0"
    "isolate-override\0"
    "plaintext\0"
    "-webkit-isolate\0"
    "-webkit-isolate-override\0"
    "-webkit-plaintext\0"
    "landscape\0"
    "ledger\0"
    "legal\0"
    "letter\0"
    "line-through\0"
    "local\0"
    "no-close-quote\0"
    "no-open-quote\0"
    "nowrap\0"
    "open-quote\0"
    "overlay\0"
    "overline\0"
    "portrait\0"
    "pre\0"
    "pre-line\0"
    "pre-wrap\0"
    "relative\0"
    "scroll\0"
    "separate\0"
    "show\0"
    "static\0"
    "thick\0"
    "thin\0"
    "underline\0"
    "wavy\0"
    "-webkit-nowrap\0"
    "stretch\0"
    "start\0"
    "end\0"
    "clone\0"
    "slice\0"
    "reverse\0"
    "horizontal\0"
    "vertical\0"
    "inline-axis\0"
    "block-axis\0"
    "single\0"
    "multiple\0"
    "flex-start\0"
    "flex-end\0"
    "space-between\0"
    "space-around\0"
    "space-evenly\0"
    "unsafe\0"
    "safe\0"
    "row\0"
    "row-reverse\0"
    "column\0"
    "column-reverse\0"
    "wrap\0"
    "wrap-reverse\0"
    "dense\0"
    "read-only\0"
    "read-write\0"
    "read-write-plaintext-only\0"
    "element\0"
    "-webkit-min-content\0"
    "-webkit-max-content\0"
    "-webkit-fill-available\0"
    "-webkit-fit-content\0"
    "min-content\0"
    "max-content\0"
    "fit-content\0"
    "clip\0"
    "ellipsis\0"
    "discard\0"
    "break-all\0"
    "keep-all\0"
    "break-word\0"
    "space\0"
    "loose\0"
    "strict\0"
    "after-white-space\0"
    "checkbox\0"
    "radio\0"
    "push-button\0"
    "square-button\0"
    "button\0"
    "button-bevel\0"
    "inner-spin-button\0"
    "listbox\0"
    "listitem\0"
    "media-enter-fullscreen-button\0"
    "media-exit-fullscreen-button\0"
    "media-fullscreen-volume-slider\0"
    "media-fullscreen-volume-slider-thumb\0"
    "media-mute-button\0"
    "media-play-button\0"
    "media-overlay-play-button\0"
    "media-toggle-closed-captions-button\0"
    "media-slider\0"
    "media-sliderthumb\0"
    "media-volume-slider-container\0"
    "media-volume-slider\0"
    "media-volume-sliderthumb\0"
    "media-controls-background\0"
    "media-controls-fullscreen-background\0"
    "media-current-time-display\0"
    "media-time-remaining-display\0"
    "-internal-media-cast-off-button\0"
    "-internal-media-overlay-cast-off-button\0"
    "menulist\0"
    "menulist-button\0"
    "menulist-text\0"
    "menulist-textfield\0"
    "meter\0"
    "progress-bar\0"
    "progress-bar-value\0"
    "slider-horizontal\0"
    "slider-vertical\0"
    "sliderthumb-horizontal\0"
    "sliderthumb-vertical\0"
    "caret\0"
    "searchfield\0"
    "searchfield-decoration\0"
    "searchfield-results-decoration\0"
    "searchfield-cancel-button\0"
    "textfield\0"
    "relevancy-level-indicator\0"
    "continuous-capacity-level-indicator\0"
    "discrete-capacity-level-indicator\0"
    "rating-level-indicator\0"
    "textarea\0"
    "caps-lock-indicator\0"
    "round\0"
    "border\0"
    "border-box\0"
    "content\0"
    "content-box\0"
    "padding\0"
    "padding-box\0"
    "margin-box\0"
    "contain\0"
    "cover\0"
    "logical\0"
    "visual\0"
    "alternate\0"
    "alternate-reverse\0"
    "forwards\0"
    "backwards\0"
    "infinite\0"
    "running\0"
    "paused\0"
    "flat\0"
    "preserve-3d\0"
    "ease\0"
    "linear\0"
    "ease-in\0"
    "ease-out\0"
    "ease-in-out\0"
    "step-start\0"
    "step-middle\0"
    "step-end\0"
    "steps\0"
    "cubic-bezier\0"
    "document\0"
    "reset\0"
    "zoom\0"
    "visiblePainted\0"
    "visibleFill\0"
    "visibleStroke\0"
    "painted\0"
    "fill\0"
    "stroke\0"
    "bounding-box\0"
    "spell-out\0"
    "digits\0"
    "literal-punctuation\0"
    "no-punctuation\0"
    "antialiased\0"
    "subpixel-antialiased\0"
    "optimizeSpeed\0"
    "optimizeLegibility\0"
    "geometricPrecision\0"
    "economy\0"
    "exact\0"
    "lr\0"
    "rl\0"
    "tb\0"
    "lr-tb\0"
    "rl-tb\0"
    "tb-rl\0"
    "horizontal-tb\0"
    "vertical-rl\0"
    "vertical-lr\0"
    "after\0"
    "before\0"
    "over\0"
    "under\0"
    "filled\0"
    "open\0"
    "dot\0"
    "double-circle\0"
    "triangle\0"
    "sesame\0"
    "ellipse\0"
    "closest-side\0"
    "closest-corner\0"
    "farthest-side\0"
    "farthest-corner\0"
    "mixed\0"
    "sideways\0"
    "sideways-right\0"
    "upright\0"
    "vertical-right\0"
    "on\0"
    "off\0"
    "optimizeQuality\0"
    "pixelated\0"
    "-webkit-optimize-contrast\0"
    "nonzero\0"
    "evenodd\0"
    "at\0"
    "alphabetic\0"
    "fullscreen\0"
    "standalone\0"
    "minimal-ui\0"
    "browser\0"
    "sticky\0"
    "coarse\0"
    "fine\0"
    "on-demand\0"
    "hover\0"
    "multiply\0"
    "screen\0"
    "darken\0"
    "lighten\0"
    "color-dodge\0"
    "color-burn\0"
    "hard-light\0"
    "soft-light\0"
    "difference\0"
    "exclusion\0"
    "hue\0"
    "saturation\0"
    "color\0"
    "luminosity\0"
    "scale-down\0"
    "balance\0"
    "-webkit-paged-x\0"
    "-webkit-paged-y\0"
    "drag\0"
    "no-drag\0"
    "span\0"
    "minmax\0"
    "each-line\0"
    "progressive\0"
    "interlace\0"
    "markers\0"
    "-internal-extend-to-zoom\0"
    "pan-x\0"
    "pan-y\0"
    "pan-left\0"
    "pan-right\0"
    "pan-up\0"
    "pan-down\0"
    "manipulation\0"
    "last-baseline\0"
    "self-start\0"
    "self-end\0"
    "legacy\0"
    "smooth\0"
    "contents\0"
    "scroll-position\0"
    "revert\0"
    "unset\0"
    "linear-gradient\0"
    "radial-gradient\0"
    "repeating-linear-gradient\0"
    "repeating-radial-gradient\0"
    "-webkit-cross-fade\0"
    "-webkit-gradient\0"
    "-webkit-linear-gradient\0"
    "-webkit-radial-gradient\0"
    "-webkit-repeating-linear-gradient\0"
    "-webkit-repeating-radial-gradient\0"
    "-webkit-image-set\0"
    "from\0"
    "to\0"
    "color-stop\0"
    "radial\0"
    "attr\0"
    "counter\0"
    "counters\0"
    "rect\0"
    "polygon\0"
    "format\0"
    "invert\0"
    "grayscale\0"
    "sepia\0"
    "saturate\0"
    "hue-rotate\0"
    "opacity\0"
    "brightness\0"
    "contrast\0"
    "blur\0"
    "drop-shadow\0"
    "url\0"
    "rgb\0"
    "rgba\0"
    "hsl\0"
    "hsla\0"
    "matrix\0"
    "matrix3d\0"
    "perspective\0"
    "rotate\0"
    "rotateX\0"
    "rotateY\0"
    "rotateZ\0"
    "rotate3d\0"
    "scale\0"
    "scaleX\0"
    "scaleY\0"
    "scaleZ\0"
    "scale3d\0"
    "skew\0"
    "skewX\0"
    "skewY\0"
    "translate\0"
    "translateX\0"
    "translateY\0"
    "translateZ\0"
    "translate3d\0"
    "path\0"
    "calc\0"
    "-webkit-calc\0"
    "mandatory\0"
    "proximity\0"
    "from-image\0"
    "paint\0"
    "style\0"
    "layout\0"
    "var\0"
    "-internal-variable-value\0"
    "aliceblue\0"
    "antiquewhite\0"
    "aquamarine\0"
    "azure\0"
    "beige\0"
    "bisque\0"
    "blanchedalmond\0"
    "blueviolet\0"
    "brown\0"
    "burlywood\0"
    "cadetblue\0"
    "chartreuse\0"
    "chocolate\0"
    "coral\0"
    "cornflowerblue\0"
    "cornsilk\0"
    "crimson\0"
    "cyan\0"
    "darkblue\0"
    "darkcyan\0"
    "darkgoldenrod\0"
    "darkgray\0"
    "darkgreen\0"
    "darkgrey\0"
    "darkkhaki\0"
    "darkmagenta\0"
    "darkolivegreen\0"
    "darkorange\0"
    "darkorchid\0"
    "darkred\0"
    "darksalmon\0"
    "darkseagreen\0"
    "darkslateblue\0"
    "darkslategray\0"
    "darkslategrey\0"
    "darkturquoise\0"
    "darkviolet\0"
    "deeppink\0"
    "deepskyblue\0"
    "dimgray\0"
    "dimgrey\0"
    "dodgerblue\0"
    "firebrick\0"
    "floralwhite\0"
    "forestgreen\0"
    "gainsboro\0"
    "ghostwhite\0"
    "gold\0"
    "goldenrod\0"
    "greenyellow\0"
    "honeydew\0"
    "hotpink\0"
    "indianred\0"
    "indigo\0"
    "ivory\0"
    "khaki\0"
    "lavender\0"
    "lavenderblush\0"
    "lawngreen\0"
    "lemonchiffon\0"
    "lightblue\0"
    "lightcoral\0"
    "lightcyan\0"
    "lightgoldenrodyellow\0"
    "lightgray\0"
    "lightgreen\0"
    "lightgrey\0"
    "lightpink\0"
    "lightsalmon\0"
    "lightseagreen\0"
    "lightskyblue\0"
    "lightslategray\0"
    "lightslategrey\0"
    "lightsteelblue\0"
    "lightyellow\0"
    "limegreen\0"
    "linen\0"
    "magenta\0"
    "mediumaquamarine\0"
    "mediumblue\0"
    "mediumorchid\0"
    "mediumpurple\0"
    "mediumseagreen\0"
    "mediumslateblue\0"
    "mediumspringgreen\0"
    "mediumturquoise\0"
    "mediumvioletred\0"
    "midnightblue\0"
    "mintcream\0"
    "mistyrose\0"
    "moccasin\0"
    "navajowhite\0"
    "oldlace\0"
    "olivedrab\0"
    "orangered\0"
    "orchid\0"
    "palegoldenrod\0"
    "palegreen\0"
    "paleturquoise\0"
    "palevioletred\0"
    "papayawhip\0"
    "peachpuff\0"
    "peru\0"
    "pink\0"
    "plum\0"
    "powderblue\0"
    "rebeccapurple\0"
    "rosybrown\0"
    "royalblue\0"
    "saddlebrown\0"
    "salmon\0"
    "sandybrown\0"
    "seagreen\0"
    "seashell\0"
    "sienna\0"
    "skyblue\0"
    "slateblue\0"
    "slategray\0"
    "slategrey\0"
    "snow\0"
    "springgreen\0"
    "steelblue\0"
    "tan\0"
    "thistle\0"
    "tomato\0"
    "turquoise\0"
    "violet\0"
    "wheat\0"
    "whitesmoke\0"
    "yellowgreen\0"
    "alpha\0"
    "luminance\0"
    "accumulate\0"
    "new\0"
    "sRGB\0"
    "linearRGB\0"
    "crispEdges\0"
    "butt\0"
    "miter\0"
    "bevel\0"
    "before-edge\0"
    "after-edge\0"
    "central\0"
    "text-before-edge\0"
    "text-after-edge\0"
    "ideographic\0"
    "hanging\0"
    "mathematical\0"
    "use-script\0"
    "no-change\0"
    "reset-size\0"
    "dynamic\0"
    "non-scaling-stroke\0"
};

static const unsigned short valueListStringOffsets[] = {
  0,
  8,
  16,
  21,
  28,
  34,
  41,
  48,
  54,
  61,
  68,
  74,
  81,
  89,
  94,
  99,
  111,
  125,
  146,
  168,
  184,
  195,
  202,
  210,
  214,
  225,
  242,
  262,
  286,
  313,
  334,
  358,
  369,
  383,
  390,
  395,
  402,
  410,
  414,
  418,
  422,
  426,
  430,
  434,
  438,
  442,
  446,
  462,
  478,
  488,
  503,
  517,
  526,
  541,
  556,
  565,
  573,
  579,
  586,
  592,
  600,
  609,
  627,
  635,
  642,
  648,
  659,
  667,
  675,
  685,
  698,
  717,
  722,
  731,
  740,
  745,
  751,
  756,
  764,
  769,
  775,
  780,
  787,
  792,
  798,
  805,
  812,
  816,
  823,
  828,
  834,
  841,
  853,
  866,
  885,
  898,
  912,
  925,
  936,
  947,
  963,
  976,
  987,
  999,
  1008,
  1018,
  1032,
  1047,
  1063,
  1083,
  1098,
  1107,
  1116,
  1126,
  1143,
  1154,
  1170,
  1188,
  1201,
  1208,
  1220,
  1231,
  1267,
  1308,
  1346,
  1389,
  1414,
  1427,
  1432,
  1445,
  1452,
  1461,
  1470,
  1480,
  1486,
  1491,
  1503,
  1513,
  1524,
  1536,
  1553,
  1568,
  1584,
  1601,
  1605,
  1618,
  1627,
  1634,
  1638,
  1644,
  1653,
  1665,
  1669,
  1676,
  1700,
  1713,
  1718,
  1724,
  1731,
  1739,
  1752,
  1766,
  1781,
  1802,
  1813,
  1824,
  1832,
  1839,
  1844,
  1851,
  1858,
  1866,
  1887,
  1900,
  1908,
  1918,
  1924,
  1935,
  1944,
  1953,
  1961,
  1965,
  1975,
  1985,
  1993,
  1999,
  2007,
  2012,
  2019,
  2027,
  2032,
  2044,
  2056,
  2068,
  2080,
  2092,
  2104,
  2116,
  2135,
  2153,
  2171,
  2192,
  2216,
  2240,
  2247,
  2264,
  2285,
  2305,
  2327,
  2334,
  2343,
  2358,
  2373,
  2382,
  2398,
  2418,
  2440,
  2460,
  2482,
  2491,
  2500,
  2515,
  2530,
  2537,
  2543,
  2553,
  2566,
  2572,
  2585,
  2601,
  2620,
  2639,
  2649,
  2668,
  2681,
  2692,
  2706,
  2718,
  2737,
  2742,
  2754,
  2759,
  2771,
  2784,
  2804,
  2809,
  2819,
  2827,
  2835,
  2840,
  2854,
  2859,
  2872,
  2878,
  2887,
  2895,
  2907,
  2915,
  2924,
  2933,
  2943,
  2953,
  2962,
  2972,
  2982,
  2991,
  3000,
  3010,
  3020,
  3032,
  3044,
  3055,
  3066,
  3071,
  3076,
  3081,
  3092,
  3105,
  3122,
  3138,
  3155,
  3159,
  3163,
  3174,
  3184,
  3194,
  3202,
  3211,
  3214,
  3217,
  3220,
  3226,
  3235,
  3242,
  3248,
  3251,
  3254,
  3260,
  3274,
  3280,
  3285,
  3297,
  3303,
  3309,
  3314,
  3319,
  3327,
  3344,
  3354,
  3370,
  3395,
  3413,
  3423,
  3430,
  3436,
  3443,
  3456,
  3462,
  3477,
  3491,
  3498,
  3509,
  3517,
  3526,
  3535,
  3539,
  3548,
  3557,
  3566,
  3573,
  3582,
  3587,
  3594,
  3600,
  3605,
  3615,
  3620,
  3635,
  3643,
  3649,
  3653,
  3659,
  3665,
  3673,
  3684,
  3693,
  3705,
  3716,
  3723,
  3732,
  3743,
  3752,
  3766,
  3779,
  3792,
  3799,
  3804,
  3808,
  3820,
  3827,
  3842,
  3847,
  3860,
  3866,
  3876,
  3887,
  3913,
  3921,
  3941,
  3961,
  3984,
  4004,
  4016,
  4028,
  4040,
  4045,
  4054,
  4062,
  4072,
  4081,
  4092,
  4098,
  4104,
  4111,
  4129,
  4138,
  4144,
  4156,
  4170,
  4177,
  4190,
  4208,
  4216,
  4225,
  4255,
  4284,
  4315,
  4352,
  4370,
  4388,
  4414,
  4450,
  4463,
  4481,
  4511,
  4531,
  4556,
  4582,
  4619,
  4646,
  4675,
  4707,
  4747,
  4756,
  4772,
  4786,
  4805,
  4811,
  4824,
  4843,
  4861,
  4877,
  4900,
  4921,
  4927,
  4939,
  4962,
  4993,
  5019,
  5029,
  5055,
  5091,
  5125,
  5148,
  5157,
  5177,
  5183,
  5190,
  5201,
  5209,
  5221,
  5229,
  5241,
  5252,
  5260,
  5266,
  5274,
  5281,
  5291,
  5309,
  5318,
  5328,
  5337,
  5345,
  5352,
  5357,
  5369,
  5374,
  5381,
  5389,
  5398,
  5410,
  5421,
  5433,
  5442,
  5448,
  5461,
  5470,
  5476,
  5481,
  5496,
  5508,
  5522,
  5530,
  5535,
  5542,
  5555,
  5565,
  5572,
  5592,
  5607,
  5619,
  5640,
  5654,
  5673,
  5692,
  5700,
  5706,
  5709,
  5712,
  5715,
  5721,
  5727,
  5733,
  5747,
  5759,
  5771,
  5777,
  5784,
  5789,
  5795,
  5802,
  5807,
  5811,
  5825,
  5834,
  5841,
  5849,
  5862,
  5877,
  5891,
  5907,
  5913,
  5922,
  5937,
  5945,
  5960,
  5963,
  5967,
  5983,
  5993,
  6019,
  6027,
  6035,
  6038,
  6049,
  6060,
  6071,
  6082,
  6090,
  6097,
  6104,
  6109,
  6119,
  6125,
  6134,
  6141,
  6148,
  6156,
  6168,
  6179,
  6190,
  6201,
  6212,
  6222,
  6226,
  6237,
  6243,
  6254,
  6265,
  6273,
  6289,
  6305,
  6310,
  6318,
  6323,
  6330,
  6340,
  6352,
  6362,
  6370,
  6395,
  6401,
  6407,
  6416,
  6426,
  6433,
  6442,
  6455,
  6469,
  6480,
  6489,
  6496,
  6503,
  6512,
  6528,
  6535,
  6541,
  6557,
  6573,
  6599,
  6625,
  6644,
  6661,
  6685,
  6709,
  6743,
  6777,
  6795,
  6800,
  6803,
  6814,
  6821,
  6826,
  6834,
  6843,
  6848,
  6856,
  6863,
  6870,
  6880,
  6886,
  6895,
  6906,
  6914,
  6925,
  6934,
  6939,
  6951,
  6955,
  6959,
  6964,
  6968,
  6973,
  6980,
  6989,
  7001,
  7008,
  7016,
  7024,
  7032,
  7041,
  7047,
  7054,
  7061,
  7068,
  7076,
  7081,
  7087,
  7093,
  7103,
  7114,
  7125,
  7136,
  7148,
  7153,
  7158,
  7171,
  7181,
  7191,
  7202,
  7208,
  7214,
  7221,
  7225,
  7250,
  7260,
  7273,
  7284,
  7290,
  7296,
  7303,
  7318,
  7329,
  7335,
  7345,
  7355,
  7366,
  7376,
  7382,
  7397,
  7406,
  7414,
  7419,
  7428,
  7437,
  7451,
  7460,
  7470,
  7479,
  7489,
  7501,
  7516,
  7527,
  7538,
  7546,
  7557,
  7570,
  7584,
  7598,
  7612,
  7626,
  7637,
  7646,
  7658,
  7666,
  7674,
  7685,
  7695,
  7707,
  7719,
  7729,
  7740,
  7745,
  7755,
  7767,
  7776,
  7784,
  7794,
  7801,
  7807,
  7813,
  7822,
  7836,
  7846,
  7859,
  7869,
  7880,
  7890,
  7911,
  7921,
  7932,
  7942,
  7952,
  7964,
  7978,
  7991,
  8006,
  8021,
  8036,
  8048,
  8058,
  8064,
  8072,
  8089,
  8100,
  8113,
  8126,
  8141,
  8157,
  8175,
  8191,
  8207,
  8220,
  8230,
  8240,
  8249,
  8261,
  8269,
  8279,
  8289,
  8296,
  8310,
  8320,
  8334,
  8348,
  8359,
  8369,
  8374,
  8379,
  8384,
  8395,
  8409,
  8419,
  8429,
  8441,
  8448,
  8459,
  8468,
  8477,
  8484,
  8492,
  8502,
  8512,
  8522,
  8527,
  8539,
  8549,
  8553,
  8561,
  8568,
  8578,
  8585,
  8591,
  8602,
  8614,
  8620,
  8630,
  8641,
  8645,
  8650,
  8660,
  8671,
  8676,
  8682,
  8688,
  8700,
  8711,
  8719,
  8736,
  8752,
  8764,
  8772,
  8785,
  8796,
  8806,
  8817,
  8825,
};

/* maximum key range = 5861, duplicates = 0 */

class CSSValueKeywordsHash
{
private:
  static inline unsigned int value_hash_function (const char *str, unsigned int len);
public:
  static const struct Value *findValueImpl (const char *str, unsigned int len);
};

inline unsigned int
CSSValueKeywordsHash::value_hash_function (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894,   93,   49,   56,   14, 5894,
        21,   20,   23,   16,   19,   17,   15,   14,   13, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894,   14,  337,  122,
        47,   13,  580,   18,  580,   14, 1144,  361,   53,   16,
        15,   24,  274, 1070,   17,  102,   13,  208,   94, 1447,
      1382,  871,  683,   46,   13, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894,
      5894, 5894, 5894, 5894, 5894, 5894, 5894, 5894
    };
  register unsigned int hval = 0;

  switch (len)
    {
      default:
        hval += asso_values[(unsigned char)str[41]];
      /*FALLTHROUGH*/
      case 41:
        hval += asso_values[(unsigned char)str[40]];
      /*FALLTHROUGH*/
      case 40:
        hval += asso_values[(unsigned char)str[39]];
      /*FALLTHROUGH*/
      case 39:
        hval += asso_values[(unsigned char)str[38]];
      /*FALLTHROUGH*/
      case 38:
        hval += asso_values[(unsigned char)str[37]];
      /*FALLTHROUGH*/
      case 37:
        hval += asso_values[(unsigned char)str[36]];
      /*FALLTHROUGH*/
      case 36:
        hval += asso_values[(unsigned char)str[35]];
      /*FALLTHROUGH*/
      case 35:
        hval += asso_values[(unsigned char)str[34]];
      /*FALLTHROUGH*/
      case 34:
        hval += asso_values[(unsigned char)str[33]];
      /*FALLTHROUGH*/
      case 33:
        hval += asso_values[(unsigned char)str[32]];
      /*FALLTHROUGH*/
      case 32:
        hval += asso_values[(unsigned char)str[31]];
      /*FALLTHROUGH*/
      case 31:
        hval += asso_values[(unsigned char)str[30]];
      /*FALLTHROUGH*/
      case 30:
        hval += asso_values[(unsigned char)str[29]];
      /*FALLTHROUGH*/
      case 29:
        hval += asso_values[(unsigned char)str[28]];
      /*FALLTHROUGH*/
      case 28:
        hval += asso_values[(unsigned char)str[27]];
      /*FALLTHROUGH*/
      case 27:
        hval += asso_values[(unsigned char)str[26]];
      /*FALLTHROUGH*/
      case 26:
        hval += asso_values[(unsigned char)str[25]];
      /*FALLTHROUGH*/
      case 25:
        hval += asso_values[(unsigned char)str[24]];
      /*FALLTHROUGH*/
      case 24:
        hval += asso_values[(unsigned char)str[23]];
      /*FALLTHROUGH*/
      case 23:
        hval += asso_values[(unsigned char)str[22]];
      /*FALLTHROUGH*/
      case 22:
        hval += asso_values[(unsigned char)str[21]];
      /*FALLTHROUGH*/
      case 21:
        hval += asso_values[(unsigned char)str[20]];
      /*FALLTHROUGH*/
      case 20:
        hval += asso_values[(unsigned char)str[19]];
      /*FALLTHROUGH*/
      case 19:
        hval += asso_values[(unsigned char)str[18]];
      /*FALLTHROUGH*/
      case 18:
        hval += asso_values[(unsigned char)str[17]];
      /*FALLTHROUGH*/
      case 17:
        hval += asso_values[(unsigned char)str[16]];
      /*FALLTHROUGH*/
      case 16:
        hval += asso_values[(unsigned char)str[15]];
      /*FALLTHROUGH*/
      case 15:
        hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
      case 14:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
        hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
      case 11:
        hval += asso_values[(unsigned char)str[10]+1];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]+2];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]+1];
        break;
    }
  return hval;
}

struct stringpool_t
  {
    char stringpool_str0[sizeof("lr")];
    char stringpool_str1[sizeof("900")];
    char stringpool_str2[sizeof("800")];
    char stringpool_str3[sizeof("700")];
    char stringpool_str4[sizeof("400")];
    char stringpool_str5[sizeof("600")];
    char stringpool_str6[sizeof("ltr")];
    char stringpool_str7[sizeof("500")];
    char stringpool_str8[sizeof("200")];
    char stringpool_str9[sizeof("100")];
    char stringpool_str10[sizeof("dot")];
    char stringpool_str11[sizeof("300")];
    char stringpool_str12[sizeof("lao")];
    char stringpool_str13[sizeof("drag")];
    char stringpool_str14[sizeof("lime")];
    char stringpool_str15[sizeof("fine")];
    char stringpool_str16[sizeof("start")];
    char stringpool_str17[sizeof("large")];
    char stringpool_str18[sizeof("meter")];
    char stringpool_str19[sizeof("miter")];
    char stringpool_str20[sizeof("linen")];
    char stringpool_str21[sizeof("none")];
    char stringpool_str22[sizeof("from")];
    char stringpool_str23[sizeof("darken")];
    char stringpool_str24[sizeof("larger")];
    char stringpool_str25[sizeof("hide")];
    char stringpool_str26[sizeof("linear")];
    char stringpool_str27[sizeof("srgb")];
    char stringpool_str28[sizeof("help")];
    char stringpool_str29[sizeof("fill")];
    char stringpool_str30[sizeof("hanging")];
    char stringpool_str31[sizeof("slice")];
    char stringpool_str32[sizeof("magenta")];
    char stringpool_str33[sizeof("caret")];
    char stringpool_str34[sizeof("format")];
    char stringpool_str35[sizeof("small")];
    char stringpool_str36[sizeof("seagreen")];
    char stringpool_str37[sizeof("zoom")];
    char stringpool_str38[sizeof("ledger")];
    char stringpool_str39[sizeof("single")];
    char stringpool_str40[sizeof("calc")];
    char stringpool_str41[sizeof("cell")];
    char stringpool_str42[sizeof("nonzero")];
    char stringpool_str43[sizeof("clip")];
    char stringpool_str44[sizeof("darkgreen")];
    char stringpool_str45[sizeof("darkred")];
    char stringpool_str46[sizeof("b5")];
    char stringpool_str47[sizeof("limegreen")];
    char stringpool_str48[sizeof("smaller")];
    char stringpool_str49[sizeof("disc")];
    char stringpool_str50[sizeof("salmon")];
    char stringpool_str51[sizeof("b4")];
    char stringpool_str52[sizeof("move")];
    char stringpool_str53[sizeof("rl")];
    char stringpool_str54[sizeof("normal")];
    char stringpool_str55[sizeof("circle")];
    char stringpool_str56[sizeof("steps")];
    char stringpool_str57[sizeof("filled")];
    char stringpool_str58[sizeof("darkorange")];
    char stringpool_str59[sizeof("red")];
    char stringpool_str60[sizeof("url")];
    char stringpool_str61[sizeof("letter")];
    char stringpool_str62[sizeof("hover")];
    char stringpool_str63[sizeof("rtl")];
    char stringpool_str64[sizeof("hsl")];
    char stringpool_str65[sizeof("beige")];
    char stringpool_str66[sizeof("scale")];
    char stringpool_str67[sizeof("navy")];
    char stringpool_str68[sizeof("flat")];
    char stringpool_str69[sizeof("mongolian")];
    char stringpool_str70[sizeof("ridge")];
    char stringpool_str71[sizeof("under")];
    char stringpool_str72[sizeof("markers")];
    char stringpool_str73[sizeof("screen")];
    char stringpool_str74[sizeof("center")];
    char stringpool_str75[sizeof("cover")];
    char stringpool_str76[sizeof("dotted")];
    char stringpool_str77[sizeof("coarse")];
    char stringpool_str78[sizeof("discard")];
    char stringpool_str79[sizeof("content")];
    char stringpool_str80[sizeof("legal")];
    char stringpool_str81[sizeof("contain")];
    char stringpool_str82[sizeof("step-end")];
    char stringpool_str83[sizeof("hiragana")];
    char stringpool_str84[sizeof("to")];
    char stringpool_str85[sizeof("lavender")];
    char stringpool_str86[sizeof("hue")];
    char stringpool_str87[sizeof("tan")];
    char stringpool_str88[sizeof("cambodian")];
    char stringpool_str89[sizeof("zoom-in")];
    char stringpool_str90[sizeof("unset")];
    char stringpool_str91[sizeof("crimson")];
    char stringpool_str92[sizeof("scale3d")];
    char stringpool_str93[sizeof("bengali")];
    char stringpool_str94[sizeof("reset")];
    char stringpool_str95[sizeof("teal")];
    char stringpool_str96[sizeof("from-image")];
    char stringpool_str97[sizeof("clear")];
    char stringpool_str98[sizeof("central")];
    char stringpool_str99[sizeof("revert")];
    char stringpool_str100[sizeof("dense")];
    char stringpool_str101[sizeof("coral")];
    char stringpool_str102[sizeof("static")];
    char stringpool_str103[sizeof("darkviolet")];
    char stringpool_str104[sizeof("forestgreen")];
    char stringpool_str105[sizeof("sesame")];
    char stringpool_str106[sizeof("listitem")];
    char stringpool_str107[sizeof("darksalmon")];
    char stringpool_str108[sizeof("loose")];
    char stringpool_str109[sizeof("underline")];
    char stringpool_str110[sizeof("rotate")];
    char stringpool_str111[sizeof("on")];
    char stringpool_str112[sizeof("hsla")];
    char stringpool_str113[sizeof("bottom")];
    char stringpool_str114[sizeof("bevel")];
    char stringpool_str115[sizeof("linear-gradient")];
    char stringpool_str116[sizeof("step-start")];
    char stringpool_str117[sizeof("keep-all")];
    char stringpool_str118[sizeof("hangul")];
    char stringpool_str119[sizeof("moccasin")];
    char stringpool_str120[sizeof("mintcream")];
    char stringpool_str121[sizeof("contents")];
    char stringpool_str122[sizeof("contrast")];
    char stringpool_str123[sizeof("space")];
    char stringpool_str124[sizeof("rect")];
    char stringpool_str125[sizeof("devanagari")];
    char stringpool_str126[sizeof("local")];
    char stringpool_str127[sizeof("sienna")];
    char stringpool_str128[sizeof("at")];
    char stringpool_str129[sizeof("baseline")];
    char stringpool_str130[sizeof("a5")];
    char stringpool_str131[sizeof("rotate3d")];
    char stringpool_str132[sizeof("a3")];
    char stringpool_str133[sizeof("double")];
    char stringpool_str134[sizeof("reverse")];
    char stringpool_str135[sizeof("a4")];
    char stringpool_str136[sizeof("oriya")];
    char stringpool_str137[sizeof("list-item")];
    char stringpool_str138[sizeof("attr")];
    char stringpool_str139[sizeof("hue-rotate")];
    char stringpool_str140[sizeof("copy")];
    char stringpool_str141[sizeof("over")];
    char stringpool_str142[sizeof("blur")];
    char stringpool_str143[sizeof("blue")];
    char stringpool_str144[sizeof("darkgoldenrod")];
    char stringpool_str145[sizeof("tomato")];
    char stringpool_str146[sizeof("clone")];
    char stringpool_str147[sizeof("darkolivegreen")];
    char stringpool_str148[sizeof("relative")];
    char stringpool_str149[sizeof("kannada")];
    char stringpool_str150[sizeof("repeat")];
    char stringpool_str151[sizeof("butt")];
    char stringpool_str152[sizeof("all")];
    char stringpool_str153[sizeof("armenian")];
    char stringpool_str154[sizeof("zoom-out")];
    char stringpool_str155[sizeof("rgb")];
    char stringpool_str156[sizeof("hotpink")];
    char stringpool_str157[sizeof("triangle")];
    char stringpool_str158[sizeof("balance")];
    char stringpool_str159[sizeof("destination-in")];
    char stringpool_str160[sizeof("linearrgb")];
    char stringpool_str161[sizeof("button")];
    char stringpool_str162[sizeof("separate")];
    char stringpool_str163[sizeof("standalone")];
    char stringpool_str164[sizeof("no-repeat")];
    char stringpool_str165[sizeof("caption")];
    char stringpool_str166[sizeof("darkmagenta")];
    char stringpool_str167[sizeof("overline")];
    char stringpool_str168[sizeof("alternate")];
    char stringpool_str169[sizeof("solid")];
    char stringpool_str170[sizeof("top")];
    char stringpool_str171[sizeof("min-content")];
    char stringpool_str172[sizeof("fit-content")];
    char stringpool_str173[sizeof("source-in")];
    char stringpool_str174[sizeof("digits")];
    char stringpool_str175[sizeof("super")];
    char stringpool_str176[sizeof("oldlace")];
    char stringpool_str177[sizeof("strict")];
    char stringpool_str178[sizeof("collapse")];
    char stringpool_str179[sizeof("tb")];
    char stringpool_str180[sizeof("radio")];
    char stringpool_str181[sizeof("lr-tb")];
    char stringpool_str182[sizeof("sub")];
    char stringpool_str183[sizeof("bisque")];
    char stringpool_str184[sizeof("black")];
    char stringpool_str185[sizeof("block")];
    char stringpool_str186[sizeof("span")];
    char stringpool_str187[sizeof("fullscreen")];
    char stringpool_str188[sizeof("rgba")];
    char stringpool_str189[sizeof("destination-over")];
    char stringpool_str190[sizeof("table")];
    char stringpool_str191[sizeof("radial")];
    char stringpool_str192[sizeof("decimal")];
    char stringpool_str193[sizeof("counter")];
    char stringpool_str194[sizeof("cursive")];
    char stringpool_str195[sizeof("background")];
    char stringpool_str196[sizeof("katakana")];
    char stringpool_str197[sizeof("logical")];
    char stringpool_str198[sizeof("orange")];
    char stringpool_str199[sizeof("blueviolet")];
    char stringpool_str200[sizeof("tibetan")];
    char stringpool_str201[sizeof("hand")];
    char stringpool_str202[sizeof("safe")];
    char stringpool_str203[sizeof("alias")];
    char stringpool_str204[sizeof("end")];
    char stringpool_str205[sizeof("green")];
    char stringpool_str206[sizeof("running")];
    char stringpool_str207[sizeof("round")];
    char stringpool_str208[sizeof("grey")];
    char stringpool_str209[sizeof("gray")];
    char stringpool_str210[sizeof("medium")];
    char stringpool_str211[sizeof("hangul-consonant")];
    char stringpool_str212[sizeof("darkblue")];
    char stringpool_str213[sizeof("sepia")];
    char stringpool_str214[sizeof("khaki")];
    char stringpool_str215[sizeof("destination-out")];
    char stringpool_str216[sizeof("rl-tb")];
    char stringpool_str217[sizeof("hidden")];
    char stringpool_str218[sizeof("khmer")];
    char stringpool_str219[sizeof("orangered")];
    char stringpool_str220[sizeof("georgian")];
    char stringpool_str221[sizeof("closest-side")];
    char stringpool_str222[sizeof("counters")];
    char stringpool_str223[sizeof("left")];
    char stringpool_str224[sizeof("tb-rl")];
    char stringpool_str225[sizeof("spell-out")];
    char stringpool_str226[sizeof("translate")];
    char stringpool_str227[sizeof("element")];
    char stringpool_str228[sizeof("ease")];
    char stringpool_str229[sizeof("small-caps")];
    char stringpool_str230[sizeof("double-circle")];
    char stringpool_str231[sizeof("source-over")];
    char stringpool_str232[sizeof("deeppink")];
    char stringpool_str233[sizeof("steelblue")];
    char stringpool_str234[sizeof("middle")];
    char stringpool_str235[sizeof("multiple")];
    char stringpool_str236[sizeof("outset")];
    char stringpool_str237[sizeof("source-out")];
    char stringpool_str238[sizeof("dodgerblue")];
    char stringpool_str239[sizeof("translate3d")];
    char stringpool_str240[sizeof("cadetblue")];
    char stringpool_str241[sizeof("crispedges")];
    char stringpool_str242[sizeof("space-around")];
    char stringpool_str243[sizeof("no-common-ligatures")];
    char stringpool_str244[sizeof("destination-atop")];
    char stringpool_str245[sizeof("flex")];
    char stringpool_str246[sizeof("no-drag")];
    char stringpool_str247[sizeof("border")];
    char stringpool_str248[sizeof("stretch")];
    char stringpool_str249[sizeof("outside")];
    char stringpool_str250[sizeof("bold")];
    char stringpool_str251[sizeof("luminance")];
    char stringpool_str252[sizeof("chocolate")];
    char stringpool_str253[sizeof("springgreen")];
    char stringpool_str254[sizeof("no-change")];
    char stringpool_str255[sizeof("slateblue")];
    char stringpool_str256[sizeof("radial-gradient")];
    char stringpool_str257[sizeof("minimal-ui")];
    char stringpool_str258[sizeof("use-script")];
    char stringpool_str259[sizeof("bolder")];
    char stringpool_str260[sizeof("currentcolor")];
    char stringpool_str261[sizeof("blink")];
    char stringpool_str262[sizeof("distribute")];
    char stringpool_str263[sizeof("self-end")];
    char stringpool_str264[sizeof("darkslateblue")];
    char stringpool_str265[sizeof("darkseagreen")];
    char stringpool_str266[sizeof("open")];
    char stringpool_str267[sizeof("ease-in")];
    char stringpool_str268[sizeof("closest-corner")];
    char stringpool_str269[sizeof("break-all")];
    char stringpool_str270[sizeof("condensed")];
    char stringpool_str271[sizeof("scalez")];
    char stringpool_str272[sizeof("avoid")];
    char stringpool_str273[sizeof("darkorchid")];
    char stringpool_str274[sizeof("upper-roman")];
    char stringpool_str275[sizeof("all-scroll")];
    char stringpool_str276[sizeof("upper-latin")];
    char stringpool_str277[sizeof("cornsilk")];
    char stringpool_str278[sizeof("turquoise")];
    char stringpool_str279[sizeof("upper-armenian")];
    char stringpool_str280[sizeof("semi-condensed")];
    char stringpool_str281[sizeof("status-bar")];
    char stringpool_str282[sizeof("step-middle")];
    char stringpool_str283[sizeof("repeating-linear-gradient")];
    char stringpool_str284[sizeof("self-start")];
    char stringpool_str285[sizeof("xor")];
    char stringpool_str286[sizeof("firebrick")];
    char stringpool_str287[sizeof("table-cell")];
    char stringpool_str288[sizeof("snow")];
    char stringpool_str289[sizeof("style")];
    char stringpool_str290[sizeof("unsafe")];
    char stringpool_str291[sizeof("uppercase")];
    char stringpool_str292[sizeof("fallback")];
    char stringpool_str293[sizeof("flex-end")];
    char stringpool_str294[sizeof("alternate-reverse")];
    char stringpool_str295[sizeof("seashell")];
    char stringpool_str296[sizeof("antialiased")];
    char stringpool_str297[sizeof("mediumseagreen")];
    char stringpool_str298[sizeof("s-resize")];
    char stringpool_str299[sizeof("se-resize")];
    char stringpool_str300[sizeof("dimgrey")];
    char stringpool_str301[sizeof("dimgray")];
    char stringpool_str302[sizeof("grayscale")];
    char stringpool_str303[sizeof("n-resize")];
    char stringpool_str304[sizeof("after")];
    char stringpool_str305[sizeof("ne-resize")];
    char stringpool_str306[sizeof("historical-ligatures")];
    char stringpool_str307[sizeof("rotatez")];
    char stringpool_str308[sizeof("darkgrey")];
    char stringpool_str309[sizeof("darkgray")];
    char stringpool_str310[sizeof("serif")];
    char stringpool_str311[sizeof("brightness")];
    char stringpool_str312[sizeof("embed")];
    char stringpool_str313[sizeof("no-punctuation")];
    char stringpool_str314[sizeof("default")];
    char stringpool_str315[sizeof("common-ligatures")];
    char stringpool_str316[sizeof("flex-start")];
    char stringpool_str317[sizeof("chartreuse")];
    char stringpool_str318[sizeof("col-resize")];
    char stringpool_str319[sizeof("crosshair")];
    char stringpool_str320[sizeof("optional")];
    char stringpool_str321[sizeof("no-drop")];
    char stringpool_str322[sizeof("aliceblue")];
    char stringpool_str323[sizeof("scaley")];
    char stringpool_str324[sizeof("brown")];
    char stringpool_str325[sizeof("ease-out")];
    char stringpool_str326[sizeof("hiragana-iroha")];
    char stringpool_str327[sizeof("ns-resize")];
    char stringpool_str328[sizeof("repeating-radial-gradient")];
    char stringpool_str329[sizeof("on-demand")];
    char stringpool_str330[sizeof("sans-serif")];
    char stringpool_str331[sizeof("thin")];
    char stringpool_str332[sizeof("evenodd")];
    char stringpool_str333[sizeof("darkcyan")];
    char stringpool_str334[sizeof("thistle")];
    char stringpool_str335[sizeof("pre")];
    char stringpool_str336[sizeof("slategrey")];
    char stringpool_str337[sizeof("slategray")];
    char stringpool_str338[sizeof("forwards")];
    char stringpool_str339[sizeof("pink")];
    char stringpool_str340[sizeof("table-column")];
    char stringpool_str341[sizeof("fantasy")];
    char stringpool_str342[sizeof("after-edge")];
    char stringpool_str343[sizeof("landscape")];
    char stringpool_str344[sizeof("reset-size")];
    char stringpool_str345[sizeof("maroon")];
    char stringpool_str346[sizeof("transparent")];
    char stringpool_str347[sizeof("rotatey")];
    char stringpool_str348[sizeof("legacy")];
    char stringpool_str349[sizeof("thai")];
    char stringpool_str350[sizeof("browser")];
    char stringpool_str351[sizeof("literal-punctuation")];
    char stringpool_str352[sizeof("malayalam")];
    char stringpool_str353[sizeof("dynamic")];
    char stringpool_str354[sizeof("thick")];
    char stringpool_str355[sizeof("mediumspringgreen")];
    char stringpool_str356[sizeof("x-large")];
    char stringpool_str357[sizeof("grid")];
    char stringpool_str358[sizeof("buttonface")];
    char stringpool_str359[sizeof("cyan")];
    char stringpool_str360[sizeof("x-small")];
    char stringpool_str361[sizeof("color")];
    char stringpool_str362[sizeof("horizontal")];
    char stringpool_str363[sizeof("myanmar")];
    char stringpool_str364[sizeof("palegreen")];
    char stringpool_str365[sizeof("lighten")];
    char stringpool_str366[sizeof("lighter")];
    char stringpool_str367[sizeof("gold")];
    char stringpool_str368[sizeof("ultra-condensed")];
    char stringpool_str369[sizeof("polygon")];
    char stringpool_str370[sizeof("pre-line")];
    char stringpool_str371[sizeof("mistyrose")];
    char stringpool_str372[sizeof("azure")];
    char stringpool_str373[sizeof("skew")];
    char stringpool_str374[sizeof("darkslategrey")];
    char stringpool_str375[sizeof("darkslategray")];
    char stringpool_str376[sizeof("portrait")];
    char stringpool_str377[sizeof("manipulation")];
    char stringpool_str378[sizeof("mediumblue")];
    char stringpool_str379[sizeof("lightgreen")];
    char stringpool_str380[sizeof("sticky")];
    char stringpool_str381[sizeof("media-volume-slider")];
    char stringpool_str382[sizeof("right")];
    char stringpool_str383[sizeof("inset")];
    char stringpool_str384[sizeof("invert")];
    char stringpool_str385[sizeof("both")];
    char stringpool_str386[sizeof("slider-vertical")];
    char stringpool_str387[sizeof("italic")];
    char stringpool_str388[sizeof("isolate")];
    char stringpool_str389[sizeof("scroll")];
    char stringpool_str390[sizeof("overlay")];
    char stringpool_str391[sizeof("dashed")];
    char stringpool_str392[sizeof("initial")];
    char stringpool_str393[sizeof("persian")];
    char stringpool_str394[sizeof("goldenrod")];
    char stringpool_str395[sizeof("plum")];
    char stringpool_str396[sizeof("media-slider")];
    char stringpool_str397[sizeof("upright")];
    char stringpool_str398[sizeof("progress")];
    char stringpool_str399[sizeof("ease-in-out")];
    char stringpool_str400[sizeof("purple")];
    char stringpool_str401[sizeof("difference")];
    char stringpool_str402[sizeof("paint")];
    char stringpool_str403[sizeof("gainsboro")];
    char stringpool_str404[sizeof("darkkhaki")];
    char stringpool_str405[sizeof("translatez")];
    char stringpool_str406[sizeof("blanchedalmond")];
    char stringpool_str407[sizeof("hard-light")];
    char stringpool_str408[sizeof("repeat-y")];
    char stringpool_str409[sizeof("lavenderblush")];
    char stringpool_str410[sizeof("farthest-corner")];
    char stringpool_str411[sizeof("interlace")];
    char stringpool_str412[sizeof("pointer")];
    char stringpool_str413[sizeof("searchfield")];
    char stringpool_str414[sizeof("wrap")];
    char stringpool_str415[sizeof("katakana-iroha")];
    char stringpool_str416[sizeof("off")];
    char stringpool_str417[sizeof("lightcoral")];
    char stringpool_str418[sizeof("ellipse")];
    char stringpool_str419[sizeof("lightsalmon")];
    char stringpool_str420[sizeof("space-evenly")];
    char stringpool_str421[sizeof("square")];
    char stringpool_str422[sizeof("painted")];
    char stringpool_str423[sizeof("matrix")];
    char stringpool_str424[sizeof("media-mute-button")];
    char stringpool_str425[sizeof("mediumpurple")];
    char stringpool_str426[sizeof("minmax")];
    char stringpool_str427[sizeof("midnightblue")];
    char stringpool_str428[sizeof("opacity")];
    char stringpool_str429[sizeof("mixed")];
    char stringpool_str430[sizeof("var")];
    char stringpool_str431[sizeof("fixed")];
    char stringpool_str432[sizeof("soft-light")];
    char stringpool_str433[sizeof("new")];
    char stringpool_str434[sizeof("capitalize")];
    char stringpool_str435[sizeof("stroke")];
    char stringpool_str436[sizeof("show")];
    char stringpool_str437[sizeof("menu")];
    char stringpool_str438[sizeof("swap")];
    char stringpool_str439[sizeof("silver")];
    char stringpool_str440[sizeof("palegoldenrod")];
    char stringpool_str441[sizeof("geometricprecision")];
    char stringpool_str442[sizeof("wait")];
    char stringpool_str443[sizeof("mediumvioletred")];
    char stringpool_str444[sizeof("matrix3d")];
    char stringpool_str445[sizeof("e-resize")];
    char stringpool_str446[sizeof("violet")];
    char stringpool_str447[sizeof("palevioletred")];
    char stringpool_str448[sizeof("wavy")];
    char stringpool_str449[sizeof("ellipsis")];
    char stringpool_str450[sizeof("saturate")];
    char stringpool_str451[sizeof("content-box")];
    char stringpool_str452[sizeof("scalex")];
    char stringpool_str453[sizeof("paused")];
    char stringpool_str454[sizeof("mandatory")];
    char stringpool_str455[sizeof("icon")];
    char stringpool_str456[sizeof("skyblue")];
    char stringpool_str457[sizeof("preserve-3d")];
    char stringpool_str458[sizeof("row")];
    char stringpool_str459[sizeof("translatey")];
    char stringpool_str460[sizeof("multiply")];
    char stringpool_str461[sizeof("saturation")];
    char stringpool_str462[sizeof("inline")];
    char stringpool_str463[sizeof("column")];
    char stringpool_str464[sizeof("urdu")];
    char stringpool_str465[sizeof("source-atop")];
    char stringpool_str466[sizeof("indigo")];
    char stringpool_str467[sizeof("threedface")];
    char stringpool_str468[sizeof("horizontal-tb")];
    char stringpool_str469[sizeof("orchid")];
    char stringpool_str470[sizeof("fuchsia")];
    char stringpool_str471[sizeof("auto")];
    char stringpool_str472[sizeof("media-volume-slider-container")];
    char stringpool_str473[sizeof("pan-up")];
    char stringpool_str474[sizeof("luminosity")];
    char stringpool_str475[sizeof("non-scaling-stroke")];
    char stringpool_str476[sizeof("button-bevel")];
    char stringpool_str477[sizeof("monospace")];
    char stringpool_str478[sizeof("no-close-quote")];
    char stringpool_str479[sizeof("document")];
    char stringpool_str480[sizeof("rotatex")];
    char stringpool_str481[sizeof("menulist")];
    char stringpool_str482[sizeof("indianred")];
    char stringpool_str483[sizeof("inside")];
    char stringpool_str484[sizeof("scrollbar")];
    char stringpool_str485[sizeof("text")];
    char stringpool_str486[sizeof("wrap-reverse")];
    char stringpool_str487[sizeof("message-box")];
    char stringpool_str488[sizeof("lower-roman")];
    char stringpool_str489[sizeof("smooth")];
    char stringpool_str490[sizeof("color-stop")];
    char stringpool_str491[sizeof("simp-chinese-formal")];
    char stringpool_str492[sizeof("lower-greek")];
    char stringpool_str493[sizeof("lower-latin")];
    char stringpool_str494[sizeof("not-allowed")];
    char stringpool_str495[sizeof("olive")];
    char stringpool_str496[sizeof("aqua")];
    char stringpool_str497[sizeof("lower-armenian")];
    char stringpool_str498[sizeof("simp-chinese-informal")];
    char stringpool_str499[sizeof("table-column-group")];
    char stringpool_str500[sizeof("textarea")];
    char stringpool_str501[sizeof("inline-grid")];
    char stringpool_str502[sizeof("padding")];
    char stringpool_str503[sizeof("read-only")];
    char stringpool_str504[sizeof("mathematical")];
    char stringpool_str505[sizeof("lowercase")];
    char stringpool_str506[sizeof("vertical")];
    char stringpool_str507[sizeof("no-contextual")];
    char stringpool_str508[sizeof("rating-level-indicator")];
    char stringpool_str509[sizeof("royalblue")];
    char stringpool_str510[sizeof("lightsteelblue")];
    char stringpool_str511[sizeof("nowrap")];
    char stringpool_str512[sizeof("pan-right")];
    char stringpool_str513[sizeof("groove")];
    char stringpool_str514[sizeof("inherit")];
    char stringpool_str515[sizeof("scale-down")];
    char stringpool_str516[sizeof("small-caption")];
    char stringpool_str517[sizeof("pan-left")];
    char stringpool_str518[sizeof("before")];
    char stringpool_str519[sizeof("lightblue")];
    char stringpool_str520[sizeof("alpha")];
    char stringpool_str521[sizeof("searchfield-decoration")];
    char stringpool_str522[sizeof("aquamarine")];
    char stringpool_str523[sizeof("lawngreen")];
    char stringpool_str524[sizeof("hebrew")];
    char stringpool_str525[sizeof("inactiveborder")];
    char stringpool_str526[sizeof("contextual")];
    char stringpool_str527[sizeof("highlight")];
    char stringpool_str528[sizeof("lightpink")];
    char stringpool_str529[sizeof("max-content")];
    char stringpool_str530[sizeof("color-burn")];
    char stringpool_str531[sizeof("buttontext")];
    char stringpool_str532[sizeof("vertical-rl")];
    char stringpool_str533[sizeof("backwards")];
    char stringpool_str534[sizeof("repeat-x")];
    char stringpool_str535[sizeof("economy")];
    char stringpool_str536[sizeof("margin-box")];
    char stringpool_str537[sizeof("context-menu")];
    char stringpool_str538[sizeof("telugu")];
    char stringpool_str539[sizeof("relevancy-level-indicator")];
    char stringpool_str540[sizeof("activeborder")];
    char stringpool_str541[sizeof("listbox")];
    char stringpool_str542[sizeof("lightseagreen")];
    char stringpool_str543[sizeof("arabic-indic")];
    char stringpool_str544[sizeof("gurmukhi")];
    char stringpool_str545[sizeof("exact")];
    char stringpool_str546[sizeof("bidi-override")];
    char stringpool_str547[sizeof("discretionary-ligatures")];
    char stringpool_str548[sizeof("last-baseline")];
    char stringpool_str549[sizeof("decimal-leading-zero")];
    char stringpool_str550[sizeof("korean-hanja-formal")];
    char stringpool_str551[sizeof("color-dodge")];
    char stringpool_str552[sizeof("vertical-lr")];
    char stringpool_str553[sizeof("pan-y")];
    char stringpool_str554[sizeof("upper-alpha")];
    char stringpool_str555[sizeof("mediumslateblue")];
    char stringpool_str556[sizeof("korean-hanja-informal")];
    char stringpool_str557[sizeof("progressive")];
    char stringpool_str558[sizeof("each-line")];
    char stringpool_str559[sizeof("mediumaquamarine")];
    char stringpool_str560[sizeof("sliderthumb-vertical")];
    char stringpool_str561[sizeof("graytext")];
    char stringpool_str562[sizeof("white")];
    char stringpool_str563[sizeof("inline-table")];
    char stringpool_str564[sizeof("translatex")];
    char stringpool_str565[sizeof("above")];
    char stringpool_str566[sizeof("ultra-expanded")];
    char stringpool_str567[sizeof("captiontext")];
    char stringpool_str568[sizeof("text-top")];
    char stringpool_str569[sizeof("wheat")];
    char stringpool_str570[sizeof("gujarati")];
    char stringpool_str571[sizeof("rosybrown")];
    char stringpool_str572[sizeof("lightgrey")];
    char stringpool_str573[sizeof("lightgray")];
    char stringpool_str574[sizeof("no-historical-ligatures")];
    char stringpool_str575[sizeof("skewy")];
    char stringpool_str576[sizeof("absolute")];
    char stringpool_str577[sizeof("-webkit-left")];
    char stringpool_str578[sizeof("progress-bar")];
    char stringpool_str579[sizeof("ivory")];
    char stringpool_str580[sizeof("olivedrab")];
    char stringpool_str581[sizeof("infinite")];
    char stringpool_str582[sizeof("line-through")];
    char stringpool_str583[sizeof("table-row")];
    char stringpool_str584[sizeof("block-axis")];
    char stringpool_str585[sizeof("layout")];
    char stringpool_str586[sizeof("mediumturquoise")];
    char stringpool_str587[sizeof("text-bottom")];
    char stringpool_str588[sizeof("isolate-override")];
    char stringpool_str589[sizeof("lightcyan")];
    char stringpool_str590[sizeof("justify")];
    char stringpool_str591[sizeof("yellow")];
    char stringpool_str592[sizeof("-webkit-center")];
    char stringpool_str593[sizeof("path")];
    char stringpool_str594[sizeof("farthest-side")];
    char stringpool_str595[sizeof("media-time-remaining-display")];
    char stringpool_str596[sizeof("media-controls-background")];
    char stringpool_str597[sizeof("open-quote")];
    char stringpool_str598[sizeof("menulist-button")];
    char stringpool_str599[sizeof("break-word")];
    char stringpool_str600[sizeof("semi-expanded")];
    char stringpool_str601[sizeof("table-caption")];
    char stringpool_str602[sizeof("read-write")];
    char stringpool_str603[sizeof("buttonhighlight")];
    char stringpool_str604[sizeof("-webkit-calc")];
    char stringpool_str605[sizeof("drop-shadow")];
    char stringpool_str606[sizeof("-webkit-control")];
    char stringpool_str607[sizeof("oblique")];
    char stringpool_str608[sizeof("yellowgreen")];
    char stringpool_str609[sizeof("alphabetic")];
    char stringpool_str610[sizeof("w-resize")];
    char stringpool_str611[sizeof("visible")];
    char stringpool_str612[sizeof("close-quote")];
    char stringpool_str613[sizeof("square-button")];
    char stringpool_str614[sizeof("accumulate")];
    char stringpool_str615[sizeof("rebeccapurple")];
    char stringpool_str616[sizeof("inner-spin-button")];
    char stringpool_str617[sizeof("sw-resize")];
    char stringpool_str618[sizeof("-webkit-min-content")];
    char stringpool_str619[sizeof("nw-resize")];
    char stringpool_str620[sizeof("textfield")];
    char stringpool_str621[sizeof("-webkit-radial-gradient")];
    char stringpool_str622[sizeof("media-play-button")];
    char stringpool_str623[sizeof("-webkit-linear-gradient")];
    char stringpool_str624[sizeof("perspective")];
    char stringpool_str625[sizeof("xx-large")];
    char stringpool_str626[sizeof("searchfield-results-decoration")];
    char stringpool_str627[sizeof("media-volume-sliderthumb")];
    char stringpool_str628[sizeof("honeydew")];
    char stringpool_str629[sizeof("subpixel-antialiased")];
    char stringpool_str630[sizeof("exclusion")];
    char stringpool_str631[sizeof("row-reverse")];
    char stringpool_str632[sizeof("-webkit-mini-control")];
    char stringpool_str633[sizeof("media-fullscreen-volume-slider")];
    char stringpool_str634[sizeof("row-resize")];
    char stringpool_str635[sizeof("media-toggle-closed-captions-button")];
    char stringpool_str636[sizeof("trad-chinese-formal")];
    char stringpool_str637[sizeof("expanded")];
    char stringpool_str638[sizeof("korean-hangul-formal")];
    char stringpool_str639[sizeof("-webkit-link")];
    char stringpool_str640[sizeof("-webkit-auto")];
    char stringpool_str641[sizeof("trad-chinese-informal")];
    char stringpool_str642[sizeof("media-sliderthumb")];
    char stringpool_str643[sizeof("-webkit-gradient")];
    char stringpool_str644[sizeof("nesw-resize")];
    char stringpool_str645[sizeof("inline-block")];
    char stringpool_str646[sizeof("-webkit-isolate")];
    char stringpool_str647[sizeof("sideways")];
    char stringpool_str648[sizeof("column-reverse")];
    char stringpool_str649[sizeof("pan-x")];
    char stringpool_str650[sizeof("activecaption")];
    char stringpool_str651[sizeof("peru")];
    char stringpool_str652[sizeof("table-footer-group")];
    char stringpool_str653[sizeof("searchfield-cancel-button")];
    char stringpool_str654[sizeof("table-header-group")];
    char stringpool_str655[sizeof("before-edge")];
    char stringpool_str656[sizeof("text-after-edge")];
    char stringpool_str657[sizeof("deepskyblue")];
    char stringpool_str658[sizeof("saddlebrown")];
    char stringpool_str659[sizeof("inactivecaption")];
    char stringpool_str660[sizeof("mediumorchid")];
    char stringpool_str661[sizeof("whitesmoke")];
    char stringpool_str662[sizeof("appworkspace")];
    char stringpool_str663[sizeof("border-box")];
    char stringpool_str664[sizeof("progress-bar-value")];
    char stringpool_str665[sizeof("pixelated")];
    char stringpool_str666[sizeof("skewx")];
    char stringpool_str667[sizeof("-webkit-image-set")];
    char stringpool_str668[sizeof("optimizespeed")];
    char stringpool_str669[sizeof("media-enter-fullscreen-button")];
    char stringpool_str670[sizeof("pan-down")];
    char stringpool_str671[sizeof("xx-small")];
    char stringpool_str672[sizeof("caps-lock-indicator")];
    char stringpool_str673[sizeof("below")];
    char stringpool_str674[sizeof("threedhighlight")];
    char stringpool_str675[sizeof("proximity")];
    char stringpool_str676[sizeof("visiblepainted")];
    char stringpool_str677[sizeof("-webkit-grab")];
    char stringpool_str678[sizeof("discrete-capacity-level-indicator")];
    char stringpool_str679[sizeof("checkbox")];
    char stringpool_str680[sizeof("-internal-variable-value")];
    char stringpool_str681[sizeof("burlywood")];
    char stringpool_str682[sizeof("-webkit-baseline-middle")];
    char stringpool_str683[sizeof("-internal-media-cast-off-button")];
    char stringpool_str684[sizeof("bounding-box")];
    char stringpool_str685[sizeof("floralwhite")];
    char stringpool_str686[sizeof("cubic-bezier")];
    char stringpool_str687[sizeof("inter-word")];
    char stringpool_str688[sizeof("space-between")];
    char stringpool_str689[sizeof("-webkit-isolate-override")];
    char stringpool_str690[sizeof("darkturquoise")];
    char stringpool_str691[sizeof("lower-alpha")];
    char stringpool_str692[sizeof("lightslategrey")];
    char stringpool_str693[sizeof("lightslategray")];
    char stringpool_str694[sizeof("cornflowerblue")];
    char stringpool_str695[sizeof("plus-lighter")];
    char stringpool_str696[sizeof("pre-wrap")];
    char stringpool_str697[sizeof("menutext")];
    char stringpool_str698[sizeof("always")];
    char stringpool_str699[sizeof("plaintext")];
    char stringpool_str700[sizeof("continuous-capacity-level-indicator")];
    char stringpool_str701[sizeof("buttonshadow")];
    char stringpool_str702[sizeof("-webkit-activelink")];
    char stringpool_str703[sizeof("-webkit-text")];
    char stringpool_str704[sizeof("ew-resize")];
    char stringpool_str705[sizeof("-webkit-small-control")];
    char stringpool_str706[sizeof("visiblefill")];
    char stringpool_str707[sizeof("scroll-position")];
    char stringpool_str708[sizeof("lightskyblue")];
    char stringpool_str709[sizeof("greenyellow")];
    char stringpool_str710[sizeof("extra-condensed")];
    char stringpool_str711[sizeof("cjk-ideographic")];
    char stringpool_str712[sizeof("visual")];
    char stringpool_str713[sizeof("padding-box")];
    char stringpool_str714[sizeof("nwse-resize")];
    char stringpool_str715[sizeof("visiblestroke")];
    char stringpool_str716[sizeof("inline-axis")];
    char stringpool_str717[sizeof("-webkit-grabbing")];
    char stringpool_str718[sizeof("media-current-time-display")];
    char stringpool_str719[sizeof("-webkit-fit-content")];
    char stringpool_str720[sizeof("-webkit-zoom-in")];
    char stringpool_str721[sizeof("sliderthumb-horizontal")];
    char stringpool_str722[sizeof("no-open-quote")];
    char stringpool_str723[sizeof("inline-flex")];
    char stringpool_str724[sizeof("push-button")];
    char stringpool_str725[sizeof("ideographic")];
    char stringpool_str726[sizeof("-webkit-right")];
    char stringpool_str727[sizeof("-webkit-max-content")];
    char stringpool_str728[sizeof("-webkit-box")];
    char stringpool_str729[sizeof("-webkit-body")];
    char stringpool_str730[sizeof("lemonchiffon")];
    char stringpool_str731[sizeof("-webkit-pictograph")];
    char stringpool_str732[sizeof("peachpuff")];
    char stringpool_str733[sizeof("sandybrown")];
    char stringpool_str734[sizeof("threedshadow")];
    char stringpool_str735[sizeof("slider-horizontal")];
    char stringpool_str736[sizeof("table-row-group")];
    char stringpool_str737[sizeof("-webkit-zoom-out")];
    char stringpool_str738[sizeof("-internal-extend-to-zoom")];
    char stringpool_str739[sizeof("-webkit-fill-available")];
    char stringpool_str740[sizeof("-webkit-cross-fade")];
    char stringpool_str741[sizeof("-webkit-match-parent")];
    char stringpool_str742[sizeof("threeddarkshadow")];
    char stringpool_str743[sizeof("cjk-heavenly-stem")];
    char stringpool_str744[sizeof("window")];
    char stringpool_str745[sizeof("ghostwhite")];
    char stringpool_str746[sizeof("-webkit-focus-ring-color")];
    char stringpool_str747[sizeof("media-controls-fullscreen-background")];
    char stringpool_str748[sizeof("text-before-edge")];
    char stringpool_str749[sizeof("-internal-active-list-box-selection")];
    char stringpool_str750[sizeof("media-overlay-play-button")];
    char stringpool_str751[sizeof("lightyellow")];
    char stringpool_str752[sizeof("optimizelegibility")];
    char stringpool_str753[sizeof("no-discretionary-ligatures")];
    char stringpool_str754[sizeof("vertical-right")];
    char stringpool_str755[sizeof("ethiopic-halehame")];
    char stringpool_str756[sizeof("-webkit-repeating-linear-gradient")];
    char stringpool_str757[sizeof("-webkit-optimize-contrast")];
    char stringpool_str758[sizeof("media-fullscreen-volume-slider-thumb")];
    char stringpool_str759[sizeof("-webkit-repeating-radial-gradient")];
    char stringpool_str760[sizeof("menulist-text")];
    char stringpool_str761[sizeof("powderblue")];
    char stringpool_str762[sizeof("-webkit-nowrap")];
    char stringpool_str763[sizeof("ethiopic-halehame-am")];
    char stringpool_str764[sizeof("vertical-text")];
    char stringpool_str765[sizeof("highlighttext")];
    char stringpool_str766[sizeof("media-exit-fullscreen-button")];
    char stringpool_str767[sizeof("ethiopic-halehame-ti-et")];
    char stringpool_str768[sizeof("extra-expanded")];
    char stringpool_str769[sizeof("ethiopic-halehame-ti-er")];
    char stringpool_str770[sizeof("-webkit-paged-y")];
    char stringpool_str771[sizeof("lightgoldenrodyellow")];
    char stringpool_str772[sizeof("paleturquoise")];
    char stringpool_str773[sizeof("-webkit-inline-box")];
    char stringpool_str774[sizeof("threedlightshadow")];
    char stringpool_str775[sizeof("-internal-media-overlay-cast-off-button")];
    char stringpool_str776[sizeof("inactivecaptiontext")];
    char stringpool_str777[sizeof("navajowhite")];
    char stringpool_str778[sizeof("-webkit-plaintext")];
    char stringpool_str779[sizeof("cjk-earthly-branch")];
    char stringpool_str780[sizeof("optimizequality")];
    char stringpool_str781[sizeof("infobackground")];
    char stringpool_str782[sizeof("infotext")];
    char stringpool_str783[sizeof("-webkit-inline-flex")];
    char stringpool_str784[sizeof("antiquewhite")];
    char stringpool_str785[sizeof("after-white-space")];
    char stringpool_str786[sizeof("-internal-inactive-list-box-selection")];
    char stringpool_str787[sizeof("sideways-right")];
    char stringpool_str788[sizeof("-webkit-paged-x")];
    char stringpool_str789[sizeof("menulist-textfield")];
    char stringpool_str790[sizeof("-webkit-flex")];
    char stringpool_str791[sizeof("windowframe")];
    char stringpool_str792[sizeof("papayawhip")];
    char stringpool_str793[sizeof("windowtext")];
    char stringpool_str794[sizeof("-internal-active-list-box-selection-text")];
    char stringpool_str795[sizeof("read-write-plaintext-only")];
    char stringpool_str796[sizeof("-webkit-xxx-large")];
    char stringpool_str797[sizeof("-internal-inactive-list-box-selection-text")];
  };
static const struct stringpool_t stringpool_contents =
  {
    "lr",
    "900",
    "800",
    "700",
    "400",
    "600",
    "ltr",
    "500",
    "200",
    "100",
    "dot",
    "300",
    "lao",
    "drag",
    "lime",
    "fine",
    "start",
    "large",
    "meter",
    "miter",
    "linen",
    "none",
    "from",
    "darken",
    "larger",
    "hide",
    "linear",
    "srgb",
    "help",
    "fill",
    "hanging",
    "slice",
    "magenta",
    "caret",
    "format",
    "small",
    "seagreen",
    "zoom",
    "ledger",
    "single",
    "calc",
    "cell",
    "nonzero",
    "clip",
    "darkgreen",
    "darkred",
    "b5",
    "limegreen",
    "smaller",
    "disc",
    "salmon",
    "b4",
    "move",
    "rl",
    "normal",
    "circle",
    "steps",
    "filled",
    "darkorange",
    "red",
    "url",
    "letter",
    "hover",
    "rtl",
    "hsl",
    "beige",
    "scale",
    "navy",
    "flat",
    "mongolian",
    "ridge",
    "under",
    "markers",
    "screen",
    "center",
    "cover",
    "dotted",
    "coarse",
    "discard",
    "content",
    "legal",
    "contain",
    "step-end",
    "hiragana",
    "to",
    "lavender",
    "hue",
    "tan",
    "cambodian",
    "zoom-in",
    "unset",
    "crimson",
    "scale3d",
    "bengali",
    "reset",
    "teal",
    "from-image",
    "clear",
    "central",
    "revert",
    "dense",
    "coral",
    "static",
    "darkviolet",
    "forestgreen",
    "sesame",
    "listitem",
    "darksalmon",
    "loose",
    "underline",
    "rotate",
    "on",
    "hsla",
    "bottom",
    "bevel",
    "linear-gradient",
    "step-start",
    "keep-all",
    "hangul",
    "moccasin",
    "mintcream",
    "contents",
    "contrast",
    "space",
    "rect",
    "devanagari",
    "local",
    "sienna",
    "at",
    "baseline",
    "a5",
    "rotate3d",
    "a3",
    "double",
    "reverse",
    "a4",
    "oriya",
    "list-item",
    "attr",
    "hue-rotate",
    "copy",
    "over",
    "blur",
    "blue",
    "darkgoldenrod",
    "tomato",
    "clone",
    "darkolivegreen",
    "relative",
    "kannada",
    "repeat",
    "butt",
    "all",
    "armenian",
    "zoom-out",
    "rgb",
    "hotpink",
    "triangle",
    "balance",
    "destination-in",
    "linearrgb",
    "button",
    "separate",
    "standalone",
    "no-repeat",
    "caption",
    "darkmagenta",
    "overline",
    "alternate",
    "solid",
    "top",
    "min-content",
    "fit-content",
    "source-in",
    "digits",
    "super",
    "oldlace",
    "strict",
    "collapse",
    "tb",
    "radio",
    "lr-tb",
    "sub",
    "bisque",
    "black",
    "block",
    "span",
    "fullscreen",
    "rgba",
    "destination-over",
    "table",
    "radial",
    "decimal",
    "counter",
    "cursive",
    "background",
    "katakana",
    "logical",
    "orange",
    "blueviolet",
    "tibetan",
    "hand",
    "safe",
    "alias",
    "end",
    "green",
    "running",
    "round",
    "grey",
    "gray",
    "medium",
    "hangul-consonant",
    "darkblue",
    "sepia",
    "khaki",
    "destination-out",
    "rl-tb",
    "hidden",
    "khmer",
    "orangered",
    "georgian",
    "closest-side",
    "counters",
    "left",
    "tb-rl",
    "spell-out",
    "translate",
    "element",
    "ease",
    "small-caps",
    "double-circle",
    "source-over",
    "deeppink",
    "steelblue",
    "middle",
    "multiple",
    "outset",
    "source-out",
    "dodgerblue",
    "translate3d",
    "cadetblue",
    "crispedges",
    "space-around",
    "no-common-ligatures",
    "destination-atop",
    "flex",
    "no-drag",
    "border",
    "stretch",
    "outside",
    "bold",
    "luminance",
    "chocolate",
    "springgreen",
    "no-change",
    "slateblue",
    "radial-gradient",
    "minimal-ui",
    "use-script",
    "bolder",
    "currentcolor",
    "blink",
    "distribute",
    "self-end",
    "darkslateblue",
    "darkseagreen",
    "open",
    "ease-in",
    "closest-corner",
    "break-all",
    "condensed",
    "scalez",
    "avoid",
    "darkorchid",
    "upper-roman",
    "all-scroll",
    "upper-latin",
    "cornsilk",
    "turquoise",
    "upper-armenian",
    "semi-condensed",
    "status-bar",
    "step-middle",
    "repeating-linear-gradient",
    "self-start",
    "xor",
    "firebrick",
    "table-cell",
    "snow",
    "style",
    "unsafe",
    "uppercase",
    "fallback",
    "flex-end",
    "alternate-reverse",
    "seashell",
    "antialiased",
    "mediumseagreen",
    "s-resize",
    "se-resize",
    "dimgrey",
    "dimgray",
    "grayscale",
    "n-resize",
    "after",
    "ne-resize",
    "historical-ligatures",
    "rotatez",
    "darkgrey",
    "darkgray",
    "serif",
    "brightness",
    "embed",
    "no-punctuation",
    "default",
    "common-ligatures",
    "flex-start",
    "chartreuse",
    "col-resize",
    "crosshair",
    "optional",
    "no-drop",
    "aliceblue",
    "scaley",
    "brown",
    "ease-out",
    "hiragana-iroha",
    "ns-resize",
    "repeating-radial-gradient",
    "on-demand",
    "sans-serif",
    "thin",
    "evenodd",
    "darkcyan",
    "thistle",
    "pre",
    "slategrey",
    "slategray",
    "forwards",
    "pink",
    "table-column",
    "fantasy",
    "after-edge",
    "landscape",
    "reset-size",
    "maroon",
    "transparent",
    "rotatey",
    "legacy",
    "thai",
    "browser",
    "literal-punctuation",
    "malayalam",
    "dynamic",
    "thick",
    "mediumspringgreen",
    "x-large",
    "grid",
    "buttonface",
    "cyan",
    "x-small",
    "color",
    "horizontal",
    "myanmar",
    "palegreen",
    "lighten",
    "lighter",
    "gold",
    "ultra-condensed",
    "polygon",
    "pre-line",
    "mistyrose",
    "azure",
    "skew",
    "darkslategrey",
    "darkslategray",
    "portrait",
    "manipulation",
    "mediumblue",
    "lightgreen",
    "sticky",
    "media-volume-slider",
    "right",
    "inset",
    "invert",
    "both",
    "slider-vertical",
    "italic",
    "isolate",
    "scroll",
    "overlay",
    "dashed",
    "initial",
    "persian",
    "goldenrod",
    "plum",
    "media-slider",
    "upright",
    "progress",
    "ease-in-out",
    "purple",
    "difference",
    "paint",
    "gainsboro",
    "darkkhaki",
    "translatez",
    "blanchedalmond",
    "hard-light",
    "repeat-y",
    "lavenderblush",
    "farthest-corner",
    "interlace",
    "pointer",
    "searchfield",
    "wrap",
    "katakana-iroha",
    "off",
    "lightcoral",
    "ellipse",
    "lightsalmon",
    "space-evenly",
    "square",
    "painted",
    "matrix",
    "media-mute-button",
    "mediumpurple",
    "minmax",
    "midnightblue",
    "opacity",
    "mixed",
    "var",
    "fixed",
    "soft-light",
    "new",
    "capitalize",
    "stroke",
    "show",
    "menu",
    "swap",
    "silver",
    "palegoldenrod",
    "geometricprecision",
    "wait",
    "mediumvioletred",
    "matrix3d",
    "e-resize",
    "violet",
    "palevioletred",
    "wavy",
    "ellipsis",
    "saturate",
    "content-box",
    "scalex",
    "paused",
    "mandatory",
    "icon",
    "skyblue",
    "preserve-3d",
    "row",
    "translatey",
    "multiply",
    "saturation",
    "inline",
    "column",
    "urdu",
    "source-atop",
    "indigo",
    "threedface",
    "horizontal-tb",
    "orchid",
    "fuchsia",
    "auto",
    "media-volume-slider-container",
    "pan-up",
    "luminosity",
    "non-scaling-stroke",
    "button-bevel",
    "monospace",
    "no-close-quote",
    "document",
    "rotatex",
    "menulist",
    "indianred",
    "inside",
    "scrollbar",
    "text",
    "wrap-reverse",
    "message-box",
    "lower-roman",
    "smooth",
    "color-stop",
    "simp-chinese-formal",
    "lower-greek",
    "lower-latin",
    "not-allowed",
    "olive",
    "aqua",
    "lower-armenian",
    "simp-chinese-informal",
    "table-column-group",
    "textarea",
    "inline-grid",
    "padding",
    "read-only",
    "mathematical",
    "lowercase",
    "vertical",
    "no-contextual",
    "rating-level-indicator",
    "royalblue",
    "lightsteelblue",
    "nowrap",
    "pan-right",
    "groove",
    "inherit",
    "scale-down",
    "small-caption",
    "pan-left",
    "before",
    "lightblue",
    "alpha",
    "searchfield-decoration",
    "aquamarine",
    "lawngreen",
    "hebrew",
    "inactiveborder",
    "contextual",
    "highlight",
    "lightpink",
    "max-content",
    "color-burn",
    "buttontext",
    "vertical-rl",
    "backwards",
    "repeat-x",
    "economy",
    "margin-box",
    "context-menu",
    "telugu",
    "relevancy-level-indicator",
    "activeborder",
    "listbox",
    "lightseagreen",
    "arabic-indic",
    "gurmukhi",
    "exact",
    "bidi-override",
    "discretionary-ligatures",
    "last-baseline",
    "decimal-leading-zero",
    "korean-hanja-formal",
    "color-dodge",
    "vertical-lr",
    "pan-y",
    "upper-alpha",
    "mediumslateblue",
    "korean-hanja-informal",
    "progressive",
    "each-line",
    "mediumaquamarine",
    "sliderthumb-vertical",
    "graytext",
    "white",
    "inline-table",
    "translatex",
    "above",
    "ultra-expanded",
    "captiontext",
    "text-top",
    "wheat",
    "gujarati",
    "rosybrown",
    "lightgrey",
    "lightgray",
    "no-historical-ligatures",
    "skewy",
    "absolute",
    "-webkit-left",
    "progress-bar",
    "ivory",
    "olivedrab",
    "infinite",
    "line-through",
    "table-row",
    "block-axis",
    "layout",
    "mediumturquoise",
    "text-bottom",
    "isolate-override",
    "lightcyan",
    "justify",
    "yellow",
    "-webkit-center",
    "path",
    "farthest-side",
    "media-time-remaining-display",
    "media-controls-background",
    "open-quote",
    "menulist-button",
    "break-word",
    "semi-expanded",
    "table-caption",
    "read-write",
    "buttonhighlight",
    "-webkit-calc",
    "drop-shadow",
    "-webkit-control",
    "oblique",
    "yellowgreen",
    "alphabetic",
    "w-resize",
    "visible",
    "close-quote",
    "square-button",
    "accumulate",
    "rebeccapurple",
    "inner-spin-button",
    "sw-resize",
    "-webkit-min-content",
    "nw-resize",
    "textfield",
    "-webkit-radial-gradient",
    "media-play-button",
    "-webkit-linear-gradient",
    "perspective",
    "xx-large",
    "searchfield-results-decoration",
    "media-volume-sliderthumb",
    "honeydew",
    "subpixel-antialiased",
    "exclusion",
    "row-reverse",
    "-webkit-mini-control",
    "media-fullscreen-volume-slider",
    "row-resize",
    "media-toggle-closed-captions-button",
    "trad-chinese-formal",
    "expanded",
    "korean-hangul-formal",
    "-webkit-link",
    "-webkit-auto",
    "trad-chinese-informal",
    "media-sliderthumb",
    "-webkit-gradient",
    "nesw-resize",
    "inline-block",
    "-webkit-isolate",
    "sideways",
    "column-reverse",
    "pan-x",
    "activecaption",
    "peru",
    "table-footer-group",
    "searchfield-cancel-button",
    "table-header-group",
    "before-edge",
    "text-after-edge",
    "deepskyblue",
    "saddlebrown",
    "inactivecaption",
    "mediumorchid",
    "whitesmoke",
    "appworkspace",
    "border-box",
    "progress-bar-value",
    "pixelated",
    "skewx",
    "-webkit-image-set",
    "optimizespeed",
    "media-enter-fullscreen-button",
    "pan-down",
    "xx-small",
    "caps-lock-indicator",
    "below",
    "threedhighlight",
    "proximity",
    "visiblepainted",
    "-webkit-grab",
    "discrete-capacity-level-indicator",
    "checkbox",
    "-internal-variable-value",
    "burlywood",
    "-webkit-baseline-middle",
    "-internal-media-cast-off-button",
    "bounding-box",
    "floralwhite",
    "cubic-bezier",
    "inter-word",
    "space-between",
    "-webkit-isolate-override",
    "darkturquoise",
    "lower-alpha",
    "lightslategrey",
    "lightslategray",
    "cornflowerblue",
    "plus-lighter",
    "pre-wrap",
    "menutext",
    "always",
    "plaintext",
    "continuous-capacity-level-indicator",
    "buttonshadow",
    "-webkit-activelink",
    "-webkit-text",
    "ew-resize",
    "-webkit-small-control",
    "visiblefill",
    "scroll-position",
    "lightskyblue",
    "greenyellow",
    "extra-condensed",
    "cjk-ideographic",
    "visual",
    "padding-box",
    "nwse-resize",
    "visiblestroke",
    "inline-axis",
    "-webkit-grabbing",
    "media-current-time-display",
    "-webkit-fit-content",
    "-webkit-zoom-in",
    "sliderthumb-horizontal",
    "no-open-quote",
    "inline-flex",
    "push-button",
    "ideographic",
    "-webkit-right",
    "-webkit-max-content",
    "-webkit-box",
    "-webkit-body",
    "lemonchiffon",
    "-webkit-pictograph",
    "peachpuff",
    "sandybrown",
    "threedshadow",
    "slider-horizontal",
    "table-row-group",
    "-webkit-zoom-out",
    "-internal-extend-to-zoom",
    "-webkit-fill-available",
    "-webkit-cross-fade",
    "-webkit-match-parent",
    "threeddarkshadow",
    "cjk-heavenly-stem",
    "window",
    "ghostwhite",
    "-webkit-focus-ring-color",
    "media-controls-fullscreen-background",
    "text-before-edge",
    "-internal-active-list-box-selection",
    "media-overlay-play-button",
    "lightyellow",
    "optimizelegibility",
    "no-discretionary-ligatures",
    "vertical-right",
    "ethiopic-halehame",
    "-webkit-repeating-linear-gradient",
    "-webkit-optimize-contrast",
    "media-fullscreen-volume-slider-thumb",
    "-webkit-repeating-radial-gradient",
    "menulist-text",
    "powderblue",
    "-webkit-nowrap",
    "ethiopic-halehame-am",
    "vertical-text",
    "highlighttext",
    "media-exit-fullscreen-button",
    "ethiopic-halehame-ti-et",
    "extra-expanded",
    "ethiopic-halehame-ti-er",
    "-webkit-paged-y",
    "lightgoldenrodyellow",
    "paleturquoise",
    "-webkit-inline-box",
    "threedlightshadow",
    "-internal-media-overlay-cast-off-button",
    "inactivecaptiontext",
    "navajowhite",
    "-webkit-plaintext",
    "cjk-earthly-branch",
    "optimizequality",
    "infobackground",
    "infotext",
    "-webkit-inline-flex",
    "antiquewhite",
    "after-white-space",
    "-internal-inactive-list-box-selection",
    "sideways-right",
    "-webkit-paged-x",
    "menulist-textfield",
    "-webkit-flex",
    "windowframe",
    "papayawhip",
    "windowtext",
    "-internal-active-list-box-selection-text",
    "read-write-plaintext-only",
    "-webkit-xxx-large",
    "-internal-inactive-list-box-selection-text"
  };
#define stringpool ((const char *) &stringpool_contents)
const struct Value *
CSSValueKeywordsHash::findValueImpl (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 798,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 42,
      MIN_HASH_VALUE = 33,
      MAX_HASH_VALUE = 5893
    };

  static const struct Value value_word_list[] =
    {
      {offsetof(struct stringpool_t, stringpool_str0), CSSValueLr},
      {offsetof(struct stringpool_t, stringpool_str1), CSSValue900},
      {offsetof(struct stringpool_t, stringpool_str2), CSSValue800},
      {offsetof(struct stringpool_t, stringpool_str3), CSSValue700},
      {offsetof(struct stringpool_t, stringpool_str4), CSSValue400},
      {offsetof(struct stringpool_t, stringpool_str5), CSSValue600},
      {offsetof(struct stringpool_t, stringpool_str6), CSSValueLtr},
      {offsetof(struct stringpool_t, stringpool_str7), CSSValue500},
      {offsetof(struct stringpool_t, stringpool_str8), CSSValue200},
      {offsetof(struct stringpool_t, stringpool_str9), CSSValue100},
      {offsetof(struct stringpool_t, stringpool_str10), CSSValueDot},
      {offsetof(struct stringpool_t, stringpool_str11), CSSValue300},
      {offsetof(struct stringpool_t, stringpool_str12), CSSValueLao},
      {offsetof(struct stringpool_t, stringpool_str13), CSSValueDrag},
      {offsetof(struct stringpool_t, stringpool_str14), CSSValueLime},
      {offsetof(struct stringpool_t, stringpool_str15), CSSValueFine},
      {offsetof(struct stringpool_t, stringpool_str16), CSSValueStart},
      {offsetof(struct stringpool_t, stringpool_str17), CSSValueLarge},
      {offsetof(struct stringpool_t, stringpool_str18), CSSValueMeter},
      {offsetof(struct stringpool_t, stringpool_str19), CSSValueMiter},
      {offsetof(struct stringpool_t, stringpool_str20), CSSValueLinen},
      {offsetof(struct stringpool_t, stringpool_str21), CSSValueNone},
      {offsetof(struct stringpool_t, stringpool_str22), CSSValueFrom},
      {offsetof(struct stringpool_t, stringpool_str23), CSSValueDarken},
      {offsetof(struct stringpool_t, stringpool_str24), CSSValueLarger},
      {offsetof(struct stringpool_t, stringpool_str25), CSSValueHide},
      {offsetof(struct stringpool_t, stringpool_str26), CSSValueLinear},
      {offsetof(struct stringpool_t, stringpool_str27), CSSValueSRGB},
      {offsetof(struct stringpool_t, stringpool_str28), CSSValueHelp},
      {offsetof(struct stringpool_t, stringpool_str29), CSSValueFill},
      {offsetof(struct stringpool_t, stringpool_str30), CSSValueHanging},
      {offsetof(struct stringpool_t, stringpool_str31), CSSValueSlice},
      {offsetof(struct stringpool_t, stringpool_str32), CSSValueMagenta},
      {offsetof(struct stringpool_t, stringpool_str33), CSSValueCaret},
      {offsetof(struct stringpool_t, stringpool_str34), CSSValueFormat},
      {offsetof(struct stringpool_t, stringpool_str35), CSSValueSmall},
      {offsetof(struct stringpool_t, stringpool_str36), CSSValueSeagreen},
      {offsetof(struct stringpool_t, stringpool_str37), CSSValueZoom},
      {offsetof(struct stringpool_t, stringpool_str38), CSSValueLedger},
      {offsetof(struct stringpool_t, stringpool_str39), CSSValueSingle},
      {offsetof(struct stringpool_t, stringpool_str40), CSSValueCalc},
      {offsetof(struct stringpool_t, stringpool_str41), CSSValueCell},
      {offsetof(struct stringpool_t, stringpool_str42), CSSValueNonzero},
      {offsetof(struct stringpool_t, stringpool_str43), CSSValueClip},
      {offsetof(struct stringpool_t, stringpool_str44), CSSValueDarkgreen},
      {offsetof(struct stringpool_t, stringpool_str45), CSSValueDarkred},
      {offsetof(struct stringpool_t, stringpool_str46), CSSValueB5},
      {offsetof(struct stringpool_t, stringpool_str47), CSSValueLimegreen},
      {offsetof(struct stringpool_t, stringpool_str48), CSSValueSmaller},
      {offsetof(struct stringpool_t, stringpool_str49), CSSValueDisc},
      {offsetof(struct stringpool_t, stringpool_str50), CSSValueSalmon},
      {offsetof(struct stringpool_t, stringpool_str51), CSSValueB4},
      {offsetof(struct stringpool_t, stringpool_str52), CSSValueMove},
      {offsetof(struct stringpool_t, stringpool_str53), CSSValueRl},
      {offsetof(struct stringpool_t, stringpool_str54), CSSValueNormal},
      {offsetof(struct stringpool_t, stringpool_str55), CSSValueCircle},
      {offsetof(struct stringpool_t, stringpool_str56), CSSValueSteps},
      {offsetof(struct stringpool_t, stringpool_str57), CSSValueFilled},
      {offsetof(struct stringpool_t, stringpool_str58), CSSValueDarkorange},
      {offsetof(struct stringpool_t, stringpool_str59), CSSValueRed},
      {offsetof(struct stringpool_t, stringpool_str60), CSSValueUrl},
      {offsetof(struct stringpool_t, stringpool_str61), CSSValueLetter},
      {offsetof(struct stringpool_t, stringpool_str62), CSSValueHover},
      {offsetof(struct stringpool_t, stringpool_str63), CSSValueRtl},
      {offsetof(struct stringpool_t, stringpool_str64), CSSValueHsl},
      {offsetof(struct stringpool_t, stringpool_str65), CSSValueBeige},
      {offsetof(struct stringpool_t, stringpool_str66), CSSValueScale},
      {offsetof(struct stringpool_t, stringpool_str67), CSSValueNavy},
      {offsetof(struct stringpool_t, stringpool_str68), CSSValueFlat},
      {offsetof(struct stringpool_t, stringpool_str69), CSSValueMongolian},
      {offsetof(struct stringpool_t, stringpool_str70), CSSValueRidge},
      {offsetof(struct stringpool_t, stringpool_str71), CSSValueUnder},
      {offsetof(struct stringpool_t, stringpool_str72), CSSValueMarkers},
      {offsetof(struct stringpool_t, stringpool_str73), CSSValueScreen},
      {offsetof(struct stringpool_t, stringpool_str74), CSSValueCenter},
      {offsetof(struct stringpool_t, stringpool_str75), CSSValueCover},
      {offsetof(struct stringpool_t, stringpool_str76), CSSValueDotted},
      {offsetof(struct stringpool_t, stringpool_str77), CSSValueCoarse},
      {offsetof(struct stringpool_t, stringpool_str78), CSSValueDiscard},
      {offsetof(struct stringpool_t, stringpool_str79), CSSValueContent},
      {offsetof(struct stringpool_t, stringpool_str80), CSSValueLegal},
      {offsetof(struct stringpool_t, stringpool_str81), CSSValueContain},
      {offsetof(struct stringpool_t, stringpool_str82), CSSValueStepEnd},
      {offsetof(struct stringpool_t, stringpool_str83), CSSValueHiragana},
      {offsetof(struct stringpool_t, stringpool_str84), CSSValueTo},
      {offsetof(struct stringpool_t, stringpool_str85), CSSValueLavender},
      {offsetof(struct stringpool_t, stringpool_str86), CSSValueHue},
      {offsetof(struct stringpool_t, stringpool_str87), CSSValueTan},
      {offsetof(struct stringpool_t, stringpool_str88), CSSValueCambodian},
      {offsetof(struct stringpool_t, stringpool_str89), CSSValueZoomIn},
      {offsetof(struct stringpool_t, stringpool_str90), CSSValueUnset},
      {offsetof(struct stringpool_t, stringpool_str91), CSSValueCrimson},
      {offsetof(struct stringpool_t, stringpool_str92), CSSValueScale3d},
      {offsetof(struct stringpool_t, stringpool_str93), CSSValueBengali},
      {offsetof(struct stringpool_t, stringpool_str94), CSSValueReset},
      {offsetof(struct stringpool_t, stringpool_str95), CSSValueTeal},
      {offsetof(struct stringpool_t, stringpool_str96), CSSValueFromImage},
      {offsetof(struct stringpool_t, stringpool_str97), CSSValueClear},
      {offsetof(struct stringpool_t, stringpool_str98), CSSValueCentral},
      {offsetof(struct stringpool_t, stringpool_str99), CSSValueRevert},
      {offsetof(struct stringpool_t, stringpool_str100), CSSValueDense},
      {offsetof(struct stringpool_t, stringpool_str101), CSSValueCoral},
      {offsetof(struct stringpool_t, stringpool_str102), CSSValueStatic},
      {offsetof(struct stringpool_t, stringpool_str103), CSSValueDarkviolet},
      {offsetof(struct stringpool_t, stringpool_str104), CSSValueForestgreen},
      {offsetof(struct stringpool_t, stringpool_str105), CSSValueSesame},
      {offsetof(struct stringpool_t, stringpool_str106), CSSValueListitem},
      {offsetof(struct stringpool_t, stringpool_str107), CSSValueDarksalmon},
      {offsetof(struct stringpool_t, stringpool_str108), CSSValueLoose},
      {offsetof(struct stringpool_t, stringpool_str109), CSSValueUnderline},
      {offsetof(struct stringpool_t, stringpool_str110), CSSValueRotate},
      {offsetof(struct stringpool_t, stringpool_str111), CSSValueOn},
      {offsetof(struct stringpool_t, stringpool_str112), CSSValueHsla},
      {offsetof(struct stringpool_t, stringpool_str113), CSSValueBottom},
      {offsetof(struct stringpool_t, stringpool_str114), CSSValueBevel},
      {offsetof(struct stringpool_t, stringpool_str115), CSSValueLinearGradient},
      {offsetof(struct stringpool_t, stringpool_str116), CSSValueStepStart},
      {offsetof(struct stringpool_t, stringpool_str117), CSSValueKeepAll},
      {offsetof(struct stringpool_t, stringpool_str118), CSSValueHangul},
      {offsetof(struct stringpool_t, stringpool_str119), CSSValueMoccasin},
      {offsetof(struct stringpool_t, stringpool_str120), CSSValueMintcream},
      {offsetof(struct stringpool_t, stringpool_str121), CSSValueContents},
      {offsetof(struct stringpool_t, stringpool_str122), CSSValueContrast},
      {offsetof(struct stringpool_t, stringpool_str123), CSSValueSpace},
      {offsetof(struct stringpool_t, stringpool_str124), CSSValueRect},
      {offsetof(struct stringpool_t, stringpool_str125), CSSValueDevanagari},
      {offsetof(struct stringpool_t, stringpool_str126), CSSValueLocal},
      {offsetof(struct stringpool_t, stringpool_str127), CSSValueSienna},
      {offsetof(struct stringpool_t, stringpool_str128), CSSValueAt},
      {offsetof(struct stringpool_t, stringpool_str129), CSSValueBaseline},
      {offsetof(struct stringpool_t, stringpool_str130), CSSValueA5},
      {offsetof(struct stringpool_t, stringpool_str131), CSSValueRotate3d},
      {offsetof(struct stringpool_t, stringpool_str132), CSSValueA3},
      {offsetof(struct stringpool_t, stringpool_str133), CSSValueDouble},
      {offsetof(struct stringpool_t, stringpool_str134), CSSValueReverse},
      {offsetof(struct stringpool_t, stringpool_str135), CSSValueA4},
      {offsetof(struct stringpool_t, stringpool_str136), CSSValueOriya},
      {offsetof(struct stringpool_t, stringpool_str137), CSSValueListItem},
      {offsetof(struct stringpool_t, stringpool_str138), CSSValueAttr},
      {offsetof(struct stringpool_t, stringpool_str139), CSSValueHueRotate},
      {offsetof(struct stringpool_t, stringpool_str140), CSSValueCopy},
      {offsetof(struct stringpool_t, stringpool_str141), CSSValueOver},
      {offsetof(struct stringpool_t, stringpool_str142), CSSValueBlur},
      {offsetof(struct stringpool_t, stringpool_str143), CSSValueBlue},
      {offsetof(struct stringpool_t, stringpool_str144), CSSValueDarkgoldenrod},
      {offsetof(struct stringpool_t, stringpool_str145), CSSValueTomato},
      {offsetof(struct stringpool_t, stringpool_str146), CSSValueClone},
      {offsetof(struct stringpool_t, stringpool_str147), CSSValueDarkolivegreen},
      {offsetof(struct stringpool_t, stringpool_str148), CSSValueRelative},
      {offsetof(struct stringpool_t, stringpool_str149), CSSValueKannada},
      {offsetof(struct stringpool_t, stringpool_str150), CSSValueRepeat},
      {offsetof(struct stringpool_t, stringpool_str151), CSSValueButt},
      {offsetof(struct stringpool_t, stringpool_str152), CSSValueAll},
      {offsetof(struct stringpool_t, stringpool_str153), CSSValueArmenian},
      {offsetof(struct stringpool_t, stringpool_str154), CSSValueZoomOut},
      {offsetof(struct stringpool_t, stringpool_str155), CSSValueRgb},
      {offsetof(struct stringpool_t, stringpool_str156), CSSValueHotpink},
      {offsetof(struct stringpool_t, stringpool_str157), CSSValueTriangle},
      {offsetof(struct stringpool_t, stringpool_str158), CSSValueBalance},
      {offsetof(struct stringpool_t, stringpool_str159), CSSValueDestinationIn},
      {offsetof(struct stringpool_t, stringpool_str160), CSSValueLinearRGB},
      {offsetof(struct stringpool_t, stringpool_str161), CSSValueButton},
      {offsetof(struct stringpool_t, stringpool_str162), CSSValueSeparate},
      {offsetof(struct stringpool_t, stringpool_str163), CSSValueStandalone},
      {offsetof(struct stringpool_t, stringpool_str164), CSSValueNoRepeat},
      {offsetof(struct stringpool_t, stringpool_str165), CSSValueCaption},
      {offsetof(struct stringpool_t, stringpool_str166), CSSValueDarkmagenta},
      {offsetof(struct stringpool_t, stringpool_str167), CSSValueOverline},
      {offsetof(struct stringpool_t, stringpool_str168), CSSValueAlternate},
      {offsetof(struct stringpool_t, stringpool_str169), CSSValueSolid},
      {offsetof(struct stringpool_t, stringpool_str170), CSSValueTop},
      {offsetof(struct stringpool_t, stringpool_str171), CSSValueMinContent},
      {offsetof(struct stringpool_t, stringpool_str172), CSSValueFitContent},
      {offsetof(struct stringpool_t, stringpool_str173), CSSValueSourceIn},
      {offsetof(struct stringpool_t, stringpool_str174), CSSValueDigits},
      {offsetof(struct stringpool_t, stringpool_str175), CSSValueSuper},
      {offsetof(struct stringpool_t, stringpool_str176), CSSValueOldlace},
      {offsetof(struct stringpool_t, stringpool_str177), CSSValueStrict},
      {offsetof(struct stringpool_t, stringpool_str178), CSSValueCollapse},
      {offsetof(struct stringpool_t, stringpool_str179), CSSValueTb},
      {offsetof(struct stringpool_t, stringpool_str180), CSSValueRadio},
      {offsetof(struct stringpool_t, stringpool_str181), CSSValueLrTb},
      {offsetof(struct stringpool_t, stringpool_str182), CSSValueSub},
      {offsetof(struct stringpool_t, stringpool_str183), CSSValueBisque},
      {offsetof(struct stringpool_t, stringpool_str184), CSSValueBlack},
      {offsetof(struct stringpool_t, stringpool_str185), CSSValueBlock},
      {offsetof(struct stringpool_t, stringpool_str186), CSSValueSpan},
      {offsetof(struct stringpool_t, stringpool_str187), CSSValueFullscreen},
      {offsetof(struct stringpool_t, stringpool_str188), CSSValueRgba},
      {offsetof(struct stringpool_t, stringpool_str189), CSSValueDestinationOver},
      {offsetof(struct stringpool_t, stringpool_str190), CSSValueTable},
      {offsetof(struct stringpool_t, stringpool_str191), CSSValueRadial},
      {offsetof(struct stringpool_t, stringpool_str192), CSSValueDecimal},
      {offsetof(struct stringpool_t, stringpool_str193), CSSValueCounter},
      {offsetof(struct stringpool_t, stringpool_str194), CSSValueCursive},
      {offsetof(struct stringpool_t, stringpool_str195), CSSValueBackground},
      {offsetof(struct stringpool_t, stringpool_str196), CSSValueKatakana},
      {offsetof(struct stringpool_t, stringpool_str197), CSSValueLogical},
      {offsetof(struct stringpool_t, stringpool_str198), CSSValueOrange},
      {offsetof(struct stringpool_t, stringpool_str199), CSSValueBlueviolet},
      {offsetof(struct stringpool_t, stringpool_str200), CSSValueTibetan},
      {offsetof(struct stringpool_t, stringpool_str201), CSSValueHand},
      {offsetof(struct stringpool_t, stringpool_str202), CSSValueSafe},
      {offsetof(struct stringpool_t, stringpool_str203), CSSValueAlias},
      {offsetof(struct stringpool_t, stringpool_str204), CSSValueEnd},
      {offsetof(struct stringpool_t, stringpool_str205), CSSValueGreen},
      {offsetof(struct stringpool_t, stringpool_str206), CSSValueRunning},
      {offsetof(struct stringpool_t, stringpool_str207), CSSValueRound},
      {offsetof(struct stringpool_t, stringpool_str208), CSSValueGrey},
      {offsetof(struct stringpool_t, stringpool_str209), CSSValueGray},
      {offsetof(struct stringpool_t, stringpool_str210), CSSValueMedium},
      {offsetof(struct stringpool_t, stringpool_str211), CSSValueHangulConsonant},
      {offsetof(struct stringpool_t, stringpool_str212), CSSValueDarkblue},
      {offsetof(struct stringpool_t, stringpool_str213), CSSValueSepia},
      {offsetof(struct stringpool_t, stringpool_str214), CSSValueKhaki},
      {offsetof(struct stringpool_t, stringpool_str215), CSSValueDestinationOut},
      {offsetof(struct stringpool_t, stringpool_str216), CSSValueRlTb},
      {offsetof(struct stringpool_t, stringpool_str217), CSSValueHidden},
      {offsetof(struct stringpool_t, stringpool_str218), CSSValueKhmer},
      {offsetof(struct stringpool_t, stringpool_str219), CSSValueOrangered},
      {offsetof(struct stringpool_t, stringpool_str220), CSSValueGeorgian},
      {offsetof(struct stringpool_t, stringpool_str221), CSSValueClosestSide},
      {offsetof(struct stringpool_t, stringpool_str222), CSSValueCounters},
      {offsetof(struct stringpool_t, stringpool_str223), CSSValueLeft},
      {offsetof(struct stringpool_t, stringpool_str224), CSSValueTbRl},
      {offsetof(struct stringpool_t, stringpool_str225), CSSValueSpellOut},
      {offsetof(struct stringpool_t, stringpool_str226), CSSValueTranslate},
      {offsetof(struct stringpool_t, stringpool_str227), CSSValueElement},
      {offsetof(struct stringpool_t, stringpool_str228), CSSValueEase},
      {offsetof(struct stringpool_t, stringpool_str229), CSSValueSmallCaps},
      {offsetof(struct stringpool_t, stringpool_str230), CSSValueDoubleCircle},
      {offsetof(struct stringpool_t, stringpool_str231), CSSValueSourceOver},
      {offsetof(struct stringpool_t, stringpool_str232), CSSValueDeeppink},
      {offsetof(struct stringpool_t, stringpool_str233), CSSValueSteelblue},
      {offsetof(struct stringpool_t, stringpool_str234), CSSValueMiddle},
      {offsetof(struct stringpool_t, stringpool_str235), CSSValueMultiple},
      {offsetof(struct stringpool_t, stringpool_str236), CSSValueOutset},
      {offsetof(struct stringpool_t, stringpool_str237), CSSValueSourceOut},
      {offsetof(struct stringpool_t, stringpool_str238), CSSValueDodgerblue},
      {offsetof(struct stringpool_t, stringpool_str239), CSSValueTranslate3d},
      {offsetof(struct stringpool_t, stringpool_str240), CSSValueCadetblue},
      {offsetof(struct stringpool_t, stringpool_str241), CSSValueCrispEdges},
      {offsetof(struct stringpool_t, stringpool_str242), CSSValueSpaceAround},
      {offsetof(struct stringpool_t, stringpool_str243), CSSValueNoCommonLigatures},
      {offsetof(struct stringpool_t, stringpool_str244), CSSValueDestinationAtop},
      {offsetof(struct stringpool_t, stringpool_str245), CSSValueFlex},
      {offsetof(struct stringpool_t, stringpool_str246), CSSValueNoDrag},
      {offsetof(struct stringpool_t, stringpool_str247), CSSValueBorder},
      {offsetof(struct stringpool_t, stringpool_str248), CSSValueStretch},
      {offsetof(struct stringpool_t, stringpool_str249), CSSValueOutside},
      {offsetof(struct stringpool_t, stringpool_str250), CSSValueBold},
      {offsetof(struct stringpool_t, stringpool_str251), CSSValueLuminance},
      {offsetof(struct stringpool_t, stringpool_str252), CSSValueChocolate},
      {offsetof(struct stringpool_t, stringpool_str253), CSSValueSpringgreen},
      {offsetof(struct stringpool_t, stringpool_str254), CSSValueNoChange},
      {offsetof(struct stringpool_t, stringpool_str255), CSSValueSlateblue},
      {offsetof(struct stringpool_t, stringpool_str256), CSSValueRadialGradient},
      {offsetof(struct stringpool_t, stringpool_str257), CSSValueMinimalUi},
      {offsetof(struct stringpool_t, stringpool_str258), CSSValueUseScript},
      {offsetof(struct stringpool_t, stringpool_str259), CSSValueBolder},
      {offsetof(struct stringpool_t, stringpool_str260), CSSValueCurrentcolor},
      {offsetof(struct stringpool_t, stringpool_str261), CSSValueBlink},
      {offsetof(struct stringpool_t, stringpool_str262), CSSValueDistribute},
      {offsetof(struct stringpool_t, stringpool_str263), CSSValueSelfEnd},
      {offsetof(struct stringpool_t, stringpool_str264), CSSValueDarkslateblue},
      {offsetof(struct stringpool_t, stringpool_str265), CSSValueDarkseagreen},
      {offsetof(struct stringpool_t, stringpool_str266), CSSValueOpen},
      {offsetof(struct stringpool_t, stringpool_str267), CSSValueEaseIn},
      {offsetof(struct stringpool_t, stringpool_str268), CSSValueClosestCorner},
      {offsetof(struct stringpool_t, stringpool_str269), CSSValueBreakAll},
      {offsetof(struct stringpool_t, stringpool_str270), CSSValueCondensed},
      {offsetof(struct stringpool_t, stringpool_str271), CSSValueScaleZ},
      {offsetof(struct stringpool_t, stringpool_str272), CSSValueAvoid},
      {offsetof(struct stringpool_t, stringpool_str273), CSSValueDarkorchid},
      {offsetof(struct stringpool_t, stringpool_str274), CSSValueUpperRoman},
      {offsetof(struct stringpool_t, stringpool_str275), CSSValueAllScroll},
      {offsetof(struct stringpool_t, stringpool_str276), CSSValueUpperLatin},
      {offsetof(struct stringpool_t, stringpool_str277), CSSValueCornsilk},
      {offsetof(struct stringpool_t, stringpool_str278), CSSValueTurquoise},
      {offsetof(struct stringpool_t, stringpool_str279), CSSValueUpperArmenian},
      {offsetof(struct stringpool_t, stringpool_str280), CSSValueSemiCondensed},
      {offsetof(struct stringpool_t, stringpool_str281), CSSValueStatusBar},
      {offsetof(struct stringpool_t, stringpool_str282), CSSValueStepMiddle},
      {offsetof(struct stringpool_t, stringpool_str283), CSSValueRepeatingLinearGradient},
      {offsetof(struct stringpool_t, stringpool_str284), CSSValueSelfStart},
      {offsetof(struct stringpool_t, stringpool_str285), CSSValueXor},
      {offsetof(struct stringpool_t, stringpool_str286), CSSValueFirebrick},
      {offsetof(struct stringpool_t, stringpool_str287), CSSValueTableCell},
      {offsetof(struct stringpool_t, stringpool_str288), CSSValueSnow},
      {offsetof(struct stringpool_t, stringpool_str289), CSSValueStyle},
      {offsetof(struct stringpool_t, stringpool_str290), CSSValueUnsafe},
      {offsetof(struct stringpool_t, stringpool_str291), CSSValueUppercase},
      {offsetof(struct stringpool_t, stringpool_str292), CSSValueFallback},
      {offsetof(struct stringpool_t, stringpool_str293), CSSValueFlexEnd},
      {offsetof(struct stringpool_t, stringpool_str294), CSSValueAlternateReverse},
      {offsetof(struct stringpool_t, stringpool_str295), CSSValueSeashell},
      {offsetof(struct stringpool_t, stringpool_str296), CSSValueAntialiased},
      {offsetof(struct stringpool_t, stringpool_str297), CSSValueMediumseagreen},
      {offsetof(struct stringpool_t, stringpool_str298), CSSValueSResize},
      {offsetof(struct stringpool_t, stringpool_str299), CSSValueSeResize},
      {offsetof(struct stringpool_t, stringpool_str300), CSSValueDimgrey},
      {offsetof(struct stringpool_t, stringpool_str301), CSSValueDimgray},
      {offsetof(struct stringpool_t, stringpool_str302), CSSValueGrayscale},
      {offsetof(struct stringpool_t, stringpool_str303), CSSValueNResize},
      {offsetof(struct stringpool_t, stringpool_str304), CSSValueAfter},
      {offsetof(struct stringpool_t, stringpool_str305), CSSValueNeResize},
      {offsetof(struct stringpool_t, stringpool_str306), CSSValueHistoricalLigatures},
      {offsetof(struct stringpool_t, stringpool_str307), CSSValueRotateZ},
      {offsetof(struct stringpool_t, stringpool_str308), CSSValueDarkgrey},
      {offsetof(struct stringpool_t, stringpool_str309), CSSValueDarkgray},
      {offsetof(struct stringpool_t, stringpool_str310), CSSValueSerif},
      {offsetof(struct stringpool_t, stringpool_str311), CSSValueBrightness},
      {offsetof(struct stringpool_t, stringpool_str312), CSSValueEmbed},
      {offsetof(struct stringpool_t, stringpool_str313), CSSValueNoPunctuation},
      {offsetof(struct stringpool_t, stringpool_str314), CSSValueDefault},
      {offsetof(struct stringpool_t, stringpool_str315), CSSValueCommonLigatures},
      {offsetof(struct stringpool_t, stringpool_str316), CSSValueFlexStart},
      {offsetof(struct stringpool_t, stringpool_str317), CSSValueChartreuse},
      {offsetof(struct stringpool_t, stringpool_str318), CSSValueColResize},
      {offsetof(struct stringpool_t, stringpool_str319), CSSValueCrosshair},
      {offsetof(struct stringpool_t, stringpool_str320), CSSValueOptional},
      {offsetof(struct stringpool_t, stringpool_str321), CSSValueNoDrop},
      {offsetof(struct stringpool_t, stringpool_str322), CSSValueAliceblue},
      {offsetof(struct stringpool_t, stringpool_str323), CSSValueScaleY},
      {offsetof(struct stringpool_t, stringpool_str324), CSSValueBrown},
      {offsetof(struct stringpool_t, stringpool_str325), CSSValueEaseOut},
      {offsetof(struct stringpool_t, stringpool_str326), CSSValueHiraganaIroha},
      {offsetof(struct stringpool_t, stringpool_str327), CSSValueNsResize},
      {offsetof(struct stringpool_t, stringpool_str328), CSSValueRepeatingRadialGradient},
      {offsetof(struct stringpool_t, stringpool_str329), CSSValueOnDemand},
      {offsetof(struct stringpool_t, stringpool_str330), CSSValueSansSerif},
      {offsetof(struct stringpool_t, stringpool_str331), CSSValueThin},
      {offsetof(struct stringpool_t, stringpool_str332), CSSValueEvenodd},
      {offsetof(struct stringpool_t, stringpool_str333), CSSValueDarkcyan},
      {offsetof(struct stringpool_t, stringpool_str334), CSSValueThistle},
      {offsetof(struct stringpool_t, stringpool_str335), CSSValuePre},
      {offsetof(struct stringpool_t, stringpool_str336), CSSValueSlategrey},
      {offsetof(struct stringpool_t, stringpool_str337), CSSValueSlategray},
      {offsetof(struct stringpool_t, stringpool_str338), CSSValueForwards},
      {offsetof(struct stringpool_t, stringpool_str339), CSSValuePink},
      {offsetof(struct stringpool_t, stringpool_str340), CSSValueTableColumn},
      {offsetof(struct stringpool_t, stringpool_str341), CSSValueFantasy},
      {offsetof(struct stringpool_t, stringpool_str342), CSSValueAfterEdge},
      {offsetof(struct stringpool_t, stringpool_str343), CSSValueLandscape},
      {offsetof(struct stringpool_t, stringpool_str344), CSSValueResetSize},
      {offsetof(struct stringpool_t, stringpool_str345), CSSValueMaroon},
      {offsetof(struct stringpool_t, stringpool_str346), CSSValueTransparent},
      {offsetof(struct stringpool_t, stringpool_str347), CSSValueRotateY},
      {offsetof(struct stringpool_t, stringpool_str348), CSSValueLegacy},
      {offsetof(struct stringpool_t, stringpool_str349), CSSValueThai},
      {offsetof(struct stringpool_t, stringpool_str350), CSSValueBrowser},
      {offsetof(struct stringpool_t, stringpool_str351), CSSValueLiteralPunctuation},
      {offsetof(struct stringpool_t, stringpool_str352), CSSValueMalayalam},
      {offsetof(struct stringpool_t, stringpool_str353), CSSValueDynamic},
      {offsetof(struct stringpool_t, stringpool_str354), CSSValueThick},
      {offsetof(struct stringpool_t, stringpool_str355), CSSValueMediumspringgreen},
      {offsetof(struct stringpool_t, stringpool_str356), CSSValueXLarge},
      {offsetof(struct stringpool_t, stringpool_str357), CSSValueGrid},
      {offsetof(struct stringpool_t, stringpool_str358), CSSValueButtonface},
      {offsetof(struct stringpool_t, stringpool_str359), CSSValueCyan},
      {offsetof(struct stringpool_t, stringpool_str360), CSSValueXSmall},
      {offsetof(struct stringpool_t, stringpool_str361), CSSValueColor},
      {offsetof(struct stringpool_t, stringpool_str362), CSSValueHorizontal},
      {offsetof(struct stringpool_t, stringpool_str363), CSSValueMyanmar},
      {offsetof(struct stringpool_t, stringpool_str364), CSSValuePalegreen},
      {offsetof(struct stringpool_t, stringpool_str365), CSSValueLighten},
      {offsetof(struct stringpool_t, stringpool_str366), CSSValueLighter},
      {offsetof(struct stringpool_t, stringpool_str367), CSSValueGold},
      {offsetof(struct stringpool_t, stringpool_str368), CSSValueUltraCondensed},
      {offsetof(struct stringpool_t, stringpool_str369), CSSValuePolygon},
      {offsetof(struct stringpool_t, stringpool_str370), CSSValuePreLine},
      {offsetof(struct stringpool_t, stringpool_str371), CSSValueMistyrose},
      {offsetof(struct stringpool_t, stringpool_str372), CSSValueAzure},
      {offsetof(struct stringpool_t, stringpool_str373), CSSValueSkew},
      {offsetof(struct stringpool_t, stringpool_str374), CSSValueDarkslategrey},
      {offsetof(struct stringpool_t, stringpool_str375), CSSValueDarkslategray},
      {offsetof(struct stringpool_t, stringpool_str376), CSSValuePortrait},
      {offsetof(struct stringpool_t, stringpool_str377), CSSValueManipulation},
      {offsetof(struct stringpool_t, stringpool_str378), CSSValueMediumblue},
      {offsetof(struct stringpool_t, stringpool_str379), CSSValueLightgreen},
      {offsetof(struct stringpool_t, stringpool_str380), CSSValueSticky},
      {offsetof(struct stringpool_t, stringpool_str381), CSSValueMediaVolumeSlider},
      {offsetof(struct stringpool_t, stringpool_str382), CSSValueRight},
      {offsetof(struct stringpool_t, stringpool_str383), CSSValueInset},
      {offsetof(struct stringpool_t, stringpool_str384), CSSValueInvert},
      {offsetof(struct stringpool_t, stringpool_str385), CSSValueBoth},
      {offsetof(struct stringpool_t, stringpool_str386), CSSValueSliderVertical},
      {offsetof(struct stringpool_t, stringpool_str387), CSSValueItalic},
      {offsetof(struct stringpool_t, stringpool_str388), CSSValueIsolate},
      {offsetof(struct stringpool_t, stringpool_str389), CSSValueScroll},
      {offsetof(struct stringpool_t, stringpool_str390), CSSValueOverlay},
      {offsetof(struct stringpool_t, stringpool_str391), CSSValueDashed},
      {offsetof(struct stringpool_t, stringpool_str392), CSSValueInitial},
      {offsetof(struct stringpool_t, stringpool_str393), CSSValuePersian},
      {offsetof(struct stringpool_t, stringpool_str394), CSSValueGoldenrod},
      {offsetof(struct stringpool_t, stringpool_str395), CSSValuePlum},
      {offsetof(struct stringpool_t, stringpool_str396), CSSValueMediaSlider},
      {offsetof(struct stringpool_t, stringpool_str397), CSSValueUpright},
      {offsetof(struct stringpool_t, stringpool_str398), CSSValueProgress},
      {offsetof(struct stringpool_t, stringpool_str399), CSSValueEaseInOut},
      {offsetof(struct stringpool_t, stringpool_str400), CSSValuePurple},
      {offsetof(struct stringpool_t, stringpool_str401), CSSValueDifference},
      {offsetof(struct stringpool_t, stringpool_str402), CSSValuePaint},
      {offsetof(struct stringpool_t, stringpool_str403), CSSValueGainsboro},
      {offsetof(struct stringpool_t, stringpool_str404), CSSValueDarkkhaki},
      {offsetof(struct stringpool_t, stringpool_str405), CSSValueTranslateZ},
      {offsetof(struct stringpool_t, stringpool_str406), CSSValueBlanchedalmond},
      {offsetof(struct stringpool_t, stringpool_str407), CSSValueHardLight},
      {offsetof(struct stringpool_t, stringpool_str408), CSSValueRepeatY},
      {offsetof(struct stringpool_t, stringpool_str409), CSSValueLavenderblush},
      {offsetof(struct stringpool_t, stringpool_str410), CSSValueFarthestCorner},
      {offsetof(struct stringpool_t, stringpool_str411), CSSValueInterlace},
      {offsetof(struct stringpool_t, stringpool_str412), CSSValuePointer},
      {offsetof(struct stringpool_t, stringpool_str413), CSSValueSearchfield},
      {offsetof(struct stringpool_t, stringpool_str414), CSSValueWrap},
      {offsetof(struct stringpool_t, stringpool_str415), CSSValueKatakanaIroha},
      {offsetof(struct stringpool_t, stringpool_str416), CSSValueOff},
      {offsetof(struct stringpool_t, stringpool_str417), CSSValueLightcoral},
      {offsetof(struct stringpool_t, stringpool_str418), CSSValueEllipse},
      {offsetof(struct stringpool_t, stringpool_str419), CSSValueLightsalmon},
      {offsetof(struct stringpool_t, stringpool_str420), CSSValueSpaceEvenly},
      {offsetof(struct stringpool_t, stringpool_str421), CSSValueSquare},
      {offsetof(struct stringpool_t, stringpool_str422), CSSValuePainted},
      {offsetof(struct stringpool_t, stringpool_str423), CSSValueMatrix},
      {offsetof(struct stringpool_t, stringpool_str424), CSSValueMediaMuteButton},
      {offsetof(struct stringpool_t, stringpool_str425), CSSValueMediumpurple},
      {offsetof(struct stringpool_t, stringpool_str426), CSSValueMinmax},
      {offsetof(struct stringpool_t, stringpool_str427), CSSValueMidnightblue},
      {offsetof(struct stringpool_t, stringpool_str428), CSSValueOpacity},
      {offsetof(struct stringpool_t, stringpool_str429), CSSValueMixed},
      {offsetof(struct stringpool_t, stringpool_str430), CSSValueVar},
      {offsetof(struct stringpool_t, stringpool_str431), CSSValueFixed},
      {offsetof(struct stringpool_t, stringpool_str432), CSSValueSoftLight},
      {offsetof(struct stringpool_t, stringpool_str433), CSSValueNew},
      {offsetof(struct stringpool_t, stringpool_str434), CSSValueCapitalize},
      {offsetof(struct stringpool_t, stringpool_str435), CSSValueStroke},
      {offsetof(struct stringpool_t, stringpool_str436), CSSValueShow},
      {offsetof(struct stringpool_t, stringpool_str437), CSSValueMenu},
      {offsetof(struct stringpool_t, stringpool_str438), CSSValueSwap},
      {offsetof(struct stringpool_t, stringpool_str439), CSSValueSilver},
      {offsetof(struct stringpool_t, stringpool_str440), CSSValuePalegoldenrod},
      {offsetof(struct stringpool_t, stringpool_str441), CSSValueGeometricPrecision},
      {offsetof(struct stringpool_t, stringpool_str442), CSSValueWait},
      {offsetof(struct stringpool_t, stringpool_str443), CSSValueMediumvioletred},
      {offsetof(struct stringpool_t, stringpool_str444), CSSValueMatrix3d},
      {offsetof(struct stringpool_t, stringpool_str445), CSSValueEResize},
      {offsetof(struct stringpool_t, stringpool_str446), CSSValueViolet},
      {offsetof(struct stringpool_t, stringpool_str447), CSSValuePalevioletred},
      {offsetof(struct stringpool_t, stringpool_str448), CSSValueWavy},
      {offsetof(struct stringpool_t, stringpool_str449), CSSValueEllipsis},
      {offsetof(struct stringpool_t, stringpool_str450), CSSValueSaturate},
      {offsetof(struct stringpool_t, stringpool_str451), CSSValueContentBox},
      {offsetof(struct stringpool_t, stringpool_str452), CSSValueScaleX},
      {offsetof(struct stringpool_t, stringpool_str453), CSSValuePaused},
      {offsetof(struct stringpool_t, stringpool_str454), CSSValueMandatory},
      {offsetof(struct stringpool_t, stringpool_str455), CSSValueIcon},
      {offsetof(struct stringpool_t, stringpool_str456), CSSValueSkyblue},
      {offsetof(struct stringpool_t, stringpool_str457), CSSValuePreserve3d},
      {offsetof(struct stringpool_t, stringpool_str458), CSSValueRow},
      {offsetof(struct stringpool_t, stringpool_str459), CSSValueTranslateY},
      {offsetof(struct stringpool_t, stringpool_str460), CSSValueMultiply},
      {offsetof(struct stringpool_t, stringpool_str461), CSSValueSaturation},
      {offsetof(struct stringpool_t, stringpool_str462), CSSValueInline},
      {offsetof(struct stringpool_t, stringpool_str463), CSSValueColumn},
      {offsetof(struct stringpool_t, stringpool_str464), CSSValueUrdu},
      {offsetof(struct stringpool_t, stringpool_str465), CSSValueSourceAtop},
      {offsetof(struct stringpool_t, stringpool_str466), CSSValueIndigo},
      {offsetof(struct stringpool_t, stringpool_str467), CSSValueThreedface},
      {offsetof(struct stringpool_t, stringpool_str468), CSSValueHorizontalTb},
      {offsetof(struct stringpool_t, stringpool_str469), CSSValueOrchid},
      {offsetof(struct stringpool_t, stringpool_str470), CSSValueFuchsia},
      {offsetof(struct stringpool_t, stringpool_str471), CSSValueAuto},
      {offsetof(struct stringpool_t, stringpool_str472), CSSValueMediaVolumeSliderContainer},
      {offsetof(struct stringpool_t, stringpool_str473), CSSValuePanUp},
      {offsetof(struct stringpool_t, stringpool_str474), CSSValueLuminosity},
      {offsetof(struct stringpool_t, stringpool_str475), CSSValueNonScalingStroke},
      {offsetof(struct stringpool_t, stringpool_str476), CSSValueButtonBevel},
      {offsetof(struct stringpool_t, stringpool_str477), CSSValueMonospace},
      {offsetof(struct stringpool_t, stringpool_str478), CSSValueNoCloseQuote},
      {offsetof(struct stringpool_t, stringpool_str479), CSSValueDocument},
      {offsetof(struct stringpool_t, stringpool_str480), CSSValueRotateX},
      {offsetof(struct stringpool_t, stringpool_str481), CSSValueMenulist},
      {offsetof(struct stringpool_t, stringpool_str482), CSSValueIndianred},
      {offsetof(struct stringpool_t, stringpool_str483), CSSValueInside},
      {offsetof(struct stringpool_t, stringpool_str484), CSSValueScrollbar},
      {offsetof(struct stringpool_t, stringpool_str485), CSSValueText},
      {offsetof(struct stringpool_t, stringpool_str486), CSSValueWrapReverse},
      {offsetof(struct stringpool_t, stringpool_str487), CSSValueMessageBox},
      {offsetof(struct stringpool_t, stringpool_str488), CSSValueLowerRoman},
      {offsetof(struct stringpool_t, stringpool_str489), CSSValueSmooth},
      {offsetof(struct stringpool_t, stringpool_str490), CSSValueColorStop},
      {offsetof(struct stringpool_t, stringpool_str491), CSSValueSimpChineseFormal},
      {offsetof(struct stringpool_t, stringpool_str492), CSSValueLowerGreek},
      {offsetof(struct stringpool_t, stringpool_str493), CSSValueLowerLatin},
      {offsetof(struct stringpool_t, stringpool_str494), CSSValueNotAllowed},
      {offsetof(struct stringpool_t, stringpool_str495), CSSValueOlive},
      {offsetof(struct stringpool_t, stringpool_str496), CSSValueAqua},
      {offsetof(struct stringpool_t, stringpool_str497), CSSValueLowerArmenian},
      {offsetof(struct stringpool_t, stringpool_str498), CSSValueSimpChineseInformal},
      {offsetof(struct stringpool_t, stringpool_str499), CSSValueTableColumnGroup},
      {offsetof(struct stringpool_t, stringpool_str500), CSSValueTextarea},
      {offsetof(struct stringpool_t, stringpool_str501), CSSValueInlineGrid},
      {offsetof(struct stringpool_t, stringpool_str502), CSSValuePadding},
      {offsetof(struct stringpool_t, stringpool_str503), CSSValueReadOnly},
      {offsetof(struct stringpool_t, stringpool_str504), CSSValueMathematical},
      {offsetof(struct stringpool_t, stringpool_str505), CSSValueLowercase},
      {offsetof(struct stringpool_t, stringpool_str506), CSSValueVertical},
      {offsetof(struct stringpool_t, stringpool_str507), CSSValueNoContextual},
      {offsetof(struct stringpool_t, stringpool_str508), CSSValueRatingLevelIndicator},
      {offsetof(struct stringpool_t, stringpool_str509), CSSValueRoyalblue},
      {offsetof(struct stringpool_t, stringpool_str510), CSSValueLightsteelblue},
      {offsetof(struct stringpool_t, stringpool_str511), CSSValueNowrap},
      {offsetof(struct stringpool_t, stringpool_str512), CSSValuePanRight},
      {offsetof(struct stringpool_t, stringpool_str513), CSSValueGroove},
      {offsetof(struct stringpool_t, stringpool_str514), CSSValueInherit},
      {offsetof(struct stringpool_t, stringpool_str515), CSSValueScaleDown},
      {offsetof(struct stringpool_t, stringpool_str516), CSSValueSmallCaption},
      {offsetof(struct stringpool_t, stringpool_str517), CSSValuePanLeft},
      {offsetof(struct stringpool_t, stringpool_str518), CSSValueBefore},
      {offsetof(struct stringpool_t, stringpool_str519), CSSValueLightblue},
      {offsetof(struct stringpool_t, stringpool_str520), CSSValueAlpha},
      {offsetof(struct stringpool_t, stringpool_str521), CSSValueSearchfieldDecoration},
      {offsetof(struct stringpool_t, stringpool_str522), CSSValueAquamarine},
      {offsetof(struct stringpool_t, stringpool_str523), CSSValueLawngreen},
      {offsetof(struct stringpool_t, stringpool_str524), CSSValueHebrew},
      {offsetof(struct stringpool_t, stringpool_str525), CSSValueInactiveborder},
      {offsetof(struct stringpool_t, stringpool_str526), CSSValueContextual},
      {offsetof(struct stringpool_t, stringpool_str527), CSSValueHighlight},
      {offsetof(struct stringpool_t, stringpool_str528), CSSValueLightpink},
      {offsetof(struct stringpool_t, stringpool_str529), CSSValueMaxContent},
      {offsetof(struct stringpool_t, stringpool_str530), CSSValueColorBurn},
      {offsetof(struct stringpool_t, stringpool_str531), CSSValueButtontext},
      {offsetof(struct stringpool_t, stringpool_str532), CSSValueVerticalRl},
      {offsetof(struct stringpool_t, stringpool_str533), CSSValueBackwards},
      {offsetof(struct stringpool_t, stringpool_str534), CSSValueRepeatX},
      {offsetof(struct stringpool_t, stringpool_str535), CSSValueEconomy},
      {offsetof(struct stringpool_t, stringpool_str536), CSSValueMarginBox},
      {offsetof(struct stringpool_t, stringpool_str537), CSSValueContextMenu},
      {offsetof(struct stringpool_t, stringpool_str538), CSSValueTelugu},
      {offsetof(struct stringpool_t, stringpool_str539), CSSValueRelevancyLevelIndicator},
      {offsetof(struct stringpool_t, stringpool_str540), CSSValueActiveborder},
      {offsetof(struct stringpool_t, stringpool_str541), CSSValueListbox},
      {offsetof(struct stringpool_t, stringpool_str542), CSSValueLightseagreen},
      {offsetof(struct stringpool_t, stringpool_str543), CSSValueArabicIndic},
      {offsetof(struct stringpool_t, stringpool_str544), CSSValueGurmukhi},
      {offsetof(struct stringpool_t, stringpool_str545), CSSValueExact},
      {offsetof(struct stringpool_t, stringpool_str546), CSSValueBidiOverride},
      {offsetof(struct stringpool_t, stringpool_str547), CSSValueDiscretionaryLigatures},
      {offsetof(struct stringpool_t, stringpool_str548), CSSValueLastBaseline},
      {offsetof(struct stringpool_t, stringpool_str549), CSSValueDecimalLeadingZero},
      {offsetof(struct stringpool_t, stringpool_str550), CSSValueKoreanHanjaFormal},
      {offsetof(struct stringpool_t, stringpool_str551), CSSValueColorDodge},
      {offsetof(struct stringpool_t, stringpool_str552), CSSValueVerticalLr},
      {offsetof(struct stringpool_t, stringpool_str553), CSSValuePanY},
      {offsetof(struct stringpool_t, stringpool_str554), CSSValueUpperAlpha},
      {offsetof(struct stringpool_t, stringpool_str555), CSSValueMediumslateblue},
      {offsetof(struct stringpool_t, stringpool_str556), CSSValueKoreanHanjaInformal},
      {offsetof(struct stringpool_t, stringpool_str557), CSSValueProgressive},
      {offsetof(struct stringpool_t, stringpool_str558), CSSValueEachLine},
      {offsetof(struct stringpool_t, stringpool_str559), CSSValueMediumaquamarine},
      {offsetof(struct stringpool_t, stringpool_str560), CSSValueSliderthumbVertical},
      {offsetof(struct stringpool_t, stringpool_str561), CSSValueGraytext},
      {offsetof(struct stringpool_t, stringpool_str562), CSSValueWhite},
      {offsetof(struct stringpool_t, stringpool_str563), CSSValueInlineTable},
      {offsetof(struct stringpool_t, stringpool_str564), CSSValueTranslateX},
      {offsetof(struct stringpool_t, stringpool_str565), CSSValueAbove},
      {offsetof(struct stringpool_t, stringpool_str566), CSSValueUltraExpanded},
      {offsetof(struct stringpool_t, stringpool_str567), CSSValueCaptiontext},
      {offsetof(struct stringpool_t, stringpool_str568), CSSValueTextTop},
      {offsetof(struct stringpool_t, stringpool_str569), CSSValueWheat},
      {offsetof(struct stringpool_t, stringpool_str570), CSSValueGujarati},
      {offsetof(struct stringpool_t, stringpool_str571), CSSValueRosybrown},
      {offsetof(struct stringpool_t, stringpool_str572), CSSValueLightgrey},
      {offsetof(struct stringpool_t, stringpool_str573), CSSValueLightgray},
      {offsetof(struct stringpool_t, stringpool_str574), CSSValueNoHistoricalLigatures},
      {offsetof(struct stringpool_t, stringpool_str575), CSSValueSkewY},
      {offsetof(struct stringpool_t, stringpool_str576), CSSValueAbsolute},
      {offsetof(struct stringpool_t, stringpool_str577), CSSValueWebkitLeft},
      {offsetof(struct stringpool_t, stringpool_str578), CSSValueProgressBar},
      {offsetof(struct stringpool_t, stringpool_str579), CSSValueIvory},
      {offsetof(struct stringpool_t, stringpool_str580), CSSValueOlivedrab},
      {offsetof(struct stringpool_t, stringpool_str581), CSSValueInfinite},
      {offsetof(struct stringpool_t, stringpool_str582), CSSValueLineThrough},
      {offsetof(struct stringpool_t, stringpool_str583), CSSValueTableRow},
      {offsetof(struct stringpool_t, stringpool_str584), CSSValueBlockAxis},
      {offsetof(struct stringpool_t, stringpool_str585), CSSValueLayout},
      {offsetof(struct stringpool_t, stringpool_str586), CSSValueMediumturquoise},
      {offsetof(struct stringpool_t, stringpool_str587), CSSValueTextBottom},
      {offsetof(struct stringpool_t, stringpool_str588), CSSValueIsolateOverride},
      {offsetof(struct stringpool_t, stringpool_str589), CSSValueLightcyan},
      {offsetof(struct stringpool_t, stringpool_str590), CSSValueJustify},
      {offsetof(struct stringpool_t, stringpool_str591), CSSValueYellow},
      {offsetof(struct stringpool_t, stringpool_str592), CSSValueWebkitCenter},
      {offsetof(struct stringpool_t, stringpool_str593), CSSValuePath},
      {offsetof(struct stringpool_t, stringpool_str594), CSSValueFarthestSide},
      {offsetof(struct stringpool_t, stringpool_str595), CSSValueMediaTimeRemainingDisplay},
      {offsetof(struct stringpool_t, stringpool_str596), CSSValueMediaControlsBackground},
      {offsetof(struct stringpool_t, stringpool_str597), CSSValueOpenQuote},
      {offsetof(struct stringpool_t, stringpool_str598), CSSValueMenulistButton},
      {offsetof(struct stringpool_t, stringpool_str599), CSSValueBreakWord},
      {offsetof(struct stringpool_t, stringpool_str600), CSSValueSemiExpanded},
      {offsetof(struct stringpool_t, stringpool_str601), CSSValueTableCaption},
      {offsetof(struct stringpool_t, stringpool_str602), CSSValueReadWrite},
      {offsetof(struct stringpool_t, stringpool_str603), CSSValueButtonhighlight},
      {offsetof(struct stringpool_t, stringpool_str604), CSSValueWebkitCalc},
      {offsetof(struct stringpool_t, stringpool_str605), CSSValueDropShadow},
      {offsetof(struct stringpool_t, stringpool_str606), CSSValueWebkitControl},
      {offsetof(struct stringpool_t, stringpool_str607), CSSValueOblique},
      {offsetof(struct stringpool_t, stringpool_str608), CSSValueYellowgreen},
      {offsetof(struct stringpool_t, stringpool_str609), CSSValueAlphabetic},
      {offsetof(struct stringpool_t, stringpool_str610), CSSValueWResize},
      {offsetof(struct stringpool_t, stringpool_str611), CSSValueVisible},
      {offsetof(struct stringpool_t, stringpool_str612), CSSValueCloseQuote},
      {offsetof(struct stringpool_t, stringpool_str613), CSSValueSquareButton},
      {offsetof(struct stringpool_t, stringpool_str614), CSSValueAccumulate},
      {offsetof(struct stringpool_t, stringpool_str615), CSSValueRebeccapurple},
      {offsetof(struct stringpool_t, stringpool_str616), CSSValueInnerSpinButton},
      {offsetof(struct stringpool_t, stringpool_str617), CSSValueSwResize},
      {offsetof(struct stringpool_t, stringpool_str618), CSSValueWebkitMinContent},
      {offsetof(struct stringpool_t, stringpool_str619), CSSValueNwResize},
      {offsetof(struct stringpool_t, stringpool_str620), CSSValueTextfield},
      {offsetof(struct stringpool_t, stringpool_str621), CSSValueWebkitRadialGradient},
      {offsetof(struct stringpool_t, stringpool_str622), CSSValueMediaPlayButton},
      {offsetof(struct stringpool_t, stringpool_str623), CSSValueWebkitLinearGradient},
      {offsetof(struct stringpool_t, stringpool_str624), CSSValuePerspective},
      {offsetof(struct stringpool_t, stringpool_str625), CSSValueXxLarge},
      {offsetof(struct stringpool_t, stringpool_str626), CSSValueSearchfieldResultsDecoration},
      {offsetof(struct stringpool_t, stringpool_str627), CSSValueMediaVolumeSliderthumb},
      {offsetof(struct stringpool_t, stringpool_str628), CSSValueHoneydew},
      {offsetof(struct stringpool_t, stringpool_str629), CSSValueSubpixelAntialiased},
      {offsetof(struct stringpool_t, stringpool_str630), CSSValueExclusion},
      {offsetof(struct stringpool_t, stringpool_str631), CSSValueRowReverse},
      {offsetof(struct stringpool_t, stringpool_str632), CSSValueWebkitMiniControl},
      {offsetof(struct stringpool_t, stringpool_str633), CSSValueMediaFullscreenVolumeSlider},
      {offsetof(struct stringpool_t, stringpool_str634), CSSValueRowResize},
      {offsetof(struct stringpool_t, stringpool_str635), CSSValueMediaToggleClosedCaptionsButton},
      {offsetof(struct stringpool_t, stringpool_str636), CSSValueTradChineseFormal},
      {offsetof(struct stringpool_t, stringpool_str637), CSSValueExpanded},
      {offsetof(struct stringpool_t, stringpool_str638), CSSValueKoreanHangulFormal},
      {offsetof(struct stringpool_t, stringpool_str639), CSSValueWebkitLink},
      {offsetof(struct stringpool_t, stringpool_str640), CSSValueWebkitAuto},
      {offsetof(struct stringpool_t, stringpool_str641), CSSValueTradChineseInformal},
      {offsetof(struct stringpool_t, stringpool_str642), CSSValueMediaSliderthumb},
      {offsetof(struct stringpool_t, stringpool_str643), CSSValueWebkitGradient},
      {offsetof(struct stringpool_t, stringpool_str644), CSSValueNeswResize},
      {offsetof(struct stringpool_t, stringpool_str645), CSSValueInlineBlock},
      {offsetof(struct stringpool_t, stringpool_str646), CSSValueWebkitIsolate},
      {offsetof(struct stringpool_t, stringpool_str647), CSSValueSideways},
      {offsetof(struct stringpool_t, stringpool_str648), CSSValueColumnReverse},
      {offsetof(struct stringpool_t, stringpool_str649), CSSValuePanX},
      {offsetof(struct stringpool_t, stringpool_str650), CSSValueActivecaption},
      {offsetof(struct stringpool_t, stringpool_str651), CSSValuePeru},
      {offsetof(struct stringpool_t, stringpool_str652), CSSValueTableFooterGroup},
      {offsetof(struct stringpool_t, stringpool_str653), CSSValueSearchfieldCancelButton},
      {offsetof(struct stringpool_t, stringpool_str654), CSSValueTableHeaderGroup},
      {offsetof(struct stringpool_t, stringpool_str655), CSSValueBeforeEdge},
      {offsetof(struct stringpool_t, stringpool_str656), CSSValueTextAfterEdge},
      {offsetof(struct stringpool_t, stringpool_str657), CSSValueDeepskyblue},
      {offsetof(struct stringpool_t, stringpool_str658), CSSValueSaddlebrown},
      {offsetof(struct stringpool_t, stringpool_str659), CSSValueInactivecaption},
      {offsetof(struct stringpool_t, stringpool_str660), CSSValueMediumorchid},
      {offsetof(struct stringpool_t, stringpool_str661), CSSValueWhitesmoke},
      {offsetof(struct stringpool_t, stringpool_str662), CSSValueAppworkspace},
      {offsetof(struct stringpool_t, stringpool_str663), CSSValueBorderBox},
      {offsetof(struct stringpool_t, stringpool_str664), CSSValueProgressBarValue},
      {offsetof(struct stringpool_t, stringpool_str665), CSSValuePixelated},
      {offsetof(struct stringpool_t, stringpool_str666), CSSValueSkewX},
      {offsetof(struct stringpool_t, stringpool_str667), CSSValueWebkitImageSet},
      {offsetof(struct stringpool_t, stringpool_str668), CSSValueOptimizeSpeed},
      {offsetof(struct stringpool_t, stringpool_str669), CSSValueMediaEnterFullscreenButton},
      {offsetof(struct stringpool_t, stringpool_str670), CSSValuePanDown},
      {offsetof(struct stringpool_t, stringpool_str671), CSSValueXxSmall},
      {offsetof(struct stringpool_t, stringpool_str672), CSSValueCapsLockIndicator},
      {offsetof(struct stringpool_t, stringpool_str673), CSSValueBelow},
      {offsetof(struct stringpool_t, stringpool_str674), CSSValueThreedhighlight},
      {offsetof(struct stringpool_t, stringpool_str675), CSSValueProximity},
      {offsetof(struct stringpool_t, stringpool_str676), CSSValueVisiblePainted},
      {offsetof(struct stringpool_t, stringpool_str677), CSSValueWebkitGrab},
      {offsetof(struct stringpool_t, stringpool_str678), CSSValueDiscreteCapacityLevelIndicator},
      {offsetof(struct stringpool_t, stringpool_str679), CSSValueCheckbox},
      {offsetof(struct stringpool_t, stringpool_str680), CSSValueInternalVariableValue},
      {offsetof(struct stringpool_t, stringpool_str681), CSSValueBurlywood},
      {offsetof(struct stringpool_t, stringpool_str682), CSSValueWebkitBaselineMiddle},
      {offsetof(struct stringpool_t, stringpool_str683), CSSValueInternalMediaCastOffButton},
      {offsetof(struct stringpool_t, stringpool_str684), CSSValueBoundingBox},
      {offsetof(struct stringpool_t, stringpool_str685), CSSValueFloralwhite},
      {offsetof(struct stringpool_t, stringpool_str686), CSSValueCubicBezier},
      {offsetof(struct stringpool_t, stringpool_str687), CSSValueInterWord},
      {offsetof(struct stringpool_t, stringpool_str688), CSSValueSpaceBetween},
      {offsetof(struct stringpool_t, stringpool_str689), CSSValueWebkitIsolateOverride},
      {offsetof(struct stringpool_t, stringpool_str690), CSSValueDarkturquoise},
      {offsetof(struct stringpool_t, stringpool_str691), CSSValueLowerAlpha},
      {offsetof(struct stringpool_t, stringpool_str692), CSSValueLightslategrey},
      {offsetof(struct stringpool_t, stringpool_str693), CSSValueLightslategray},
      {offsetof(struct stringpool_t, stringpool_str694), CSSValueCornflowerblue},
      {offsetof(struct stringpool_t, stringpool_str695), CSSValuePlusLighter},
      {offsetof(struct stringpool_t, stringpool_str696), CSSValuePreWrap},
      {offsetof(struct stringpool_t, stringpool_str697), CSSValueMenutext},
      {offsetof(struct stringpool_t, stringpool_str698), CSSValueAlways},
      {offsetof(struct stringpool_t, stringpool_str699), CSSValuePlaintext},
      {offsetof(struct stringpool_t, stringpool_str700), CSSValueContinuousCapacityLevelIndicator},
      {offsetof(struct stringpool_t, stringpool_str701), CSSValueButtonshadow},
      {offsetof(struct stringpool_t, stringpool_str702), CSSValueWebkitActivelink},
      {offsetof(struct stringpool_t, stringpool_str703), CSSValueWebkitText},
      {offsetof(struct stringpool_t, stringpool_str704), CSSValueEwResize},
      {offsetof(struct stringpool_t, stringpool_str705), CSSValueWebkitSmallControl},
      {offsetof(struct stringpool_t, stringpool_str706), CSSValueVisibleFill},
      {offsetof(struct stringpool_t, stringpool_str707), CSSValueScrollPosition},
      {offsetof(struct stringpool_t, stringpool_str708), CSSValueLightskyblue},
      {offsetof(struct stringpool_t, stringpool_str709), CSSValueGreenyellow},
      {offsetof(struct stringpool_t, stringpool_str710), CSSValueExtraCondensed},
      {offsetof(struct stringpool_t, stringpool_str711), CSSValueCjkIdeographic},
      {offsetof(struct stringpool_t, stringpool_str712), CSSValueVisual},
      {offsetof(struct stringpool_t, stringpool_str713), CSSValuePaddingBox},
      {offsetof(struct stringpool_t, stringpool_str714), CSSValueNwseResize},
      {offsetof(struct stringpool_t, stringpool_str715), CSSValueVisibleStroke},
      {offsetof(struct stringpool_t, stringpool_str716), CSSValueInlineAxis},
      {offsetof(struct stringpool_t, stringpool_str717), CSSValueWebkitGrabbing},
      {offsetof(struct stringpool_t, stringpool_str718), CSSValueMediaCurrentTimeDisplay},
      {offsetof(struct stringpool_t, stringpool_str719), CSSValueWebkitFitContent},
      {offsetof(struct stringpool_t, stringpool_str720), CSSValueWebkitZoomIn},
      {offsetof(struct stringpool_t, stringpool_str721), CSSValueSliderthumbHorizontal},
      {offsetof(struct stringpool_t, stringpool_str722), CSSValueNoOpenQuote},
      {offsetof(struct stringpool_t, stringpool_str723), CSSValueInlineFlex},
      {offsetof(struct stringpool_t, stringpool_str724), CSSValuePushButton},
      {offsetof(struct stringpool_t, stringpool_str725), CSSValueIdeographic},
      {offsetof(struct stringpool_t, stringpool_str726), CSSValueWebkitRight},
      {offsetof(struct stringpool_t, stringpool_str727), CSSValueWebkitMaxContent},
      {offsetof(struct stringpool_t, stringpool_str728), CSSValueWebkitBox},
      {offsetof(struct stringpool_t, stringpool_str729), CSSValueWebkitBody},
      {offsetof(struct stringpool_t, stringpool_str730), CSSValueLemonchiffon},
      {offsetof(struct stringpool_t, stringpool_str731), CSSValueWebkitPictograph},
      {offsetof(struct stringpool_t, stringpool_str732), CSSValuePeachpuff},
      {offsetof(struct stringpool_t, stringpool_str733), CSSValueSandybrown},
      {offsetof(struct stringpool_t, stringpool_str734), CSSValueThreedshadow},
      {offsetof(struct stringpool_t, stringpool_str735), CSSValueSliderHorizontal},
      {offsetof(struct stringpool_t, stringpool_str736), CSSValueTableRowGroup},
      {offsetof(struct stringpool_t, stringpool_str737), CSSValueWebkitZoomOut},
      {offsetof(struct stringpool_t, stringpool_str738), CSSValueInternalExtendToZoom},
      {offsetof(struct stringpool_t, stringpool_str739), CSSValueWebkitFillAvailable},
      {offsetof(struct stringpool_t, stringpool_str740), CSSValueWebkitCrossFade},
      {offsetof(struct stringpool_t, stringpool_str741), CSSValueWebkitMatchParent},
      {offsetof(struct stringpool_t, stringpool_str742), CSSValueThreeddarkshadow},
      {offsetof(struct stringpool_t, stringpool_str743), CSSValueCjkHeavenlyStem},
      {offsetof(struct stringpool_t, stringpool_str744), CSSValueWindow},
      {offsetof(struct stringpool_t, stringpool_str745), CSSValueGhostwhite},
      {offsetof(struct stringpool_t, stringpool_str746), CSSValueWebkitFocusRingColor},
      {offsetof(struct stringpool_t, stringpool_str747), CSSValueMediaControlsFullscreenBackground},
      {offsetof(struct stringpool_t, stringpool_str748), CSSValueTextBeforeEdge},
      {offsetof(struct stringpool_t, stringpool_str749), CSSValueInternalActiveListBoxSelection},
      {offsetof(struct stringpool_t, stringpool_str750), CSSValueMediaOverlayPlayButton},
      {offsetof(struct stringpool_t, stringpool_str751), CSSValueLightyellow},
      {offsetof(struct stringpool_t, stringpool_str752), CSSValueOptimizeLegibility},
      {offsetof(struct stringpool_t, stringpool_str753), CSSValueNoDiscretionaryLigatures},
      {offsetof(struct stringpool_t, stringpool_str754), CSSValueVerticalRight},
      {offsetof(struct stringpool_t, stringpool_str755), CSSValueEthiopicHalehame},
      {offsetof(struct stringpool_t, stringpool_str756), CSSValueWebkitRepeatingLinearGradient},
      {offsetof(struct stringpool_t, stringpool_str757), CSSValueWebkitOptimizeContrast},
      {offsetof(struct stringpool_t, stringpool_str758), CSSValueMediaFullscreenVolumeSliderThumb},
      {offsetof(struct stringpool_t, stringpool_str759), CSSValueWebkitRepeatingRadialGradient},
      {offsetof(struct stringpool_t, stringpool_str760), CSSValueMenulistText},
      {offsetof(struct stringpool_t, stringpool_str761), CSSValuePowderblue},
      {offsetof(struct stringpool_t, stringpool_str762), CSSValueWebkitNowrap},
      {offsetof(struct stringpool_t, stringpool_str763), CSSValueEthiopicHalehameAm},
      {offsetof(struct stringpool_t, stringpool_str764), CSSValueVerticalText},
      {offsetof(struct stringpool_t, stringpool_str765), CSSValueHighlighttext},
      {offsetof(struct stringpool_t, stringpool_str766), CSSValueMediaExitFullscreenButton},
      {offsetof(struct stringpool_t, stringpool_str767), CSSValueEthiopicHalehameTiEt},
      {offsetof(struct stringpool_t, stringpool_str768), CSSValueExtraExpanded},
      {offsetof(struct stringpool_t, stringpool_str769), CSSValueEthiopicHalehameTiEr},
      {offsetof(struct stringpool_t, stringpool_str770), CSSValueWebkitPagedY},
      {offsetof(struct stringpool_t, stringpool_str771), CSSValueLightgoldenrodyellow},
      {offsetof(struct stringpool_t, stringpool_str772), CSSValuePaleturquoise},
      {offsetof(struct stringpool_t, stringpool_str773), CSSValueWebkitInlineBox},
      {offsetof(struct stringpool_t, stringpool_str774), CSSValueThreedlightshadow},
      {offsetof(struct stringpool_t, stringpool_str775), CSSValueInternalMediaOverlayCastOffButton},
      {offsetof(struct stringpool_t, stringpool_str776), CSSValueInactivecaptiontext},
      {offsetof(struct stringpool_t, stringpool_str777), CSSValueNavajowhite},
      {offsetof(struct stringpool_t, stringpool_str778), CSSValueWebkitPlaintext},
      {offsetof(struct stringpool_t, stringpool_str779), CSSValueCjkEarthlyBranch},
      {offsetof(struct stringpool_t, stringpool_str780), CSSValueOptimizeQuality},
      {offsetof(struct stringpool_t, stringpool_str781), CSSValueInfobackground},
      {offsetof(struct stringpool_t, stringpool_str782), CSSValueInfotext},
      {offsetof(struct stringpool_t, stringpool_str783), CSSValueWebkitInlineFlex},
      {offsetof(struct stringpool_t, stringpool_str784), CSSValueAntiquewhite},
      {offsetof(struct stringpool_t, stringpool_str785), CSSValueAfterWhiteSpace},
      {offsetof(struct stringpool_t, stringpool_str786), CSSValueInternalInactiveListBoxSelection},
      {offsetof(struct stringpool_t, stringpool_str787), CSSValueSidewaysRight},
      {offsetof(struct stringpool_t, stringpool_str788), CSSValueWebkitPagedX},
      {offsetof(struct stringpool_t, stringpool_str789), CSSValueMenulistTextfield},
      {offsetof(struct stringpool_t, stringpool_str790), CSSValueWebkitFlex},
      {offsetof(struct stringpool_t, stringpool_str791), CSSValueWindowframe},
      {offsetof(struct stringpool_t, stringpool_str792), CSSValuePapayawhip},
      {offsetof(struct stringpool_t, stringpool_str793), CSSValueWindowtext},
      {offsetof(struct stringpool_t, stringpool_str794), CSSValueInternalActiveListBoxSelectionText},
      {offsetof(struct stringpool_t, stringpool_str795), CSSValueReadWritePlaintextOnly},
      {offsetof(struct stringpool_t, stringpool_str796), CSSValueWebkitXxxLarge},
      {offsetof(struct stringpool_t, stringpool_str797), CSSValueInternalInactiveListBoxSelectionText}
    };

  static const short lookup[] =
    {
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,   0,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,   1,   2,   3,   4,   5,   6,   7,   8,   9,
       10,  11,  -1,  -1,  12,  -1,  -1,  -1,  13,  -1,
       -1,  -1,  -1,  -1,  14,  15,  16,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  17,  -1,  18,  19,  20,  -1,
       -1,  21,  -1,  22,  -1,  -1,  -1,  -1,  23,  -1,
       -1,  24,  -1,  25,  26,  27,  -1,  28,  -1,  -1,
       29,  -1,  -1,  -1,  30,  -1,  31,  32,  -1,  33,
       34,  35,  36,  -1,  -1,  -1,  -1,  -1,  37,  -1,
       38,  -1,  39,  -1,  -1,  -1,  -1,  40,  41,  -1,
       42,  43,  -1,  -1,  -1,  -1,  44,  45,  46,  -1,
       47,  48,  49,  50,  -1,  51,  -1,  -1,  -1,  -1,
       -1,  52,  -1,  -1,  -1,  53,  54,  55,  56,  -1,
       57,  58,  59,  -1,  60,  -1,  61,  62,  63,  64,
       -1,  -1,  -1,  -1,  -1,  -1,  65,  66,  67,  68,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  69,  -1,
       70,  71,  -1,  -1,  72,  -1,  -1,  -1,  73,  74,
       75,  -1,  -1,  -1,  76,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  77,  -1,  -1,  -1,  -1,  -1,  -1,
       78,  79,  80,  81,  82,  -1,  -1,  -1,  83,  -1,
       -1,  -1,  84,  -1,  85,  86,  -1,  87,  88,  -1,
       89,  -1,  90,  91,  92,  93,  -1,  -1,  94,  -1,
       95,  96,  97,  98,  -1,  -1,  -1,  99,  -1,  -1,
       -1,  -1, 100, 101,  -1,  -1,  -1,  -1,  -1,  -1,
      102, 103,  -1,  -1,  -1,  -1,  -1, 104,  -1, 105,
       -1,  -1, 106,  -1, 107, 108, 109, 110,  -1, 111,
       -1, 112,  -1, 113,  -1,  -1,  -1,  -1,  -1,  -1,
      114,  -1,  -1,  -1, 115,  -1,  -1,  -1, 116, 117,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 118, 119,
      120,  -1,  -1, 121,  -1,  -1, 122, 123,  -1,  -1,
       -1, 124,  -1,  -1, 125,  -1,  -1, 126,  -1,  -1,
       -1,  -1,  -1, 127,  -1,  -1,  -1,  -1,  -1,  -1,
      128, 129,  -1, 130, 131,  -1,  -1, 132, 133, 134,
      135,  -1,  -1,  -1,  -1, 136,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 137, 138,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 139,  -1,  -1,  -1,  -1,
       -1, 140,  -1,  -1, 141,  -1, 142,  -1,  -1,  -1,
       -1, 143,  -1, 144,  -1,  -1,  -1, 145,  -1,  -1,
       -1, 146,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 147,
       -1,  -1,  -1,  -1, 148,  -1,  -1,  -1,  -1,  -1,
       -1, 149,  -1,  -1, 150,  -1,  -1, 151,  -1,  -1,
       -1,  -1,  -1, 152,  -1,  -1, 153,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 154, 155, 156,  -1,
      157, 158, 159,  -1,  -1,  -1, 160,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 161,  -1,  -1, 162,
      163, 164, 165,  -1,  -1,  -1, 166,  -1,  -1, 167,
       -1,  -1,  -1, 168,  -1,  -1,  -1,  -1, 169,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 170,  -1,  -1,  -1,
      171, 172,  -1,  -1,  -1, 173,  -1,  -1,  -1,  -1,
       -1, 174,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      175,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 176, 177,
       -1,  -1, 178,  -1,  -1, 179,  -1,  -1, 180,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 181, 182,  -1,
       -1, 183,  -1, 184,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 185,  -1, 186, 187,  -1,  -1, 188,
       -1, 189,  -1,  -1,  -1,  -1,  -1, 190,  -1,  -1,
       -1, 191, 192,  -1,  -1,  -1, 193,  -1,  -1,  -1,
       -1, 194,  -1, 195,  -1,  -1, 196,  -1, 197,  -1,
      198,  -1, 199,  -1,  -1,  -1,  -1,  -1,  -1, 200,
       -1,  -1,  -1, 201,  -1, 202,  -1,  -1, 203,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 204, 205,  -1,  -1, 206,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 207, 208, 209,  -1,  -1,
      210,  -1,  -1,  -1,  -1,  -1, 211,  -1,  -1,  -1,
       -1, 212,  -1,  -1,  -1, 213,  -1, 214, 215, 216,
       -1,  -1,  -1, 217, 218,  -1,  -1, 219,  -1,  -1,
       -1, 220,  -1,  -1,  -1, 221,  -1,  -1, 222,  -1,
       -1,  -1,  -1, 223, 224,  -1, 225,  -1, 226,  -1,
       -1, 227,  -1,  -1, 228,  -1, 229, 230,  -1, 231,
      232, 233, 234,  -1, 235,  -1,  -1,  -1,  -1, 236,
       -1, 237,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 238,
       -1, 239,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      240,  -1,  -1, 241, 242,  -1,  -1, 243, 244,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 245,  -1,  -1,
      246,  -1,  -1, 247,  -1,  -1, 248, 249,  -1, 250,
      251, 252,  -1, 253,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 254,  -1,  -1,  -1, 255,  -1,
       -1, 256,  -1, 257,  -1,  -1,  -1, 258,  -1, 259,
       -1,  -1,  -1,  -1,  -1,  -1, 260,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 261, 262,  -1, 263,  -1, 264,
       -1,  -1, 265,  -1,  -1, 266, 267,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 268,  -1, 269,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 270,  -1,  -1,  -1,
      271,  -1,  -1, 272, 273, 274,  -1,  -1,  -1,  -1,
      275,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 276,  -1,
       -1,  -1, 277,  -1,  -1,  -1, 278, 279,  -1,  -1,
       -1,  -1,  -1, 280,  -1, 281, 282, 283,  -1,  -1,
       -1, 284, 285,  -1,  -1,  -1,  -1,  -1, 286,  -1,
       -1, 287,  -1, 288,  -1, 289, 290,  -1, 291,  -1,
       -1,  -1,  -1,  -1, 292, 293,  -1,  -1,  -1,  -1,
      294,  -1,  -1,  -1,  -1,  -1,  -1, 295,  -1, 296,
      297,  -1,  -1, 298,  -1,  -1,  -1, 299, 300, 301,
       -1, 302,  -1,  -1, 303, 304,  -1,  -1, 305, 306,
      307,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 308,
      309,  -1,  -1,  -1, 310,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 311,  -1,  -1,  -1,  -1,  -1, 312, 313,
       -1,  -1, 314,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 315, 316,
      317,  -1, 318, 319,  -1,  -1,  -1,  -1, 320,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 321,  -1,  -1,  -1,
       -1, 322,  -1,  -1,  -1,  -1,  -1,  -1, 323, 324,
       -1,  -1, 325,  -1,  -1, 326,  -1, 327,  -1,  -1,
       -1,  -1, 328,  -1,  -1,  -1,  -1, 329,  -1, 330,
       -1,  -1,  -1,  -1,  -1,  -1, 331,  -1,  -1, 332,
       -1,  -1, 333,  -1,  -1,  -1,  -1,  -1,  -1, 334,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      335,  -1,  -1,  -1,  -1,  -1, 336, 337,  -1,  -1,
      338,  -1,  -1,  -1,  -1, 339,  -1, 340,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 341,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 342,
      343,  -1,  -1, 344,  -1, 345, 346,  -1, 347,  -1,
       -1,  -1, 348, 349,  -1,  -1, 350, 351,  -1,  -1,
       -1,  -1, 352, 353,  -1,  -1, 354,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 355,  -1, 356,  -1,  -1,
       -1, 357,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 358, 359,  -1,  -1,  -1,
      360, 361,  -1,  -1,  -1,  -1,  -1,  -1, 362,  -1,
       -1, 363,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 364,  -1, 365,  -1, 366,  -1, 367,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 368,  -1,  -1,  -1,  -1,
      369, 370, 371,  -1, 372,  -1,  -1,  -1, 373, 374,
      375,  -1,  -1, 376,  -1,  -1,  -1,  -1,  -1,  -1,
      377, 378,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 379,  -1,  -1,  -1, 380,  -1,  -1,  -1, 381,
       -1, 382, 383,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 384,  -1, 385,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 386,  -1,
       -1,  -1, 387,  -1,  -1, 388,  -1,  -1, 389,  -1,
       -1,  -1, 390, 391,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 392,  -1,
       -1, 393,  -1, 394,  -1, 395, 396, 397,  -1, 398,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 399,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 400,  -1,
      401,  -1,  -1,  -1,  -1, 402, 403,  -1,  -1,  -1,
      404, 405,  -1, 406,  -1,  -1, 407,  -1, 408,  -1,
      409,  -1, 410,  -1,  -1,  -1,  -1,  -1,  -1, 411,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 412,  -1,  -1, 413,  -1,
      414,  -1,  -1, 415, 416, 417, 418,  -1, 419,  -1,
       -1,  -1, 420, 421,  -1, 422,  -1,  -1,  -1,  -1,
       -1, 423, 424,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 425,  -1, 426,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 427, 428,  -1,  -1, 429,  -1, 430, 431,
       -1,  -1, 432,  -1, 433,  -1, 434, 435, 436,  -1,
      437, 438, 439,  -1,  -1,  -1,  -1,  -1, 440,  -1,
      441,  -1,  -1,  -1, 442,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 443,  -1,  -1, 444,  -1,
      445,  -1,  -1,  -1,  -1,  -1, 446,  -1, 447,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 448,  -1,  -1, 449,
       -1,  -1,  -1,  -1, 450,  -1, 451,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 452,
      453,  -1,  -1, 454, 455,  -1, 456,  -1,  -1,  -1,
       -1, 457,  -1, 458,  -1,  -1,  -1,  -1,  -1, 459,
       -1,  -1, 460,  -1, 461,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 462, 463,  -1,  -1, 464,  -1, 465,  -1, 466,
       -1,  -1, 467,  -1,  -1,  -1,  -1, 468, 469,  -1,
       -1,  -1, 470,  -1,  -1,  -1,  -1,  -1, 471, 472,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 473,  -1,  -1,
      474,  -1, 475,  -1,  -1,  -1, 476,  -1,  -1, 477,
       -1,  -1,  -1, 478,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 479,  -1,  -1,  -1,  -1,  -1, 480,
       -1,  -1, 481, 482,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 483,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 484, 485,  -1,  -1,
       -1,  -1,  -1, 486,  -1,  -1,  -1,  -1, 487,  -1,
      488,  -1,  -1,  -1,  -1,  -1, 489, 490,  -1,  -1,
      491,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 492,
       -1,  -1,  -1, 493, 494,  -1, 495, 496,  -1,  -1,
       -1,  -1, 497,  -1,  -1,  -1,  -1,  -1,  -1, 498,
       -1, 499,  -1,  -1,  -1, 500, 501,  -1, 502,  -1,
       -1,  -1,  -1,  -1,  -1, 503,  -1,  -1, 504,  -1,
       -1,  -1,  -1, 505, 506, 507,  -1,  -1,  -1,  -1,
       -1,  -1, 508, 509,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 510, 511, 512, 513,  -1,
       -1, 514,  -1, 515,  -1,  -1,  -1,  -1,  -1,  -1,
      516,  -1,  -1,  -1, 517, 518, 519,  -1,  -1,  -1,
       -1,  -1, 520,  -1, 521,  -1, 522, 523,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 524,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      525,  -1,  -1, 526,  -1,  -1,  -1,  -1, 527, 528,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 529,  -1,  -1,
       -1, 530,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 531,  -1,  -1,
      532, 533,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 534,
       -1, 535,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 536,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 537,  -1,  -1,  -1,  -1, 538,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 539,  -1,  -1,
       -1,  -1, 540,  -1,  -1,  -1,  -1,  -1,  -1, 541,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 542,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 543,  -1,  -1,  -1,
       -1,  -1, 544,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 545,  -1,  -1,  -1, 546,  -1, 547,  -1,
      548,  -1, 549,  -1,  -1, 550,  -1,  -1,  -1,  -1,
      551,  -1, 552,  -1,  -1,  -1, 553,  -1, 554,  -1,
       -1,  -1,  -1, 555,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 556,  -1,  -1, 557, 558,  -1,
       -1,  -1,  -1,  -1, 559,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 560,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 561,  -1,
       -1,  -1,  -1, 562,  -1,  -1,  -1, 563,  -1,  -1,
      564,  -1,  -1, 565, 566,  -1,  -1,  -1, 567,  -1,
       -1, 568,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      569,  -1, 570,  -1, 571,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 572, 573,  -1,  -1, 574, 575,
       -1,  -1,  -1, 576, 577,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 578,  -1,  -1, 579,  -1,  -1,  -1,
       -1, 580,  -1,  -1,  -1, 581,  -1,  -1, 582,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 583,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 584,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 585,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 586, 587,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 588,  -1,  -1, 589,  -1,  -1,
      590,  -1,  -1,  -1,  -1, 591,  -1,  -1,  -1, 592,
       -1, 593,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      594,  -1, 595,  -1, 596,  -1, 597,  -1,  -1,  -1,
       -1, 598,  -1, 599,  -1,  -1,  -1, 600,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 601,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 602,  -1,  -1, 603,
       -1, 604,  -1, 605, 606,  -1, 607,  -1,  -1,  -1,
      608, 609, 610,  -1,  -1,  -1,  -1, 611,  -1,  -1,
       -1,  -1,  -1, 612,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 613,  -1,  -1,  -1, 614,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 615,  -1,  -1, 616,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 617,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 618,
       -1,  -1, 619,  -1, 620,  -1, 621,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 622,  -1, 623,  -1, 624,  -1,
       -1,  -1,  -1, 625,  -1, 626,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 627,  -1,  -1,  -1,  -1,  -1, 628,
       -1,  -1, 629,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 630,  -1,  -1, 631, 632,  -1,  -1,  -1,
      633, 634,  -1, 635,  -1,  -1,  -1,  -1,  -1, 636,
      637, 638,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 639,
       -1, 640,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 641,  -1,
      642, 643, 644,  -1,  -1,  -1, 645,  -1,  -1,  -1,
      646,  -1,  -1,  -1,  -1,  -1, 647,  -1,  -1,  -1,
       -1, 648,  -1,  -1,  -1,  -1,  -1, 649,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 650, 651,  -1,  -1,
       -1,  -1, 652,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 653,  -1, 654, 655, 656, 657, 658,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 659,  -1, 660,  -1,  -1,  -1,  -1, 661,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 662, 663,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 664,  -1,
       -1,  -1,  -1,  -1, 665,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      666,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 667,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 668,  -1,  -1,  -1,  -1,  -1,
       -1, 669,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 670,  -1,
      671,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 672,
       -1,  -1,  -1,  -1,  -1, 673,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 674,  -1,  -1,  -1,
       -1,  -1, 675,  -1,  -1,  -1, 676,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 677,  -1,  -1,  -1,
      678,  -1,  -1,  -1,  -1,  -1,  -1, 679,  -1,  -1,
       -1, 680,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 681,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 682,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 683,  -1,  -1,  -1, 684, 685,
       -1,  -1, 686,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 687,  -1,  -1, 688,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 689,  -1,  -1,  -1,  -1,  -1,  -1, 690,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 691,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 692, 693,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 694,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 695, 696,  -1,
       -1, 697,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 698,  -1, 699,  -1, 700,  -1,  -1,  -1,
      701,  -1,  -1,  -1,  -1, 702,  -1, 703,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 704, 705,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      706,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 707,  -1,  -1,  -1,  -1, 708,  -1,  -1, 709,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      710,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 711,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 712,  -1,  -1,
       -1,  -1,  -1, 713,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 714,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 715,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 716,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      717,  -1, 718,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 719,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 720,  -1,  -1, 721,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 722,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 723,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 724,  -1, 725,  -1,
       -1, 726,  -1,  -1,  -1,  -1, 727,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 728,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 729,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 730,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 731,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 732, 733,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 734,  -1,  -1,
      735,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 736,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 737,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 738,  -1, 739,
       -1, 740,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 741,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 742,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 743, 744,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 745,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 746,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 747,  -1, 748,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 749,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      750, 751,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      752,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 753,  -1,  -1,  -1,  -1,  -1,  -1, 754,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 755,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 756,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 757,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 758,  -1,  -1,
      759,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 760,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 761, 762,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      763,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 764, 765,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 766,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 767,  -1,  -1, 768,
      769,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 770,
       -1,  -1,  -1, 771,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 772,  -1,  -1,  -1,  -1,  -1,
      773,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      774,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 775,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 776,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1, 777,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 778,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 779,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1, 780,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 781,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      782,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1, 783,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 784,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      785,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 786,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1, 787,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      788,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      789,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 790,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 791,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      792,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 793,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1, 794,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1, 795,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
      796,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
       -1,  -1,  -1, 797
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = value_hash_function (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = value_word_list[index].nameOffset + stringpool;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &value_word_list[index];
            }
        }
    }
  return 0;
}

const Value* findValue(register const char* str, register unsigned int len)
{
    return CSSValueKeywordsHash::findValueImpl(str, len);
}

const char* getValueName(CSSValueID id)
{
    ASSERT(id > 0 && id < numCSSValueKeywords);
    return valueListStringPool + valueListStringOffsets[id - 1];
}

bool isValueAllowedInMode(unsigned short id, CSSParserMode mode)
{
    switch (id) {
        case CSSValueInternalActiveListBoxSelection:
        case CSSValueInternalActiveListBoxSelectionText:
        case CSSValueInternalInactiveListBoxSelection:
        case CSSValueInternalInactiveListBoxSelectionText:
        case CSSValueInternalMediaCastOffButton:
        case CSSValueInternalMediaOverlayCastOffButton:
        case CSSValueInternalExtendToZoom:
        case CSSValueInternalVariableValue:
            return isUASheetBehavior(mode);
        case CSSValueWebkitFocusRingColor:
    case CSSValueWebkitText:
            return isUASheetBehavior(mode) || isQuirksModeBehavior(mode);
        default:
            return true;
    }
}

} // namespace blink

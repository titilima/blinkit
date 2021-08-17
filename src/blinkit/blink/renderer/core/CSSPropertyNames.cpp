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

#include "CSSPropertyNames.h"
#include "core/css/HashTools.h"
#include <string.h>

#include "wtf/ASCIICType.h"
#include "wtf/text/AtomicString.h"
#include "wtf/text/WTFString.h"

#ifdef _MSC_VER
// Disable the warnings from casting a 64-bit pointer to 32-bit long
// warning C4302: 'type cast': truncation from 'char (*)[28]' to 'long'
// warning C4311: 'type cast': pointer truncation from 'char (*)[18]' to 'long'
#pragma warning(disable : 4302 4311)
#endif

namespace blink {
static const char propertyNameStringsPool[] = {
    "color\0"
    "direction\0"
    "font-family\0"
    "font-kerning\0"
    "font-size\0"
    "font-size-adjust\0"
    "font-stretch\0"
    "font-style\0"
    "font-variant\0"
    "font-variant-ligatures\0"
    "font-weight\0"
    "font-feature-settings\0"
    "-webkit-font-smoothing\0"
    "-webkit-locale\0"
    "text-orientation\0"
    "-webkit-text-orientation\0"
    "writing-mode\0"
    "-webkit-writing-mode\0"
    "text-rendering\0"
    "zoom\0"
    "align-content\0"
    "align-items\0"
    "align-self\0"
    "animation-delay\0"
    "animation-direction\0"
    "animation-duration\0"
    "animation-fill-mode\0"
    "animation-iteration-count\0"
    "animation-name\0"
    "animation-play-state\0"
    "animation-timing-function\0"
    "backdrop-filter\0"
    "backface-visibility\0"
    "background-attachment\0"
    "background-blend-mode\0"
    "background-clip\0"
    "background-color\0"
    "background-image\0"
    "background-origin\0"
    "background-position-x\0"
    "background-position-y\0"
    "background-repeat-x\0"
    "background-repeat-y\0"
    "background-size\0"
    "border-bottom-color\0"
    "border-bottom-left-radius\0"
    "border-bottom-right-radius\0"
    "border-bottom-style\0"
    "border-bottom-width\0"
    "border-collapse\0"
    "border-image-outset\0"
    "border-image-repeat\0"
    "border-image-slice\0"
    "border-image-source\0"
    "border-image-width\0"
    "border-left-color\0"
    "border-left-style\0"
    "border-left-width\0"
    "border-right-color\0"
    "border-right-style\0"
    "border-right-width\0"
    "border-top-color\0"
    "border-top-left-radius\0"
    "border-top-right-radius\0"
    "border-top-style\0"
    "border-top-width\0"
    "bottom\0"
    "box-shadow\0"
    "box-sizing\0"
    "caption-side\0"
    "clear\0"
    "clip\0"
    "column-fill\0"
    "contain\0"
    "content\0"
    "counter-increment\0"
    "counter-reset\0"
    "cursor\0"
    "display\0"
    "empty-cells\0"
    "flex-basis\0"
    "flex-direction\0"
    "flex-grow\0"
    "flex-shrink\0"
    "flex-wrap\0"
    "float\0"
    "grid-auto-columns\0"
    "grid-auto-flow\0"
    "grid-auto-rows\0"
    "grid-column-end\0"
    "grid-column-gap\0"
    "grid-column-start\0"
    "grid-row-end\0"
    "grid-row-gap\0"
    "grid-row-start\0"
    "grid-template-areas\0"
    "grid-template-columns\0"
    "grid-template-rows\0"
    "height\0"
    "image-rendering\0"
    "image-orientation\0"
    "isolation\0"
    "justify-content\0"
    "justify-items\0"
    "justify-self\0"
    "left\0"
    "letter-spacing\0"
    "line-height\0"
    "list-style-image\0"
    "list-style-position\0"
    "list-style-type\0"
    "margin-bottom\0"
    "margin-left\0"
    "margin-right\0"
    "margin-top\0"
    "mask-source-type\0"
    "max-height\0"
    "max-width\0"
    "min-height\0"
    "min-width\0"
    "mix-blend-mode\0"
    "motion-offset\0"
    "motion-path\0"
    "motion-rotation\0"
    "object-fit\0"
    "object-position\0"
    "opacity\0"
    "order\0"
    "orphans\0"
    "outline-color\0"
    "outline-offset\0"
    "outline-style\0"
    "outline-width\0"
    "overflow-wrap\0"
    "overflow-x\0"
    "overflow-y\0"
    "padding-bottom\0"
    "padding-left\0"
    "padding-right\0"
    "padding-top\0"
    "page-break-after\0"
    "page-break-before\0"
    "page-break-inside\0"
    "perspective\0"
    "perspective-origin\0"
    "pointer-events\0"
    "position\0"
    "quotes\0"
    "resize\0"
    "right\0"
    "scroll-behavior\0"
    "scroll-snap-type\0"
    "scroll-snap-points-x\0"
    "scroll-snap-points-y\0"
    "scroll-snap-destination\0"
    "scroll-snap-coordinate\0"
    "shape-image-threshold\0"
    "shape-margin\0"
    "shape-outside\0"
    "size\0"
    "speak\0"
    "table-layout\0"
    "tab-size\0"
    "text-align\0"
    "text-align-last\0"
    "text-combine-upright\0"
    "text-decoration\0"
    "text-decoration-color\0"
    "text-decoration-line\0"
    "text-decoration-style\0"
    "text-indent\0"
    "text-justify\0"
    "text-overflow\0"
    "text-shadow\0"
    "text-transform\0"
    "text-underline-position\0"
    "top\0"
    "touch-action\0"
    "transform\0"
    "transform-origin\0"
    "transform-style\0"
    "translate\0"
    "rotate\0"
    "scale\0"
    "transition-delay\0"
    "transition-duration\0"
    "transition-property\0"
    "transition-timing-function\0"
    "unicode-bidi\0"
    "vertical-align\0"
    "visibility\0"
    "-webkit-appearance\0"
    "-webkit-app-region\0"
    "-webkit-background-clip\0"
    "-webkit-background-composite\0"
    "-webkit-background-origin\0"
    "-webkit-border-horizontal-spacing\0"
    "-webkit-border-image\0"
    "-webkit-border-vertical-spacing\0"
    "-webkit-box-align\0"
    "-webkit-box-decoration-break\0"
    "-webkit-box-direction\0"
    "-webkit-box-flex\0"
    "-webkit-box-flex-group\0"
    "-webkit-box-lines\0"
    "-webkit-box-ordinal-group\0"
    "-webkit-box-orient\0"
    "-webkit-box-pack\0"
    "-webkit-box-reflect\0"
    "-webkit-clip-path\0"
    "-webkit-column-break-after\0"
    "-webkit-column-break-before\0"
    "-webkit-column-break-inside\0"
    "-webkit-column-count\0"
    "-webkit-column-gap\0"
    "-webkit-column-rule-color\0"
    "-webkit-column-rule-style\0"
    "-webkit-column-rule-width\0"
    "-webkit-column-span\0"
    "-webkit-column-width\0"
    "-webkit-filter\0"
    "-webkit-highlight\0"
    "-webkit-hyphenate-character\0"
    "-webkit-line-break\0"
    "-webkit-line-clamp\0"
    "-webkit-margin-after-collapse\0"
    "-webkit-margin-before-collapse\0"
    "-webkit-margin-bottom-collapse\0"
    "-webkit-margin-top-collapse\0"
    "-webkit-mask-box-image-outset\0"
    "-webkit-mask-box-image-repeat\0"
    "-webkit-mask-box-image-slice\0"
    "-webkit-mask-box-image-source\0"
    "-webkit-mask-box-image-width\0"
    "-webkit-mask-clip\0"
    "-webkit-mask-composite\0"
    "-webkit-mask-image\0"
    "-webkit-mask-origin\0"
    "-webkit-mask-position-x\0"
    "-webkit-mask-position-y\0"
    "-webkit-mask-repeat-x\0"
    "-webkit-mask-repeat-y\0"
    "-webkit-mask-size\0"
    "-webkit-perspective-origin-x\0"
    "-webkit-perspective-origin-y\0"
    "-webkit-print-color-adjust\0"
    "-webkit-rtl-ordering\0"
    "-webkit-ruby-position\0"
    "-webkit-tap-highlight-color\0"
    "-webkit-text-combine\0"
    "-webkit-text-emphasis-color\0"
    "-webkit-text-emphasis-position\0"
    "-webkit-text-emphasis-style\0"
    "-webkit-text-fill-color\0"
    "-webkit-text-security\0"
    "-webkit-text-stroke-color\0"
    "-webkit-text-stroke-width\0"
    "-webkit-transform-origin-x\0"
    "-webkit-transform-origin-y\0"
    "-webkit-transform-origin-z\0"
    "-webkit-user-drag\0"
    "-webkit-user-modify\0"
    "-webkit-user-select\0"
    "white-space\0"
    "widows\0"
    "width\0"
    "will-change\0"
    "word-break\0"
    "word-spacing\0"
    "word-wrap\0"
    "z-index\0"
    "-webkit-border-end-color\0"
    "-webkit-border-end-style\0"
    "-webkit-border-end-width\0"
    "-webkit-border-start-color\0"
    "-webkit-border-start-style\0"
    "-webkit-border-start-width\0"
    "-webkit-border-before-color\0"
    "-webkit-border-before-style\0"
    "-webkit-border-before-width\0"
    "-webkit-border-after-color\0"
    "-webkit-border-after-style\0"
    "-webkit-border-after-width\0"
    "-webkit-margin-end\0"
    "-webkit-margin-start\0"
    "-webkit-margin-before\0"
    "-webkit-margin-after\0"
    "-webkit-padding-end\0"
    "-webkit-padding-start\0"
    "-webkit-padding-before\0"
    "-webkit-padding-after\0"
    "-webkit-logical-width\0"
    "-webkit-logical-height\0"
    "-webkit-min-logical-width\0"
    "-webkit-min-logical-height\0"
    "-webkit-max-logical-width\0"
    "-webkit-max-logical-height\0"
    "all\0"
    "font-display\0"
    "max-zoom\0"
    "min-zoom\0"
    "orientation\0"
    "page\0"
    "src\0"
    "unicode-range\0"
    "user-zoom\0"
    "-webkit-font-size-delta\0"
    "-webkit-text-decorations-in-effect\0"
    "animation\0"
    "background\0"
    "background-position\0"
    "background-repeat\0"
    "border\0"
    "border-bottom\0"
    "border-color\0"
    "border-image\0"
    "border-left\0"
    "border-radius\0"
    "border-right\0"
    "border-spacing\0"
    "border-style\0"
    "border-top\0"
    "border-width\0"
    "flex\0"
    "flex-flow\0"
    "font\0"
    "grid\0"
    "grid-area\0"
    "grid-column\0"
    "grid-gap\0"
    "grid-row\0"
    "grid-template\0"
    "list-style\0"
    "margin\0"
    "motion\0"
    "outline\0"
    "overflow\0"
    "padding\0"
    "transition\0"
    "-webkit-border-after\0"
    "-webkit-border-before\0"
    "-webkit-border-end\0"
    "-webkit-border-start\0"
    "-webkit-column-rule\0"
    "-webkit-columns\0"
    "-webkit-margin-collapse\0"
    "-webkit-mask\0"
    "-webkit-mask-box-image\0"
    "-webkit-mask-position\0"
    "-webkit-mask-repeat\0"
    "-webkit-text-emphasis\0"
    "-webkit-text-stroke\0"
    "-webkit-font-feature-settings\0"
    "-epub-text-orientation\0"
    "-epub-writing-mode\0"
    "-webkit-align-content\0"
    "-webkit-align-items\0"
    "-webkit-align-self\0"
    "-webkit-animation-delay\0"
    "-webkit-animation-direction\0"
    "-webkit-animation-duration\0"
    "-webkit-animation-fill-mode\0"
    "-webkit-animation-iteration-count\0"
    "-webkit-animation-name\0"
    "-webkit-animation-play-state\0"
    "-webkit-animation-timing-function\0"
    "-webkit-backface-visibility\0"
    "-webkit-background-size\0"
    "-webkit-border-bottom-left-radius\0"
    "-webkit-border-bottom-right-radius\0"
    "-webkit-border-top-left-radius\0"
    "-webkit-border-top-right-radius\0"
    "-webkit-box-shadow\0"
    "-webkit-box-sizing\0"
    "-epub-caption-side\0"
    "-webkit-flex-basis\0"
    "-webkit-flex-direction\0"
    "-webkit-flex-grow\0"
    "-webkit-flex-shrink\0"
    "-webkit-flex-wrap\0"
    "-webkit-justify-content\0"
    "-webkit-opacity\0"
    "-webkit-order\0"
    "-webkit-perspective\0"
    "-webkit-perspective-origin\0"
    "-webkit-shape-image-threshold\0"
    "-webkit-shape-margin\0"
    "-webkit-shape-outside\0"
    "-epub-text-transform\0"
    "-webkit-transform\0"
    "-webkit-transform-origin\0"
    "-webkit-transform-style\0"
    "-webkit-transition-delay\0"
    "-webkit-transition-duration\0"
    "-webkit-transition-property\0"
    "-webkit-transition-timing-function\0"
    "-epub-text-combine\0"
    "-epub-text-emphasis-color\0"
    "-epub-text-emphasis-style\0"
    "-epub-word-break\0"
    "-webkit-animation\0"
    "-webkit-border-radius\0"
    "-webkit-flex\0"
    "-webkit-flex-flow\0"
    "-webkit-transition\0"
    "-epub-text-emphasis\0"
};

static const unsigned short propertyNameStringsOffsets[] = {
    0,
    6,
    16,
    28,
    41,
    51,
    68,
    81,
    92,
    105,
    128,
    140,
    162,
    185,
    200,
    217,
    242,
    255,
    276,
    291,
    296,
    310,
    322,
    333,
    349,
    369,
    388,
    408,
    434,
    449,
    470,
    496,
    512,
    532,
    554,
    576,
    592,
    609,
    626,
    644,
    666,
    688,
    708,
    728,
    744,
    764,
    790,
    817,
    837,
    857,
    873,
    893,
    913,
    932,
    952,
    971,
    989,
    1007,
    1025,
    1044,
    1063,
    1082,
    1099,
    1122,
    1146,
    1163,
    1180,
    1187,
    1198,
    1209,
    1222,
    1228,
    1233,
    1245,
    1253,
    1261,
    1279,
    1293,
    1300,
    1308,
    1320,
    1331,
    1346,
    1356,
    1368,
    1378,
    1384,
    1402,
    1417,
    1432,
    1448,
    1464,
    1482,
    1495,
    1508,
    1523,
    1543,
    1565,
    1584,
    1591,
    1607,
    1625,
    1635,
    1651,
    1665,
    1678,
    1683,
    1698,
    1710,
    1727,
    1747,
    1763,
    1777,
    1789,
    1802,
    1813,
    1830,
    1841,
    1851,
    1862,
    1872,
    1887,
    1901,
    1913,
    1929,
    1940,
    1956,
    1964,
    1970,
    1978,
    1992,
    2007,
    2021,
    2035,
    2049,
    2060,
    2071,
    2086,
    2099,
    2113,
    2125,
    2142,
    2160,
    2178,
    2190,
    2209,
    2224,
    2233,
    2240,
    2247,
    2253,
    2269,
    2286,
    2307,
    2328,
    2352,
    2375,
    2397,
    2410,
    2424,
    2429,
    2435,
    2448,
    2457,
    2468,
    2484,
    2505,
    2521,
    2543,
    2564,
    2586,
    2598,
    2611,
    2625,
    2637,
    2652,
    2676,
    2680,
    2693,
    2703,
    2720,
    2736,
    2746,
    2753,
    2759,
    2776,
    2796,
    2816,
    2843,
    2856,
    2871,
    2882,
    2901,
    2920,
    2944,
    2973,
    2999,
    3033,
    3054,
    3086,
    3104,
    3133,
    3155,
    3172,
    3195,
    3213,
    3239,
    3258,
    3275,
    3295,
    3313,
    3340,
    3368,
    3396,
    3417,
    3436,
    3462,
    3488,
    3514,
    3534,
    3555,
    3570,
    3588,
    3616,
    3635,
    3654,
    3684,
    3715,
    3746,
    3774,
    3804,
    3834,
    3863,
    3893,
    3922,
    3940,
    3963,
    3982,
    4002,
    4026,
    4050,
    4072,
    4094,
    4112,
    4141,
    4170,
    4197,
    4218,
    4240,
    4268,
    4289,
    4317,
    4348,
    4376,
    4400,
    4422,
    4448,
    4474,
    4501,
    4528,
    4555,
    4573,
    4593,
    4613,
    4625,
    4632,
    4638,
    4650,
    4661,
    4674,
    4684,
    4692,
    4717,
    4742,
    4767,
    4794,
    4821,
    4848,
    4876,
    4904,
    4932,
    4959,
    4986,
    5013,
    5032,
    5053,
    5075,
    5096,
    5116,
    5138,
    5161,
    5183,
    5205,
    5228,
    5254,
    5281,
    5307,
    5334,
    5338,
    5351,
    5360,
    5369,
    5381,
    5386,
    5390,
    5404,
    5414,
    5438,
    5473,
    5483,
    5494,
    5514,
    5532,
    5539,
    5553,
    5566,
    5579,
    5591,
    5605,
    5618,
    5633,
    5646,
    5657,
    5670,
    5675,
    5685,
    5690,
    5695,
    5705,
    5717,
    5726,
    5735,
    5749,
    5760,
    5767,
    5774,
    5782,
    5791,
    5799,
    5810,
    5831,
    5853,
    5872,
    5893,
    5913,
    5929,
    5953,
    5966,
    5989,
    6011,
    6031,
    6053,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6073,
    6103,
    6103,
    6103,
    6103,
    6126,
    6126,
    6145,
    6145,
    6145,
    6167,
    6187,
    6206,
    6230,
    6258,
    6285,
    6313,
    6347,
    6370,
    6399,
    6433,
    6433,
    6461,
    6461,
    6461,
    6461,
    6461,
    6461,
    6461,
    6461,
    6461,
    6461,
    6461,
    6485,
    6485,
    6519,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6554,
    6585,
    6617,
    6617,
    6617,
    6617,
    6636,
    6655,
    6674,
    6674,
    6674,
    6674,
    6674,
    6674,
    6674,
    6674,
    6674,
    6674,
    6674,
    6693,
    6716,
    6734,
    6754,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6772,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6796,
    6812,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6826,
    6846,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6873,
    6903,
    6924,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6946,
    6967,
    6967,
    6967,
    6967,
    6985,
    7010,
    7034,
    7034,
    7034,
    7034,
    7059,
    7087,
    7115,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7150,
    7169,
    7195,
    7195,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7221,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7238,
    7256,
    7256,
    7256,
    7256,
    7256,
    7256,
    7256,
    7256,
    7256,
    7278,
    7278,
    7278,
    7278,
    7278,
    7278,
    7291,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7309,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
    7328,
};

enum
  {
    TOTAL_KEYWORDS = 406,
    MIN_WORD_LENGTH = 3,
    MAX_WORD_LENGTH = 34,
    MIN_HASH_VALUE = 12,
    MAX_HASH_VALUE = 2658
  };

/* maximum key range = 2647, duplicates = 0 */

class CSSPropertyNamesHash
{
private:
  static inline unsigned int property_hash_function (const char *str, unsigned int len);
public:
  static const struct Property *findPropertyImpl (const char *str, unsigned int len);
};

inline unsigned int
CSSPropertyNamesHash::property_hash_function (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
    {
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659,    2, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659,    2,   20,   98,
         8,    2,  413,    2,  569,    2,    2,   14,  110,    2,
         2,    5,   32,    2,    2,   68,   11,  218,   50,   20,
       620,  608,  215, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659, 2659,
      2659, 2659, 2659, 2659, 2659, 2659
    };
  register unsigned int hval = 0;

  switch (len)
    {
      default:
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
        hval += asso_values[(unsigned char)str[10]];
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
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct stringpool_t
  {
    char stringpool_str0[sizeof("margin")];
    char stringpool_str1[sizeof("grid")];
    char stringpool_str2[sizeof("order")];
    char stringpool_str3[sizeof("grid-area")];
    char stringpool_str4[sizeof("motion")];
    char stringpool_str5[sizeof("animation")];
    char stringpool_str6[sizeof("rotate")];
    char stringpool_str7[sizeof("image-rendering")];
    char stringpool_str8[sizeof("page")];
    char stringpool_str9[sizeof("border")];
    char stringpool_str10[sizeof("animation-name")];
    char stringpool_str11[sizeof("grid-row")];
    char stringpool_str12[sizeof("orientation")];
    char stringpool_str13[sizeof("top")];
    char stringpool_str14[sizeof("border-image")];
    char stringpool_str15[sizeof("grid-gap")];
    char stringpool_str16[sizeof("bottom")];
    char stringpool_str17[sizeof("padding")];
    char stringpool_str18[sizeof("grid-row-end")];
    char stringpool_str19[sizeof("image-orientation")];
    char stringpool_str20[sizeof("writing-mode")];
    char stringpool_str21[sizeof("margin-top")];
    char stringpool_str22[sizeof("margin-bottom")];
    char stringpool_str23[sizeof("motion-rotation")];
    char stringpool_str24[sizeof("word-break")];
    char stringpool_str25[sizeof("grid-row-gap")];
    char stringpool_str26[sizeof("border-top")];
    char stringpool_str27[sizeof("-webkit-order")];
    char stringpool_str28[sizeof("word-wrap")];
    char stringpool_str29[sizeof("border-bottom")];
    char stringpool_str30[sizeof("-webkit-margin-end")];
    char stringpool_str31[sizeof("-webkit-animation")];
    char stringpool_str32[sizeof("border-image-repeat")];
    char stringpool_str33[sizeof("padding-top")];
    char stringpool_str34[sizeof("transition")];
    char stringpool_str35[sizeof("padding-bottom")];
    char stringpool_str36[sizeof("-webkit-animation-name")];
    char stringpool_str37[sizeof("speak")];
    char stringpool_str38[sizeof("contain")];
    char stringpool_str39[sizeof("widows")];
    char stringpool_str40[sizeof("-webkit-border-image")];
    char stringpool_str41[sizeof("-webkit-border-end")];
    char stringpool_str42[sizeof("position")];
    char stringpool_str43[sizeof("content")];
    char stringpool_str44[sizeof("direction")];
    char stringpool_str45[sizeof("-webkit-writing-mode")];
    char stringpool_str46[sizeof("grid-row-start")];
    char stringpool_str47[sizeof("-webkit-padding-end")];
    char stringpool_str48[sizeof("-webkit-app-region")];
    char stringpool_str49[sizeof("-webkit-mask")];
    char stringpool_str50[sizeof("animation-direction")];
    char stringpool_str51[sizeof("page-break-inside")];
    char stringpool_str52[sizeof("src")];
    char stringpool_str53[sizeof("-webkit-mask-image")];
    char stringpool_str54[sizeof("-webkit-mask-origin")];
    char stringpool_str55[sizeof("-webkit-transition")];
    char stringpool_str56[sizeof("-webkit-margin-start")];
    char stringpool_str57[sizeof("grid-template")];
    char stringpool_str58[sizeof("pointer-events")];
    char stringpool_str59[sizeof("align-items")];
    char stringpool_str60[sizeof("isolation")];
    char stringpool_str61[sizeof("-webkit-border-start")];
    char stringpool_str62[sizeof("translate")];
    char stringpool_str63[sizeof("-webkit-mask-repeat")];
    char stringpool_str64[sizeof("clear")];
    char stringpool_str65[sizeof("color")];
    char stringpool_str66[sizeof("all")];
    char stringpool_str67[sizeof("-webkit-rtl-ordering")];
    char stringpool_str68[sizeof("-webkit-padding-start")];
    char stringpool_str69[sizeof("zoom")];
    char stringpool_str70[sizeof("-webkit-line-break")];
    char stringpool_str71[sizeof("caption-side")];
    char stringpool_str72[sizeof("min-zoom")];
    char stringpool_str73[sizeof("-webkit-animation-direction")];
    char stringpool_str74[sizeof("clip")];
    char stringpool_str75[sizeof("word-spacing")];
    char stringpool_str76[sizeof("border-spacing")];
    char stringpool_str77[sizeof("-webkit-appearance")];
    char stringpool_str78[sizeof("align-content")];
    char stringpool_str79[sizeof("border-color")];
    char stringpool_str80[sizeof("grid-template-areas")];
    char stringpool_str81[sizeof("object-position")];
    char stringpool_str82[sizeof("-webkit-align-items")];
    char stringpool_str83[sizeof("scale")];
    char stringpool_str84[sizeof("animation-duration")];
    char stringpool_str85[sizeof("grid-template-rows")];
    char stringpool_str86[sizeof("size")];
    char stringpool_str87[sizeof("-webkit-mask-position")];
    char stringpool_str88[sizeof("resize")];
    char stringpool_str89[sizeof("perspective")];
    char stringpool_str90[sizeof("quotes")];
    char stringpool_str91[sizeof("border-top-color")];
    char stringpool_str92[sizeof("border-bottom-color")];
    char stringpool_str93[sizeof("perspective-origin")];
    char stringpool_str94[sizeof("tab-size")];
    char stringpool_str95[sizeof("-webkit-align-content")];
    char stringpool_str96[sizeof("border-image-slice")];
    char stringpool_str97[sizeof("-epub-writing-mode")];
    char stringpool_str98[sizeof("border-radius")];
    char stringpool_str99[sizeof("letter-spacing")];
    char stringpool_str100[sizeof("unicode-range")];
    char stringpool_str101[sizeof("-webkit-border-end-color")];
    char stringpool_str102[sizeof("grid-auto-rows")];
    char stringpool_str103[sizeof("outline")];
    char stringpool_str104[sizeof("-epub-word-break")];
    char stringpool_str105[sizeof("-webkit-animation-duration")];
    char stringpool_str106[sizeof("transition-duration")];
    char stringpool_str107[sizeof("border-image-outset")];
    char stringpool_str108[sizeof("unicode-bidi")];
    char stringpool_str109[sizeof("background")];
    char stringpool_str110[sizeof("-webkit-perspective")];
    char stringpool_str111[sizeof("-webkit-user-drag")];
    char stringpool_str112[sizeof("background-image")];
    char stringpool_str113[sizeof("-webkit-mask-composite")];
    char stringpool_str114[sizeof("background-origin")];
    char stringpool_str115[sizeof("-webkit-perspective-origin")];
    char stringpool_str116[sizeof("cursor")];
    char stringpool_str117[sizeof("vertical-align")];
    char stringpool_str118[sizeof("-webkit-locale")];
    char stringpool_str119[sizeof("-webkit-mask-clip")];
    char stringpool_str120[sizeof("animation-iteration-count")];
    char stringpool_str121[sizeof("-webkit-border-radius")];
    char stringpool_str122[sizeof("background-repeat")];
    char stringpool_str123[sizeof("counter-reset")];
    char stringpool_str124[sizeof("-webkit-border-start-color")];
    char stringpool_str125[sizeof("font")];
    char stringpool_str126[sizeof("-webkit-transition-duration")];
    char stringpool_str127[sizeof("-webkit-line-clamp")];
    char stringpool_str128[sizeof("border-image-source")];
    char stringpool_str129[sizeof("-webkit-mask-size")];
    char stringpool_str130[sizeof("grid-column")];
    char stringpool_str131[sizeof("font-kerning")];
    char stringpool_str132[sizeof("-webkit-background-origin")];
    char stringpool_str133[sizeof("counter-increment")];
    char stringpool_str134[sizeof("grid-column-end")];
    char stringpool_str135[sizeof("border-collapse")];
    char stringpool_str136[sizeof("-webkit-animation-iteration-count")];
    char stringpool_str137[sizeof("grid-column-gap")];
    char stringpool_str138[sizeof("background-position")];
    char stringpool_str139[sizeof("font-variant")];
    char stringpool_str140[sizeof("transform")];
    char stringpool_str141[sizeof("-epub-caption-side")];
    char stringpool_str142[sizeof("page-break-after")];
    char stringpool_str143[sizeof("-webkit-margin-collapse")];
    char stringpool_str144[sizeof("-webkit-margin-after")];
    char stringpool_str145[sizeof("user-zoom")];
    char stringpool_str146[sizeof("transform-origin")];
    char stringpool_str147[sizeof("page-break-before")];
    char stringpool_str148[sizeof("-webkit-margin-before")];
    char stringpool_str149[sizeof("background-blend-mode")];
    char stringpool_str150[sizeof("left")];
    char stringpool_str151[sizeof("float")];
    char stringpool_str152[sizeof("-webkit-border-after")];
    char stringpool_str153[sizeof("-webkit-column-gap")];
    char stringpool_str154[sizeof("grid-column-start")];
    char stringpool_str155[sizeof("margin-left")];
    char stringpool_str156[sizeof("-webkit-border-before")];
    char stringpool_str157[sizeof("-webkit-padding-after")];
    char stringpool_str158[sizeof("-webkit-margin-top-collapse")];
    char stringpool_str159[sizeof("object-fit")];
    char stringpool_str160[sizeof("-webkit-margin-bottom-collapse")];
    char stringpool_str161[sizeof("outline-color")];
    char stringpool_str162[sizeof("-webkit-padding-before")];
    char stringpool_str163[sizeof("-webkit-columns")];
    char stringpool_str164[sizeof("border-left")];
    char stringpool_str165[sizeof("-webkit-transform")];
    char stringpool_str166[sizeof("right")];
    char stringpool_str167[sizeof("background-color")];
    char stringpool_str168[sizeof("padding-left")];
    char stringpool_str169[sizeof("-webkit-transform-origin")];
    char stringpool_str170[sizeof("-webkit-border-vertical-spacing")];
    char stringpool_str171[sizeof("margin-right")];
    char stringpool_str172[sizeof("overflow")];
    char stringpool_str173[sizeof("width")];
    char stringpool_str174[sizeof("-webkit-filter")];
    char stringpool_str175[sizeof("-webkit-column-span")];
    char stringpool_str176[sizeof("background-clip")];
    char stringpool_str177[sizeof("scroll-snap-destination")];
    char stringpool_str178[sizeof("min-width")];
    char stringpool_str179[sizeof("border-right")];
    char stringpool_str180[sizeof("-webkit-column-break-inside")];
    char stringpool_str181[sizeof("scroll-snap-coordinate")];
    char stringpool_str182[sizeof("motion-path")];
    char stringpool_str183[sizeof("padding-right")];
    char stringpool_str184[sizeof("border-width")];
    char stringpool_str185[sizeof("-webkit-print-color-adjust")];
    char stringpool_str186[sizeof("-webkit-user-select")];
    char stringpool_str187[sizeof("background-size")];
    char stringpool_str188[sizeof("border-image-width")];
    char stringpool_str189[sizeof("overflow-wrap")];
    char stringpool_str190[sizeof("text-rendering")];
    char stringpool_str191[sizeof("-webkit-background-composite")];
    char stringpool_str192[sizeof("text-indent")];
    char stringpool_str193[sizeof("orphans")];
    char stringpool_str194[sizeof("animation-fill-mode")];
    char stringpool_str195[sizeof("shape-margin")];
    char stringpool_str196[sizeof("-webkit-background-clip")];
    char stringpool_str197[sizeof("text-orientation")];
    char stringpool_str198[sizeof("grid-template-columns")];
    char stringpool_str199[sizeof("border-top-width")];
    char stringpool_str200[sizeof("border-bottom-width")];
    char stringpool_str201[sizeof("align-self")];
    char stringpool_str202[sizeof("font-size")];
    char stringpool_str203[sizeof("backdrop-filter")];
    char stringpool_str204[sizeof("-webkit-background-size")];
    char stringpool_str205[sizeof("-webkit-border-end-width")];
    char stringpool_str206[sizeof("-webkit-box-orient")];
    char stringpool_str207[sizeof("grid-auto-columns")];
    char stringpool_str208[sizeof("opacity")];
    char stringpool_str209[sizeof("-webkit-animation-fill-mode")];
    char stringpool_str210[sizeof("-webkit-shape-margin")];
    char stringpool_str211[sizeof("animation-delay")];
    char stringpool_str212[sizeof("text-align")];
    char stringpool_str213[sizeof("-webkit-text-orientation")];
    char stringpool_str214[sizeof("-webkit-border-after-color")];
    char stringpool_str215[sizeof("-webkit-border-before-color")];
    char stringpool_str216[sizeof("text-decoration")];
    char stringpool_str217[sizeof("-webkit-align-self")];
    char stringpool_str218[sizeof("mix-blend-mode")];
    char stringpool_str219[sizeof("border-left-color")];
    char stringpool_str220[sizeof("grid-auto-flow")];
    char stringpool_str221[sizeof("transition-property")];
    char stringpool_str222[sizeof("animation-timing-function")];
    char stringpool_str223[sizeof("white-space")];
    char stringpool_str224[sizeof("-webkit-transform-origin-z")];
    char stringpool_str225[sizeof("-webkit-mask-box-image")];
    char stringpool_str226[sizeof("-webkit-border-start-width")];
    char stringpool_str227[sizeof("-webkit-text-stroke")];
    char stringpool_str228[sizeof("-webkit-mask-repeat-y")];
    char stringpool_str229[sizeof("display")];
    char stringpool_str230[sizeof("-webkit-opacity")];
    char stringpool_str231[sizeof("-webkit-mask-repeat-x")];
    char stringpool_str232[sizeof("-webkit-animation-delay")];
    char stringpool_str233[sizeof("-webkit-box-align")];
    char stringpool_str234[sizeof("transition-delay")];
    char stringpool_str235[sizeof("border-style")];
    char stringpool_str236[sizeof("-webkit-column-rule")];
    char stringpool_str237[sizeof("-webkit-column-count")];
    char stringpool_str238[sizeof("border-right-color")];
    char stringpool_str239[sizeof("-webkit-text-combine")];
    char stringpool_str240[sizeof("z-index")];
    char stringpool_str241[sizeof("-webkit-box-direction")];
    char stringpool_str242[sizeof("max-zoom")];
    char stringpool_str243[sizeof("-webkit-box-pack")];
    char stringpool_str244[sizeof("-webkit-mask-box-image-repeat")];
    char stringpool_str245[sizeof("visibility")];
    char stringpool_str246[sizeof("-webkit-transition-property")];
    char stringpool_str247[sizeof("-webkit-animation-timing-function")];
    char stringpool_str248[sizeof("animation-play-state")];
    char stringpool_str249[sizeof("transition-timing-function")];
    char stringpool_str250[sizeof("border-top-style")];
    char stringpool_str251[sizeof("border-bottom-style")];
    char stringpool_str252[sizeof("-webkit-mask-position-y")];
    char stringpool_str253[sizeof("-webkit-box-decoration-break")];
    char stringpool_str254[sizeof("text-decoration-line")];
    char stringpool_str255[sizeof("-webkit-box-lines")];
    char stringpool_str256[sizeof("-webkit-mask-position-x")];
    char stringpool_str257[sizeof("-webkit-transition-delay")];
    char stringpool_str258[sizeof("will-change")];
    char stringpool_str259[sizeof("font-variant-ligatures")];
    char stringpool_str260[sizeof("-webkit-border-end-style")];
    char stringpool_str261[sizeof("-webkit-font-size-delta")];
    char stringpool_str262[sizeof("border-top-left-radius")];
    char stringpool_str263[sizeof("-webkit-clip-path")];
    char stringpool_str264[sizeof("border-bottom-left-radius")];
    char stringpool_str265[sizeof("box-sizing")];
    char stringpool_str266[sizeof("motion-offset")];
    char stringpool_str267[sizeof("-webkit-margin-after-collapse")];
    char stringpool_str268[sizeof("-webkit-animation-play-state")];
    char stringpool_str269[sizeof("-webkit-transition-timing-function")];
    char stringpool_str270[sizeof("text-align-last")];
    char stringpool_str271[sizeof("-webkit-margin-before-collapse")];
    char stringpool_str272[sizeof("outline-width")];
    char stringpool_str273[sizeof("-epub-text-orientation")];
    char stringpool_str274[sizeof("border-top-right-radius")];
    char stringpool_str275[sizeof("-webkit-column-break-after")];
    char stringpool_str276[sizeof("border-bottom-right-radius")];
    char stringpool_str277[sizeof("shape-outside")];
    char stringpool_str278[sizeof("list-style")];
    char stringpool_str279[sizeof("-webkit-column-break-before")];
    char stringpool_str280[sizeof("-webkit-perspective-origin-y")];
    char stringpool_str281[sizeof("-webkit-border-top-left-radius")];
    char stringpool_str282[sizeof("list-style-image")];
    char stringpool_str283[sizeof("text-decoration-color")];
    char stringpool_str284[sizeof("-webkit-border-bottom-left-radius")];
    char stringpool_str285[sizeof("-webkit-border-start-style")];
    char stringpool_str286[sizeof("-webkit-box-sizing")];
    char stringpool_str287[sizeof("-webkit-perspective-origin-x")];
    char stringpool_str288[sizeof("-webkit-logical-width")];
    char stringpool_str289[sizeof("-webkit-min-logical-width")];
    char stringpool_str290[sizeof("touch-action")];
    char stringpool_str291[sizeof("font-size-adjust")];
    char stringpool_str292[sizeof("background-repeat-y")];
    char stringpool_str293[sizeof("font-weight")];
    char stringpool_str294[sizeof("-webkit-text-stroke-color")];
    char stringpool_str295[sizeof("empty-cells")];
    char stringpool_str296[sizeof("background-repeat-x")];
    char stringpool_str297[sizeof("scroll-behavior")];
    char stringpool_str298[sizeof("-webkit-ruby-position")];
    char stringpool_str299[sizeof("-webkit-border-top-right-radius")];
    char stringpool_str300[sizeof("-epub-text-combine")];
    char stringpool_str301[sizeof("-webkit-border-bottom-right-radius")];
    char stringpool_str302[sizeof("-webkit-shape-outside")];
    char stringpool_str303[sizeof("-webkit-column-rule-color")];
    char stringpool_str304[sizeof("column-fill")];
    char stringpool_str305[sizeof("background-attachment")];
    char stringpool_str306[sizeof("-webkit-mask-box-image-slice")];
    char stringpool_str307[sizeof("table-layout")];
    char stringpool_str308[sizeof("background-position-y")];
    char stringpool_str309[sizeof("-webkit-box-ordinal-group")];
    char stringpool_str310[sizeof("-webkit-column-width")];
    char stringpool_str311[sizeof("list-style-position")];
    char stringpool_str312[sizeof("background-position-x")];
    char stringpool_str313[sizeof("text-underline-position")];
    char stringpool_str314[sizeof("-webkit-mask-box-image-outset")];
    char stringpool_str315[sizeof("mask-source-type")];
    char stringpool_str316[sizeof("flex")];
    char stringpool_str317[sizeof("outline-style")];
    char stringpool_str318[sizeof("text-transform")];
    char stringpool_str319[sizeof("scroll-snap-type")];
    char stringpool_str320[sizeof("height")];
    char stringpool_str321[sizeof("-webkit-border-after-width")];
    char stringpool_str322[sizeof("min-height")];
    char stringpool_str323[sizeof("-webkit-border-before-width")];
    char stringpool_str324[sizeof("-webkit-font-smoothing")];
    char stringpool_str325[sizeof("flex-grow")];
    char stringpool_str326[sizeof("border-left-width")];
    char stringpool_str327[sizeof("font-stretch")];
    char stringpool_str328[sizeof("flex-wrap")];
    char stringpool_str329[sizeof("-webkit-transform-origin-y")];
    char stringpool_str330[sizeof("-webkit-mask-box-image-source")];
    char stringpool_str331[sizeof("overflow-y")];
    char stringpool_str332[sizeof("-webkit-flex")];
    char stringpool_str333[sizeof("-webkit-transform-origin-x")];
    char stringpool_str334[sizeof("overflow-x")];
    char stringpool_str335[sizeof("scroll-snap-points-y")];
    char stringpool_str336[sizeof("font-style")];
    char stringpool_str337[sizeof("max-width")];
    char stringpool_str338[sizeof("border-right-width")];
    char stringpool_str339[sizeof("scroll-snap-points-x")];
    char stringpool_str340[sizeof("-webkit-border-horizontal-spacing")];
    char stringpool_str341[sizeof("-webkit-flex-grow")];
    char stringpool_str342[sizeof("font-feature-settings")];
    char stringpool_str343[sizeof("text-overflow")];
    char stringpool_str344[sizeof("font-display")];
    char stringpool_str345[sizeof("outline-offset")];
    char stringpool_str346[sizeof("line-height")];
    char stringpool_str347[sizeof("-webkit-flex-wrap")];
    char stringpool_str348[sizeof("flex-direction")];
    char stringpool_str349[sizeof("flex-basis")];
    char stringpool_str350[sizeof("transform-style")];
    char stringpool_str351[sizeof("text-shadow")];
    char stringpool_str352[sizeof("box-shadow")];
    char stringpool_str353[sizeof("-webkit-font-feature-settings")];
    char stringpool_str354[sizeof("-webkit-border-after-style")];
    char stringpool_str355[sizeof("-webkit-flex-direction")];
    char stringpool_str356[sizeof("-webkit-box-reflect")];
    char stringpool_str357[sizeof("-webkit-border-before-style")];
    char stringpool_str358[sizeof("border-left-style")];
    char stringpool_str359[sizeof("-webkit-flex-basis")];
    char stringpool_str360[sizeof("-webkit-transform-style")];
    char stringpool_str361[sizeof("-webkit-box-shadow")];
    char stringpool_str362[sizeof("-webkit-user-modify")];
    char stringpool_str363[sizeof("justify-items")];
    char stringpool_str364[sizeof("border-right-style")];
    char stringpool_str365[sizeof("-epub-text-transform")];
    char stringpool_str366[sizeof("-webkit-mask-box-image-width")];
    char stringpool_str367[sizeof("-webkit-text-stroke-width")];
    char stringpool_str368[sizeof("-webkit-column-rule-width")];
    char stringpool_str369[sizeof("justify-content")];
    char stringpool_str370[sizeof("-webkit-text-emphasis")];
    char stringpool_str371[sizeof("backface-visibility")];
    char stringpool_str372[sizeof("-webkit-justify-content")];
    char stringpool_str373[sizeof("-webkit-logical-height")];
    char stringpool_str374[sizeof("-webkit-min-logical-height")];
    char stringpool_str375[sizeof("font-family")];
    char stringpool_str376[sizeof("-webkit-text-fill-color")];
    char stringpool_str377[sizeof("text-decoration-style")];
    char stringpool_str378[sizeof("-webkit-text-emphasis-position")];
    char stringpool_str379[sizeof("-webkit-backface-visibility")];
    char stringpool_str380[sizeof("text-combine-upright")];
    char stringpool_str381[sizeof("-webkit-max-logical-width")];
    char stringpool_str382[sizeof("-webkit-column-rule-style")];
    char stringpool_str383[sizeof("list-style-type")];
    char stringpool_str384[sizeof("-epub-text-emphasis")];
    char stringpool_str385[sizeof("-webkit-text-emphasis-color")];
    char stringpool_str386[sizeof("flex-flow")];
    char stringpool_str387[sizeof("-webkit-text-security")];
    char stringpool_str388[sizeof("-webkit-flex-flow")];
    char stringpool_str389[sizeof("max-height")];
    char stringpool_str390[sizeof("flex-shrink")];
    char stringpool_str391[sizeof("-webkit-box-flex")];
    char stringpool_str392[sizeof("-webkit-text-decorations-in-effect")];
    char stringpool_str393[sizeof("-webkit-flex-shrink")];
    char stringpool_str394[sizeof("-epub-text-emphasis-color")];
    char stringpool_str395[sizeof("-webkit-highlight")];
    char stringpool_str396[sizeof("justify-self")];
    char stringpool_str397[sizeof("text-justify")];
    char stringpool_str398[sizeof("shape-image-threshold")];
    char stringpool_str399[sizeof("-webkit-shape-image-threshold")];
    char stringpool_str400[sizeof("-webkit-box-flex-group")];
    char stringpool_str401[sizeof("-webkit-tap-highlight-color")];
    char stringpool_str402[sizeof("-webkit-max-logical-height")];
    char stringpool_str403[sizeof("-webkit-text-emphasis-style")];
    char stringpool_str404[sizeof("-epub-text-emphasis-style")];
    char stringpool_str405[sizeof("-webkit-hyphenate-character")];
  };
static const struct stringpool_t stringpool_contents =
  {
    "margin",
    "grid",
    "order",
    "grid-area",
    "motion",
    "animation",
    "rotate",
    "image-rendering",
    "page",
    "border",
    "animation-name",
    "grid-row",
    "orientation",
    "top",
    "border-image",
    "grid-gap",
    "bottom",
    "padding",
    "grid-row-end",
    "image-orientation",
    "writing-mode",
    "margin-top",
    "margin-bottom",
    "motion-rotation",
    "word-break",
    "grid-row-gap",
    "border-top",
    "-webkit-order",
    "word-wrap",
    "border-bottom",
    "-webkit-margin-end",
    "-webkit-animation",
    "border-image-repeat",
    "padding-top",
    "transition",
    "padding-bottom",
    "-webkit-animation-name",
    "speak",
    "contain",
    "widows",
    "-webkit-border-image",
    "-webkit-border-end",
    "position",
    "content",
    "direction",
    "-webkit-writing-mode",
    "grid-row-start",
    "-webkit-padding-end",
    "-webkit-app-region",
    "-webkit-mask",
    "animation-direction",
    "page-break-inside",
    "src",
    "-webkit-mask-image",
    "-webkit-mask-origin",
    "-webkit-transition",
    "-webkit-margin-start",
    "grid-template",
    "pointer-events",
    "align-items",
    "isolation",
    "-webkit-border-start",
    "translate",
    "-webkit-mask-repeat",
    "clear",
    "color",
    "all",
    "-webkit-rtl-ordering",
    "-webkit-padding-start",
    "zoom",
    "-webkit-line-break",
    "caption-side",
    "min-zoom",
    "-webkit-animation-direction",
    "clip",
    "word-spacing",
    "border-spacing",
    "-webkit-appearance",
    "align-content",
    "border-color",
    "grid-template-areas",
    "object-position",
    "-webkit-align-items",
    "scale",
    "animation-duration",
    "grid-template-rows",
    "size",
    "-webkit-mask-position",
    "resize",
    "perspective",
    "quotes",
    "border-top-color",
    "border-bottom-color",
    "perspective-origin",
    "tab-size",
    "-webkit-align-content",
    "border-image-slice",
    "-epub-writing-mode",
    "border-radius",
    "letter-spacing",
    "unicode-range",
    "-webkit-border-end-color",
    "grid-auto-rows",
    "outline",
    "-epub-word-break",
    "-webkit-animation-duration",
    "transition-duration",
    "border-image-outset",
    "unicode-bidi",
    "background",
    "-webkit-perspective",
    "-webkit-user-drag",
    "background-image",
    "-webkit-mask-composite",
    "background-origin",
    "-webkit-perspective-origin",
    "cursor",
    "vertical-align",
    "-webkit-locale",
    "-webkit-mask-clip",
    "animation-iteration-count",
    "-webkit-border-radius",
    "background-repeat",
    "counter-reset",
    "-webkit-border-start-color",
    "font",
    "-webkit-transition-duration",
    "-webkit-line-clamp",
    "border-image-source",
    "-webkit-mask-size",
    "grid-column",
    "font-kerning",
    "-webkit-background-origin",
    "counter-increment",
    "grid-column-end",
    "border-collapse",
    "-webkit-animation-iteration-count",
    "grid-column-gap",
    "background-position",
    "font-variant",
    "transform",
    "-epub-caption-side",
    "page-break-after",
    "-webkit-margin-collapse",
    "-webkit-margin-after",
    "user-zoom",
    "transform-origin",
    "page-break-before",
    "-webkit-margin-before",
    "background-blend-mode",
    "left",
    "float",
    "-webkit-border-after",
    "-webkit-column-gap",
    "grid-column-start",
    "margin-left",
    "-webkit-border-before",
    "-webkit-padding-after",
    "-webkit-margin-top-collapse",
    "object-fit",
    "-webkit-margin-bottom-collapse",
    "outline-color",
    "-webkit-padding-before",
    "-webkit-columns",
    "border-left",
    "-webkit-transform",
    "right",
    "background-color",
    "padding-left",
    "-webkit-transform-origin",
    "-webkit-border-vertical-spacing",
    "margin-right",
    "overflow",
    "width",
    "-webkit-filter",
    "-webkit-column-span",
    "background-clip",
    "scroll-snap-destination",
    "min-width",
    "border-right",
    "-webkit-column-break-inside",
    "scroll-snap-coordinate",
    "motion-path",
    "padding-right",
    "border-width",
    "-webkit-print-color-adjust",
    "-webkit-user-select",
    "background-size",
    "border-image-width",
    "overflow-wrap",
    "text-rendering",
    "-webkit-background-composite",
    "text-indent",
    "orphans",
    "animation-fill-mode",
    "shape-margin",
    "-webkit-background-clip",
    "text-orientation",
    "grid-template-columns",
    "border-top-width",
    "border-bottom-width",
    "align-self",
    "font-size",
    "backdrop-filter",
    "-webkit-background-size",
    "-webkit-border-end-width",
    "-webkit-box-orient",
    "grid-auto-columns",
    "opacity",
    "-webkit-animation-fill-mode",
    "-webkit-shape-margin",
    "animation-delay",
    "text-align",
    "-webkit-text-orientation",
    "-webkit-border-after-color",
    "-webkit-border-before-color",
    "text-decoration",
    "-webkit-align-self",
    "mix-blend-mode",
    "border-left-color",
    "grid-auto-flow",
    "transition-property",
    "animation-timing-function",
    "white-space",
    "-webkit-transform-origin-z",
    "-webkit-mask-box-image",
    "-webkit-border-start-width",
    "-webkit-text-stroke",
    "-webkit-mask-repeat-y",
    "display",
    "-webkit-opacity",
    "-webkit-mask-repeat-x",
    "-webkit-animation-delay",
    "-webkit-box-align",
    "transition-delay",
    "border-style",
    "-webkit-column-rule",
    "-webkit-column-count",
    "border-right-color",
    "-webkit-text-combine",
    "z-index",
    "-webkit-box-direction",
    "max-zoom",
    "-webkit-box-pack",
    "-webkit-mask-box-image-repeat",
    "visibility",
    "-webkit-transition-property",
    "-webkit-animation-timing-function",
    "animation-play-state",
    "transition-timing-function",
    "border-top-style",
    "border-bottom-style",
    "-webkit-mask-position-y",
    "-webkit-box-decoration-break",
    "text-decoration-line",
    "-webkit-box-lines",
    "-webkit-mask-position-x",
    "-webkit-transition-delay",
    "will-change",
    "font-variant-ligatures",
    "-webkit-border-end-style",
    "-webkit-font-size-delta",
    "border-top-left-radius",
    "-webkit-clip-path",
    "border-bottom-left-radius",
    "box-sizing",
    "motion-offset",
    "-webkit-margin-after-collapse",
    "-webkit-animation-play-state",
    "-webkit-transition-timing-function",
    "text-align-last",
    "-webkit-margin-before-collapse",
    "outline-width",
    "-epub-text-orientation",
    "border-top-right-radius",
    "-webkit-column-break-after",
    "border-bottom-right-radius",
    "shape-outside",
    "list-style",
    "-webkit-column-break-before",
    "-webkit-perspective-origin-y",
    "-webkit-border-top-left-radius",
    "list-style-image",
    "text-decoration-color",
    "-webkit-border-bottom-left-radius",
    "-webkit-border-start-style",
    "-webkit-box-sizing",
    "-webkit-perspective-origin-x",
    "-webkit-logical-width",
    "-webkit-min-logical-width",
    "touch-action",
    "font-size-adjust",
    "background-repeat-y",
    "font-weight",
    "-webkit-text-stroke-color",
    "empty-cells",
    "background-repeat-x",
    "scroll-behavior",
    "-webkit-ruby-position",
    "-webkit-border-top-right-radius",
    "-epub-text-combine",
    "-webkit-border-bottom-right-radius",
    "-webkit-shape-outside",
    "-webkit-column-rule-color",
    "column-fill",
    "background-attachment",
    "-webkit-mask-box-image-slice",
    "table-layout",
    "background-position-y",
    "-webkit-box-ordinal-group",
    "-webkit-column-width",
    "list-style-position",
    "background-position-x",
    "text-underline-position",
    "-webkit-mask-box-image-outset",
    "mask-source-type",
    "flex",
    "outline-style",
    "text-transform",
    "scroll-snap-type",
    "height",
    "-webkit-border-after-width",
    "min-height",
    "-webkit-border-before-width",
    "-webkit-font-smoothing",
    "flex-grow",
    "border-left-width",
    "font-stretch",
    "flex-wrap",
    "-webkit-transform-origin-y",
    "-webkit-mask-box-image-source",
    "overflow-y",
    "-webkit-flex",
    "-webkit-transform-origin-x",
    "overflow-x",
    "scroll-snap-points-y",
    "font-style",
    "max-width",
    "border-right-width",
    "scroll-snap-points-x",
    "-webkit-border-horizontal-spacing",
    "-webkit-flex-grow",
    "font-feature-settings",
    "text-overflow",
    "font-display",
    "outline-offset",
    "line-height",
    "-webkit-flex-wrap",
    "flex-direction",
    "flex-basis",
    "transform-style",
    "text-shadow",
    "box-shadow",
    "-webkit-font-feature-settings",
    "-webkit-border-after-style",
    "-webkit-flex-direction",
    "-webkit-box-reflect",
    "-webkit-border-before-style",
    "border-left-style",
    "-webkit-flex-basis",
    "-webkit-transform-style",
    "-webkit-box-shadow",
    "-webkit-user-modify",
    "justify-items",
    "border-right-style",
    "-epub-text-transform",
    "-webkit-mask-box-image-width",
    "-webkit-text-stroke-width",
    "-webkit-column-rule-width",
    "justify-content",
    "-webkit-text-emphasis",
    "backface-visibility",
    "-webkit-justify-content",
    "-webkit-logical-height",
    "-webkit-min-logical-height",
    "font-family",
    "-webkit-text-fill-color",
    "text-decoration-style",
    "-webkit-text-emphasis-position",
    "-webkit-backface-visibility",
    "text-combine-upright",
    "-webkit-max-logical-width",
    "-webkit-column-rule-style",
    "list-style-type",
    "-epub-text-emphasis",
    "-webkit-text-emphasis-color",
    "flex-flow",
    "-webkit-text-security",
    "-webkit-flex-flow",
    "max-height",
    "flex-shrink",
    "-webkit-box-flex",
    "-webkit-text-decorations-in-effect",
    "-webkit-flex-shrink",
    "-epub-text-emphasis-color",
    "-webkit-highlight",
    "justify-self",
    "text-justify",
    "shape-image-threshold",
    "-webkit-shape-image-threshold",
    "-webkit-box-flex-group",
    "-webkit-tap-highlight-color",
    "-webkit-max-logical-height",
    "-webkit-text-emphasis-style",
    "-epub-text-emphasis-style",
    "-webkit-hyphenate-character"
  };
#define stringpool ((const char *) &stringpool_contents)

static const struct Property property_word_list[] =
  {
    {offsetof(struct stringpool_t, stringpool_str0), CSSPropertyMargin},
    {offsetof(struct stringpool_t, stringpool_str1), CSSPropertyGrid},
    {offsetof(struct stringpool_t, stringpool_str2), CSSPropertyOrder},
    {offsetof(struct stringpool_t, stringpool_str3), CSSPropertyGridArea},
    {offsetof(struct stringpool_t, stringpool_str4), CSSPropertyMotion},
    {offsetof(struct stringpool_t, stringpool_str5), CSSPropertyAnimation},
    {offsetof(struct stringpool_t, stringpool_str6), CSSPropertyRotate},
    {offsetof(struct stringpool_t, stringpool_str7), CSSPropertyImageRendering},
    {offsetof(struct stringpool_t, stringpool_str8), CSSPropertyPage},
    {offsetof(struct stringpool_t, stringpool_str9), CSSPropertyBorder},
    {offsetof(struct stringpool_t, stringpool_str10), CSSPropertyAnimationName},
    {offsetof(struct stringpool_t, stringpool_str11), CSSPropertyGridRow},
    {offsetof(struct stringpool_t, stringpool_str12), CSSPropertyOrientation},
    {offsetof(struct stringpool_t, stringpool_str13), CSSPropertyTop},
    {offsetof(struct stringpool_t, stringpool_str14), CSSPropertyBorderImage},
    {offsetof(struct stringpool_t, stringpool_str15), CSSPropertyGridGap},
    {offsetof(struct stringpool_t, stringpool_str16), CSSPropertyBottom},
    {offsetof(struct stringpool_t, stringpool_str17), CSSPropertyPadding},
    {offsetof(struct stringpool_t, stringpool_str18), CSSPropertyGridRowEnd},
    {offsetof(struct stringpool_t, stringpool_str19), CSSPropertyImageOrientation},
    {offsetof(struct stringpool_t, stringpool_str20), CSSPropertyWritingMode},
    {offsetof(struct stringpool_t, stringpool_str21), CSSPropertyMarginTop},
    {offsetof(struct stringpool_t, stringpool_str22), CSSPropertyMarginBottom},
    {offsetof(struct stringpool_t, stringpool_str23), CSSPropertyMotionRotation},
    {offsetof(struct stringpool_t, stringpool_str24), CSSPropertyWordBreak},
    {offsetof(struct stringpool_t, stringpool_str25), CSSPropertyGridRowGap},
    {offsetof(struct stringpool_t, stringpool_str26), CSSPropertyBorderTop},
    {offsetof(struct stringpool_t, stringpool_str27), CSSPropertyAliasWebkitOrder},
    {offsetof(struct stringpool_t, stringpool_str28), CSSPropertyWordWrap},
    {offsetof(struct stringpool_t, stringpool_str29), CSSPropertyBorderBottom},
    {offsetof(struct stringpool_t, stringpool_str30), CSSPropertyWebkitMarginEnd},
    {offsetof(struct stringpool_t, stringpool_str31), CSSPropertyAliasWebkitAnimation},
    {offsetof(struct stringpool_t, stringpool_str32), CSSPropertyBorderImageRepeat},
    {offsetof(struct stringpool_t, stringpool_str33), CSSPropertyPaddingTop},
    {offsetof(struct stringpool_t, stringpool_str34), CSSPropertyTransition},
    {offsetof(struct stringpool_t, stringpool_str35), CSSPropertyPaddingBottom},
    {offsetof(struct stringpool_t, stringpool_str36), CSSPropertyAliasWebkitAnimationName},
    {offsetof(struct stringpool_t, stringpool_str37), CSSPropertySpeak},
    {offsetof(struct stringpool_t, stringpool_str38), CSSPropertyContain},
    {offsetof(struct stringpool_t, stringpool_str39), CSSPropertyWidows},
    {offsetof(struct stringpool_t, stringpool_str40), CSSPropertyWebkitBorderImage},
    {offsetof(struct stringpool_t, stringpool_str41), CSSPropertyWebkitBorderEnd},
    {offsetof(struct stringpool_t, stringpool_str42), CSSPropertyPosition},
    {offsetof(struct stringpool_t, stringpool_str43), CSSPropertyContent},
    {offsetof(struct stringpool_t, stringpool_str44), CSSPropertyDirection},
    {offsetof(struct stringpool_t, stringpool_str45), CSSPropertyWebkitWritingMode},
    {offsetof(struct stringpool_t, stringpool_str46), CSSPropertyGridRowStart},
    {offsetof(struct stringpool_t, stringpool_str47), CSSPropertyWebkitPaddingEnd},
    {offsetof(struct stringpool_t, stringpool_str48), CSSPropertyWebkitAppRegion},
    {offsetof(struct stringpool_t, stringpool_str49), CSSPropertyWebkitMask},
    {offsetof(struct stringpool_t, stringpool_str50), CSSPropertyAnimationDirection},
    {offsetof(struct stringpool_t, stringpool_str51), CSSPropertyPageBreakInside},
    {offsetof(struct stringpool_t, stringpool_str52), CSSPropertySrc},
    {offsetof(struct stringpool_t, stringpool_str53), CSSPropertyWebkitMaskImage},
    {offsetof(struct stringpool_t, stringpool_str54), CSSPropertyWebkitMaskOrigin},
    {offsetof(struct stringpool_t, stringpool_str55), CSSPropertyAliasWebkitTransition},
    {offsetof(struct stringpool_t, stringpool_str56), CSSPropertyWebkitMarginStart},
    {offsetof(struct stringpool_t, stringpool_str57), CSSPropertyGridTemplate},
    {offsetof(struct stringpool_t, stringpool_str58), CSSPropertyPointerEvents},
    {offsetof(struct stringpool_t, stringpool_str59), CSSPropertyAlignItems},
    {offsetof(struct stringpool_t, stringpool_str60), CSSPropertyIsolation},
    {offsetof(struct stringpool_t, stringpool_str61), CSSPropertyWebkitBorderStart},
    {offsetof(struct stringpool_t, stringpool_str62), CSSPropertyTranslate},
    {offsetof(struct stringpool_t, stringpool_str63), CSSPropertyWebkitMaskRepeat},
    {offsetof(struct stringpool_t, stringpool_str64), CSSPropertyClear},
    {offsetof(struct stringpool_t, stringpool_str65), CSSPropertyColor},
    {offsetof(struct stringpool_t, stringpool_str66), CSSPropertyAll},
    {offsetof(struct stringpool_t, stringpool_str67), CSSPropertyWebkitRtlOrdering},
    {offsetof(struct stringpool_t, stringpool_str68), CSSPropertyWebkitPaddingStart},
    {offsetof(struct stringpool_t, stringpool_str69), CSSPropertyZoom},
    {offsetof(struct stringpool_t, stringpool_str70), CSSPropertyWebkitLineBreak},
    {offsetof(struct stringpool_t, stringpool_str71), CSSPropertyCaptionSide},
    {offsetof(struct stringpool_t, stringpool_str72), CSSPropertyMinZoom},
    {offsetof(struct stringpool_t, stringpool_str73), CSSPropertyAliasWebkitAnimationDirection},
    {offsetof(struct stringpool_t, stringpool_str74), CSSPropertyClip},
    {offsetof(struct stringpool_t, stringpool_str75), CSSPropertyWordSpacing},
    {offsetof(struct stringpool_t, stringpool_str76), CSSPropertyBorderSpacing},
    {offsetof(struct stringpool_t, stringpool_str77), CSSPropertyWebkitAppearance},
    {offsetof(struct stringpool_t, stringpool_str78), CSSPropertyAlignContent},
    {offsetof(struct stringpool_t, stringpool_str79), CSSPropertyBorderColor},
    {offsetof(struct stringpool_t, stringpool_str80), CSSPropertyGridTemplateAreas},
    {offsetof(struct stringpool_t, stringpool_str81), CSSPropertyObjectPosition},
    {offsetof(struct stringpool_t, stringpool_str82), CSSPropertyAliasWebkitAlignItems},
    {offsetof(struct stringpool_t, stringpool_str83), CSSPropertyScale},
    {offsetof(struct stringpool_t, stringpool_str84), CSSPropertyAnimationDuration},
    {offsetof(struct stringpool_t, stringpool_str85), CSSPropertyGridTemplateRows},
    {offsetof(struct stringpool_t, stringpool_str86), CSSPropertySize},
    {offsetof(struct stringpool_t, stringpool_str87), CSSPropertyWebkitMaskPosition},
    {offsetof(struct stringpool_t, stringpool_str88), CSSPropertyResize},
    {offsetof(struct stringpool_t, stringpool_str89), CSSPropertyPerspective},
    {offsetof(struct stringpool_t, stringpool_str90), CSSPropertyQuotes},
    {offsetof(struct stringpool_t, stringpool_str91), CSSPropertyBorderTopColor},
    {offsetof(struct stringpool_t, stringpool_str92), CSSPropertyBorderBottomColor},
    {offsetof(struct stringpool_t, stringpool_str93), CSSPropertyPerspectiveOrigin},
    {offsetof(struct stringpool_t, stringpool_str94), CSSPropertyTabSize},
    {offsetof(struct stringpool_t, stringpool_str95), CSSPropertyAliasWebkitAlignContent},
    {offsetof(struct stringpool_t, stringpool_str96), CSSPropertyBorderImageSlice},
    {offsetof(struct stringpool_t, stringpool_str97), CSSPropertyAliasEpubWritingMode},
    {offsetof(struct stringpool_t, stringpool_str98), CSSPropertyBorderRadius},
    {offsetof(struct stringpool_t, stringpool_str99), CSSPropertyLetterSpacing},
    {offsetof(struct stringpool_t, stringpool_str100), CSSPropertyUnicodeRange},
    {offsetof(struct stringpool_t, stringpool_str101), CSSPropertyWebkitBorderEndColor},
    {offsetof(struct stringpool_t, stringpool_str102), CSSPropertyGridAutoRows},
    {offsetof(struct stringpool_t, stringpool_str103), CSSPropertyOutline},
    {offsetof(struct stringpool_t, stringpool_str104), CSSPropertyAliasEpubWordBreak},
    {offsetof(struct stringpool_t, stringpool_str105), CSSPropertyAliasWebkitAnimationDuration},
    {offsetof(struct stringpool_t, stringpool_str106), CSSPropertyTransitionDuration},
    {offsetof(struct stringpool_t, stringpool_str107), CSSPropertyBorderImageOutset},
    {offsetof(struct stringpool_t, stringpool_str108), CSSPropertyUnicodeBidi},
    {offsetof(struct stringpool_t, stringpool_str109), CSSPropertyBackground},
    {offsetof(struct stringpool_t, stringpool_str110), CSSPropertyAliasWebkitPerspective},
    {offsetof(struct stringpool_t, stringpool_str111), CSSPropertyWebkitUserDrag},
    {offsetof(struct stringpool_t, stringpool_str112), CSSPropertyBackgroundImage},
    {offsetof(struct stringpool_t, stringpool_str113), CSSPropertyWebkitMaskComposite},
    {offsetof(struct stringpool_t, stringpool_str114), CSSPropertyBackgroundOrigin},
    {offsetof(struct stringpool_t, stringpool_str115), CSSPropertyAliasWebkitPerspectiveOrigin},
    {offsetof(struct stringpool_t, stringpool_str116), CSSPropertyCursor},
    {offsetof(struct stringpool_t, stringpool_str117), CSSPropertyVerticalAlign},
    {offsetof(struct stringpool_t, stringpool_str118), CSSPropertyWebkitLocale},
    {offsetof(struct stringpool_t, stringpool_str119), CSSPropertyWebkitMaskClip},
    {offsetof(struct stringpool_t, stringpool_str120), CSSPropertyAnimationIterationCount},
    {offsetof(struct stringpool_t, stringpool_str121), CSSPropertyAliasWebkitBorderRadius},
    {offsetof(struct stringpool_t, stringpool_str122), CSSPropertyBackgroundRepeat},
    {offsetof(struct stringpool_t, stringpool_str123), CSSPropertyCounterReset},
    {offsetof(struct stringpool_t, stringpool_str124), CSSPropertyWebkitBorderStartColor},
    {offsetof(struct stringpool_t, stringpool_str125), CSSPropertyFont},
    {offsetof(struct stringpool_t, stringpool_str126), CSSPropertyAliasWebkitTransitionDuration},
    {offsetof(struct stringpool_t, stringpool_str127), CSSPropertyWebkitLineClamp},
    {offsetof(struct stringpool_t, stringpool_str128), CSSPropertyBorderImageSource},
    {offsetof(struct stringpool_t, stringpool_str129), CSSPropertyWebkitMaskSize},
    {offsetof(struct stringpool_t, stringpool_str130), CSSPropertyGridColumn},
    {offsetof(struct stringpool_t, stringpool_str131), CSSPropertyFontKerning},
    {offsetof(struct stringpool_t, stringpool_str132), CSSPropertyWebkitBackgroundOrigin},
    {offsetof(struct stringpool_t, stringpool_str133), CSSPropertyCounterIncrement},
    {offsetof(struct stringpool_t, stringpool_str134), CSSPropertyGridColumnEnd},
    {offsetof(struct stringpool_t, stringpool_str135), CSSPropertyBorderCollapse},
    {offsetof(struct stringpool_t, stringpool_str136), CSSPropertyAliasWebkitAnimationIterationCount},
    {offsetof(struct stringpool_t, stringpool_str137), CSSPropertyGridColumnGap},
    {offsetof(struct stringpool_t, stringpool_str138), CSSPropertyBackgroundPosition},
    {offsetof(struct stringpool_t, stringpool_str139), CSSPropertyFontVariant},
    {offsetof(struct stringpool_t, stringpool_str140), CSSPropertyTransform},
    {offsetof(struct stringpool_t, stringpool_str141), CSSPropertyAliasEpubCaptionSide},
    {offsetof(struct stringpool_t, stringpool_str142), CSSPropertyPageBreakAfter},
    {offsetof(struct stringpool_t, stringpool_str143), CSSPropertyWebkitMarginCollapse},
    {offsetof(struct stringpool_t, stringpool_str144), CSSPropertyWebkitMarginAfter},
    {offsetof(struct stringpool_t, stringpool_str145), CSSPropertyUserZoom},
    {offsetof(struct stringpool_t, stringpool_str146), CSSPropertyTransformOrigin},
    {offsetof(struct stringpool_t, stringpool_str147), CSSPropertyPageBreakBefore},
    {offsetof(struct stringpool_t, stringpool_str148), CSSPropertyWebkitMarginBefore},
    {offsetof(struct stringpool_t, stringpool_str149), CSSPropertyBackgroundBlendMode},
    {offsetof(struct stringpool_t, stringpool_str150), CSSPropertyLeft},
    {offsetof(struct stringpool_t, stringpool_str151), CSSPropertyFloat},
    {offsetof(struct stringpool_t, stringpool_str152), CSSPropertyWebkitBorderAfter},
    {offsetof(struct stringpool_t, stringpool_str153), CSSPropertyWebkitColumnGap},
    {offsetof(struct stringpool_t, stringpool_str154), CSSPropertyGridColumnStart},
    {offsetof(struct stringpool_t, stringpool_str155), CSSPropertyMarginLeft},
    {offsetof(struct stringpool_t, stringpool_str156), CSSPropertyWebkitBorderBefore},
    {offsetof(struct stringpool_t, stringpool_str157), CSSPropertyWebkitPaddingAfter},
    {offsetof(struct stringpool_t, stringpool_str158), CSSPropertyWebkitMarginTopCollapse},
    {offsetof(struct stringpool_t, stringpool_str159), CSSPropertyObjectFit},
    {offsetof(struct stringpool_t, stringpool_str160), CSSPropertyWebkitMarginBottomCollapse},
    {offsetof(struct stringpool_t, stringpool_str161), CSSPropertyOutlineColor},
    {offsetof(struct stringpool_t, stringpool_str162), CSSPropertyWebkitPaddingBefore},
    {offsetof(struct stringpool_t, stringpool_str163), CSSPropertyWebkitColumns},
    {offsetof(struct stringpool_t, stringpool_str164), CSSPropertyBorderLeft},
    {offsetof(struct stringpool_t, stringpool_str165), CSSPropertyAliasWebkitTransform},
    {offsetof(struct stringpool_t, stringpool_str166), CSSPropertyRight},
    {offsetof(struct stringpool_t, stringpool_str167), CSSPropertyBackgroundColor},
    {offsetof(struct stringpool_t, stringpool_str168), CSSPropertyPaddingLeft},
    {offsetof(struct stringpool_t, stringpool_str169), CSSPropertyAliasWebkitTransformOrigin},
    {offsetof(struct stringpool_t, stringpool_str170), CSSPropertyWebkitBorderVerticalSpacing},
    {offsetof(struct stringpool_t, stringpool_str171), CSSPropertyMarginRight},
    {offsetof(struct stringpool_t, stringpool_str172), CSSPropertyOverflow},
    {offsetof(struct stringpool_t, stringpool_str173), CSSPropertyWidth},
    {offsetof(struct stringpool_t, stringpool_str174), CSSPropertyWebkitFilter},
    {offsetof(struct stringpool_t, stringpool_str175), CSSPropertyWebkitColumnSpan},
    {offsetof(struct stringpool_t, stringpool_str176), CSSPropertyBackgroundClip},
    {offsetof(struct stringpool_t, stringpool_str177), CSSPropertyScrollSnapDestination},
    {offsetof(struct stringpool_t, stringpool_str178), CSSPropertyMinWidth},
    {offsetof(struct stringpool_t, stringpool_str179), CSSPropertyBorderRight},
    {offsetof(struct stringpool_t, stringpool_str180), CSSPropertyWebkitColumnBreakInside},
    {offsetof(struct stringpool_t, stringpool_str181), CSSPropertyScrollSnapCoordinate},
    {offsetof(struct stringpool_t, stringpool_str182), CSSPropertyMotionPath},
    {offsetof(struct stringpool_t, stringpool_str183), CSSPropertyPaddingRight},
    {offsetof(struct stringpool_t, stringpool_str184), CSSPropertyBorderWidth},
    {offsetof(struct stringpool_t, stringpool_str185), CSSPropertyWebkitPrintColorAdjust},
    {offsetof(struct stringpool_t, stringpool_str186), CSSPropertyWebkitUserSelect},
    {offsetof(struct stringpool_t, stringpool_str187), CSSPropertyBackgroundSize},
    {offsetof(struct stringpool_t, stringpool_str188), CSSPropertyBorderImageWidth},
    {offsetof(struct stringpool_t, stringpool_str189), CSSPropertyOverflowWrap},
    {offsetof(struct stringpool_t, stringpool_str190), CSSPropertyTextRendering},
    {offsetof(struct stringpool_t, stringpool_str191), CSSPropertyWebkitBackgroundComposite},
    {offsetof(struct stringpool_t, stringpool_str192), CSSPropertyTextIndent},
    {offsetof(struct stringpool_t, stringpool_str193), CSSPropertyOrphans},
    {offsetof(struct stringpool_t, stringpool_str194), CSSPropertyAnimationFillMode},
    {offsetof(struct stringpool_t, stringpool_str195), CSSPropertyShapeMargin},
    {offsetof(struct stringpool_t, stringpool_str196), CSSPropertyWebkitBackgroundClip},
    {offsetof(struct stringpool_t, stringpool_str197), CSSPropertyTextOrientation},
    {offsetof(struct stringpool_t, stringpool_str198), CSSPropertyGridTemplateColumns},
    {offsetof(struct stringpool_t, stringpool_str199), CSSPropertyBorderTopWidth},
    {offsetof(struct stringpool_t, stringpool_str200), CSSPropertyBorderBottomWidth},
    {offsetof(struct stringpool_t, stringpool_str201), CSSPropertyAlignSelf},
    {offsetof(struct stringpool_t, stringpool_str202), CSSPropertyFontSize},
    {offsetof(struct stringpool_t, stringpool_str203), CSSPropertyBackdropFilter},
    {offsetof(struct stringpool_t, stringpool_str204), CSSPropertyAliasWebkitBackgroundSize},
    {offsetof(struct stringpool_t, stringpool_str205), CSSPropertyWebkitBorderEndWidth},
    {offsetof(struct stringpool_t, stringpool_str206), CSSPropertyWebkitBoxOrient},
    {offsetof(struct stringpool_t, stringpool_str207), CSSPropertyGridAutoColumns},
    {offsetof(struct stringpool_t, stringpool_str208), CSSPropertyOpacity},
    {offsetof(struct stringpool_t, stringpool_str209), CSSPropertyAliasWebkitAnimationFillMode},
    {offsetof(struct stringpool_t, stringpool_str210), CSSPropertyAliasWebkitShapeMargin},
    {offsetof(struct stringpool_t, stringpool_str211), CSSPropertyAnimationDelay},
    {offsetof(struct stringpool_t, stringpool_str212), CSSPropertyTextAlign},
    {offsetof(struct stringpool_t, stringpool_str213), CSSPropertyWebkitTextOrientation},
    {offsetof(struct stringpool_t, stringpool_str214), CSSPropertyWebkitBorderAfterColor},
    {offsetof(struct stringpool_t, stringpool_str215), CSSPropertyWebkitBorderBeforeColor},
    {offsetof(struct stringpool_t, stringpool_str216), CSSPropertyTextDecoration},
    {offsetof(struct stringpool_t, stringpool_str217), CSSPropertyAliasWebkitAlignSelf},
    {offsetof(struct stringpool_t, stringpool_str218), CSSPropertyMixBlendMode},
    {offsetof(struct stringpool_t, stringpool_str219), CSSPropertyBorderLeftColor},
    {offsetof(struct stringpool_t, stringpool_str220), CSSPropertyGridAutoFlow},
    {offsetof(struct stringpool_t, stringpool_str221), CSSPropertyTransitionProperty},
    {offsetof(struct stringpool_t, stringpool_str222), CSSPropertyAnimationTimingFunction},
    {offsetof(struct stringpool_t, stringpool_str223), CSSPropertyWhiteSpace},
    {offsetof(struct stringpool_t, stringpool_str224), CSSPropertyWebkitTransformOriginZ},
    {offsetof(struct stringpool_t, stringpool_str225), CSSPropertyWebkitMaskBoxImage},
    {offsetof(struct stringpool_t, stringpool_str226), CSSPropertyWebkitBorderStartWidth},
    {offsetof(struct stringpool_t, stringpool_str227), CSSPropertyWebkitTextStroke},
    {offsetof(struct stringpool_t, stringpool_str228), CSSPropertyWebkitMaskRepeatY},
    {offsetof(struct stringpool_t, stringpool_str229), CSSPropertyDisplay},
    {offsetof(struct stringpool_t, stringpool_str230), CSSPropertyAliasWebkitOpacity},
    {offsetof(struct stringpool_t, stringpool_str231), CSSPropertyWebkitMaskRepeatX},
    {offsetof(struct stringpool_t, stringpool_str232), CSSPropertyAliasWebkitAnimationDelay},
    {offsetof(struct stringpool_t, stringpool_str233), CSSPropertyWebkitBoxAlign},
    {offsetof(struct stringpool_t, stringpool_str234), CSSPropertyTransitionDelay},
    {offsetof(struct stringpool_t, stringpool_str235), CSSPropertyBorderStyle},
    {offsetof(struct stringpool_t, stringpool_str236), CSSPropertyWebkitColumnRule},
    {offsetof(struct stringpool_t, stringpool_str237), CSSPropertyWebkitColumnCount},
    {offsetof(struct stringpool_t, stringpool_str238), CSSPropertyBorderRightColor},
    {offsetof(struct stringpool_t, stringpool_str239), CSSPropertyWebkitTextCombine},
    {offsetof(struct stringpool_t, stringpool_str240), CSSPropertyZIndex},
    {offsetof(struct stringpool_t, stringpool_str241), CSSPropertyWebkitBoxDirection},
    {offsetof(struct stringpool_t, stringpool_str242), CSSPropertyMaxZoom},
    {offsetof(struct stringpool_t, stringpool_str243), CSSPropertyWebkitBoxPack},
    {offsetof(struct stringpool_t, stringpool_str244), CSSPropertyWebkitMaskBoxImageRepeat},
    {offsetof(struct stringpool_t, stringpool_str245), CSSPropertyVisibility},
    {offsetof(struct stringpool_t, stringpool_str246), CSSPropertyAliasWebkitTransitionProperty},
    {offsetof(struct stringpool_t, stringpool_str247), CSSPropertyAliasWebkitAnimationTimingFunction},
    {offsetof(struct stringpool_t, stringpool_str248), CSSPropertyAnimationPlayState},
    {offsetof(struct stringpool_t, stringpool_str249), CSSPropertyTransitionTimingFunction},
    {offsetof(struct stringpool_t, stringpool_str250), CSSPropertyBorderTopStyle},
    {offsetof(struct stringpool_t, stringpool_str251), CSSPropertyBorderBottomStyle},
    {offsetof(struct stringpool_t, stringpool_str252), CSSPropertyWebkitMaskPositionY},
    {offsetof(struct stringpool_t, stringpool_str253), CSSPropertyWebkitBoxDecorationBreak},
    {offsetof(struct stringpool_t, stringpool_str254), CSSPropertyTextDecorationLine},
    {offsetof(struct stringpool_t, stringpool_str255), CSSPropertyWebkitBoxLines},
    {offsetof(struct stringpool_t, stringpool_str256), CSSPropertyWebkitMaskPositionX},
    {offsetof(struct stringpool_t, stringpool_str257), CSSPropertyAliasWebkitTransitionDelay},
    {offsetof(struct stringpool_t, stringpool_str258), CSSPropertyWillChange},
    {offsetof(struct stringpool_t, stringpool_str259), CSSPropertyFontVariantLigatures},
    {offsetof(struct stringpool_t, stringpool_str260), CSSPropertyWebkitBorderEndStyle},
    {offsetof(struct stringpool_t, stringpool_str261), CSSPropertyWebkitFontSizeDelta},
    {offsetof(struct stringpool_t, stringpool_str262), CSSPropertyBorderTopLeftRadius},
    {offsetof(struct stringpool_t, stringpool_str263), CSSPropertyWebkitClipPath},
    {offsetof(struct stringpool_t, stringpool_str264), CSSPropertyBorderBottomLeftRadius},
    {offsetof(struct stringpool_t, stringpool_str265), CSSPropertyBoxSizing},
    {offsetof(struct stringpool_t, stringpool_str266), CSSPropertyMotionOffset},
    {offsetof(struct stringpool_t, stringpool_str267), CSSPropertyWebkitMarginAfterCollapse},
    {offsetof(struct stringpool_t, stringpool_str268), CSSPropertyAliasWebkitAnimationPlayState},
    {offsetof(struct stringpool_t, stringpool_str269), CSSPropertyAliasWebkitTransitionTimingFunction},
    {offsetof(struct stringpool_t, stringpool_str270), CSSPropertyTextAlignLast},
    {offsetof(struct stringpool_t, stringpool_str271), CSSPropertyWebkitMarginBeforeCollapse},
    {offsetof(struct stringpool_t, stringpool_str272), CSSPropertyOutlineWidth},
    {offsetof(struct stringpool_t, stringpool_str273), CSSPropertyAliasEpubTextOrientation},
    {offsetof(struct stringpool_t, stringpool_str274), CSSPropertyBorderTopRightRadius},
    {offsetof(struct stringpool_t, stringpool_str275), CSSPropertyWebkitColumnBreakAfter},
    {offsetof(struct stringpool_t, stringpool_str276), CSSPropertyBorderBottomRightRadius},
    {offsetof(struct stringpool_t, stringpool_str277), CSSPropertyShapeOutside},
    {offsetof(struct stringpool_t, stringpool_str278), CSSPropertyListStyle},
    {offsetof(struct stringpool_t, stringpool_str279), CSSPropertyWebkitColumnBreakBefore},
    {offsetof(struct stringpool_t, stringpool_str280), CSSPropertyWebkitPerspectiveOriginY},
    {offsetof(struct stringpool_t, stringpool_str281), CSSPropertyAliasWebkitBorderTopLeftRadius},
    {offsetof(struct stringpool_t, stringpool_str282), CSSPropertyListStyleImage},
    {offsetof(struct stringpool_t, stringpool_str283), CSSPropertyTextDecorationColor},
    {offsetof(struct stringpool_t, stringpool_str284), CSSPropertyAliasWebkitBorderBottomLeftRadius},
    {offsetof(struct stringpool_t, stringpool_str285), CSSPropertyWebkitBorderStartStyle},
    {offsetof(struct stringpool_t, stringpool_str286), CSSPropertyAliasWebkitBoxSizing},
    {offsetof(struct stringpool_t, stringpool_str287), CSSPropertyWebkitPerspectiveOriginX},
    {offsetof(struct stringpool_t, stringpool_str288), CSSPropertyWebkitLogicalWidth},
    {offsetof(struct stringpool_t, stringpool_str289), CSSPropertyWebkitMinLogicalWidth},
    {offsetof(struct stringpool_t, stringpool_str290), CSSPropertyTouchAction},
    {offsetof(struct stringpool_t, stringpool_str291), CSSPropertyFontSizeAdjust},
    {offsetof(struct stringpool_t, stringpool_str292), CSSPropertyBackgroundRepeatY},
    {offsetof(struct stringpool_t, stringpool_str293), CSSPropertyFontWeight},
    {offsetof(struct stringpool_t, stringpool_str294), CSSPropertyWebkitTextStrokeColor},
    {offsetof(struct stringpool_t, stringpool_str295), CSSPropertyEmptyCells},
    {offsetof(struct stringpool_t, stringpool_str296), CSSPropertyBackgroundRepeatX},
    {offsetof(struct stringpool_t, stringpool_str297), CSSPropertyScrollBehavior},
    {offsetof(struct stringpool_t, stringpool_str298), CSSPropertyWebkitRubyPosition},
    {offsetof(struct stringpool_t, stringpool_str299), CSSPropertyAliasWebkitBorderTopRightRadius},
    {offsetof(struct stringpool_t, stringpool_str300), CSSPropertyAliasEpubTextCombine},
    {offsetof(struct stringpool_t, stringpool_str301), CSSPropertyAliasWebkitBorderBottomRightRadius},
    {offsetof(struct stringpool_t, stringpool_str302), CSSPropertyAliasWebkitShapeOutside},
    {offsetof(struct stringpool_t, stringpool_str303), CSSPropertyWebkitColumnRuleColor},
    {offsetof(struct stringpool_t, stringpool_str304), CSSPropertyColumnFill},
    {offsetof(struct stringpool_t, stringpool_str305), CSSPropertyBackgroundAttachment},
    {offsetof(struct stringpool_t, stringpool_str306), CSSPropertyWebkitMaskBoxImageSlice},
    {offsetof(struct stringpool_t, stringpool_str307), CSSPropertyTableLayout},
    {offsetof(struct stringpool_t, stringpool_str308), CSSPropertyBackgroundPositionY},
    {offsetof(struct stringpool_t, stringpool_str309), CSSPropertyWebkitBoxOrdinalGroup},
    {offsetof(struct stringpool_t, stringpool_str310), CSSPropertyWebkitColumnWidth},
    {offsetof(struct stringpool_t, stringpool_str311), CSSPropertyListStylePosition},
    {offsetof(struct stringpool_t, stringpool_str312), CSSPropertyBackgroundPositionX},
    {offsetof(struct stringpool_t, stringpool_str313), CSSPropertyTextUnderlinePosition},
    {offsetof(struct stringpool_t, stringpool_str314), CSSPropertyWebkitMaskBoxImageOutset},
    {offsetof(struct stringpool_t, stringpool_str315), CSSPropertyMaskSourceType},
    {offsetof(struct stringpool_t, stringpool_str316), CSSPropertyFlex},
    {offsetof(struct stringpool_t, stringpool_str317), CSSPropertyOutlineStyle},
    {offsetof(struct stringpool_t, stringpool_str318), CSSPropertyTextTransform},
    {offsetof(struct stringpool_t, stringpool_str319), CSSPropertyScrollSnapType},
    {offsetof(struct stringpool_t, stringpool_str320), CSSPropertyHeight},
    {offsetof(struct stringpool_t, stringpool_str321), CSSPropertyWebkitBorderAfterWidth},
    {offsetof(struct stringpool_t, stringpool_str322), CSSPropertyMinHeight},
    {offsetof(struct stringpool_t, stringpool_str323), CSSPropertyWebkitBorderBeforeWidth},
    {offsetof(struct stringpool_t, stringpool_str324), CSSPropertyWebkitFontSmoothing},
    {offsetof(struct stringpool_t, stringpool_str325), CSSPropertyFlexGrow},
    {offsetof(struct stringpool_t, stringpool_str326), CSSPropertyBorderLeftWidth},
    {offsetof(struct stringpool_t, stringpool_str327), CSSPropertyFontStretch},
    {offsetof(struct stringpool_t, stringpool_str328), CSSPropertyFlexWrap},
    {offsetof(struct stringpool_t, stringpool_str329), CSSPropertyWebkitTransformOriginY},
    {offsetof(struct stringpool_t, stringpool_str330), CSSPropertyWebkitMaskBoxImageSource},
    {offsetof(struct stringpool_t, stringpool_str331), CSSPropertyOverflowY},
    {offsetof(struct stringpool_t, stringpool_str332), CSSPropertyAliasWebkitFlex},
    {offsetof(struct stringpool_t, stringpool_str333), CSSPropertyWebkitTransformOriginX},
    {offsetof(struct stringpool_t, stringpool_str334), CSSPropertyOverflowX},
    {offsetof(struct stringpool_t, stringpool_str335), CSSPropertyScrollSnapPointsY},
    {offsetof(struct stringpool_t, stringpool_str336), CSSPropertyFontStyle},
    {offsetof(struct stringpool_t, stringpool_str337), CSSPropertyMaxWidth},
    {offsetof(struct stringpool_t, stringpool_str338), CSSPropertyBorderRightWidth},
    {offsetof(struct stringpool_t, stringpool_str339), CSSPropertyScrollSnapPointsX},
    {offsetof(struct stringpool_t, stringpool_str340), CSSPropertyWebkitBorderHorizontalSpacing},
    {offsetof(struct stringpool_t, stringpool_str341), CSSPropertyAliasWebkitFlexGrow},
    {offsetof(struct stringpool_t, stringpool_str342), CSSPropertyFontFeatureSettings},
    {offsetof(struct stringpool_t, stringpool_str343), CSSPropertyTextOverflow},
    {offsetof(struct stringpool_t, stringpool_str344), CSSPropertyFontDisplay},
    {offsetof(struct stringpool_t, stringpool_str345), CSSPropertyOutlineOffset},
    {offsetof(struct stringpool_t, stringpool_str346), CSSPropertyLineHeight},
    {offsetof(struct stringpool_t, stringpool_str347), CSSPropertyAliasWebkitFlexWrap},
    {offsetof(struct stringpool_t, stringpool_str348), CSSPropertyFlexDirection},
    {offsetof(struct stringpool_t, stringpool_str349), CSSPropertyFlexBasis},
    {offsetof(struct stringpool_t, stringpool_str350), CSSPropertyTransformStyle},
    {offsetof(struct stringpool_t, stringpool_str351), CSSPropertyTextShadow},
    {offsetof(struct stringpool_t, stringpool_str352), CSSPropertyBoxShadow},
    {offsetof(struct stringpool_t, stringpool_str353), CSSPropertyAliasWebkitFontFeatureSettings},
    {offsetof(struct stringpool_t, stringpool_str354), CSSPropertyWebkitBorderAfterStyle},
    {offsetof(struct stringpool_t, stringpool_str355), CSSPropertyAliasWebkitFlexDirection},
    {offsetof(struct stringpool_t, stringpool_str356), CSSPropertyWebkitBoxReflect},
    {offsetof(struct stringpool_t, stringpool_str357), CSSPropertyWebkitBorderBeforeStyle},
    {offsetof(struct stringpool_t, stringpool_str358), CSSPropertyBorderLeftStyle},
    {offsetof(struct stringpool_t, stringpool_str359), CSSPropertyAliasWebkitFlexBasis},
    {offsetof(struct stringpool_t, stringpool_str360), CSSPropertyAliasWebkitTransformStyle},
    {offsetof(struct stringpool_t, stringpool_str361), CSSPropertyAliasWebkitBoxShadow},
    {offsetof(struct stringpool_t, stringpool_str362), CSSPropertyWebkitUserModify},
    {offsetof(struct stringpool_t, stringpool_str363), CSSPropertyJustifyItems},
    {offsetof(struct stringpool_t, stringpool_str364), CSSPropertyBorderRightStyle},
    {offsetof(struct stringpool_t, stringpool_str365), CSSPropertyAliasEpubTextTransform},
    {offsetof(struct stringpool_t, stringpool_str366), CSSPropertyWebkitMaskBoxImageWidth},
    {offsetof(struct stringpool_t, stringpool_str367), CSSPropertyWebkitTextStrokeWidth},
    {offsetof(struct stringpool_t, stringpool_str368), CSSPropertyWebkitColumnRuleWidth},
    {offsetof(struct stringpool_t, stringpool_str369), CSSPropertyJustifyContent},
    {offsetof(struct stringpool_t, stringpool_str370), CSSPropertyWebkitTextEmphasis},
    {offsetof(struct stringpool_t, stringpool_str371), CSSPropertyBackfaceVisibility},
    {offsetof(struct stringpool_t, stringpool_str372), CSSPropertyAliasWebkitJustifyContent},
    {offsetof(struct stringpool_t, stringpool_str373), CSSPropertyWebkitLogicalHeight},
    {offsetof(struct stringpool_t, stringpool_str374), CSSPropertyWebkitMinLogicalHeight},
    {offsetof(struct stringpool_t, stringpool_str375), CSSPropertyFontFamily},
    {offsetof(struct stringpool_t, stringpool_str376), CSSPropertyWebkitTextFillColor},
    {offsetof(struct stringpool_t, stringpool_str377), CSSPropertyTextDecorationStyle},
    {offsetof(struct stringpool_t, stringpool_str378), CSSPropertyWebkitTextEmphasisPosition},
    {offsetof(struct stringpool_t, stringpool_str379), CSSPropertyAliasWebkitBackfaceVisibility},
    {offsetof(struct stringpool_t, stringpool_str380), CSSPropertyTextCombineUpright},
    {offsetof(struct stringpool_t, stringpool_str381), CSSPropertyWebkitMaxLogicalWidth},
    {offsetof(struct stringpool_t, stringpool_str382), CSSPropertyWebkitColumnRuleStyle},
    {offsetof(struct stringpool_t, stringpool_str383), CSSPropertyListStyleType},
    {offsetof(struct stringpool_t, stringpool_str384), CSSPropertyAliasEpubTextEmphasis},
    {offsetof(struct stringpool_t, stringpool_str385), CSSPropertyWebkitTextEmphasisColor},
    {offsetof(struct stringpool_t, stringpool_str386), CSSPropertyFlexFlow},
    {offsetof(struct stringpool_t, stringpool_str387), CSSPropertyWebkitTextSecurity},
    {offsetof(struct stringpool_t, stringpool_str388), CSSPropertyAliasWebkitFlexFlow},
    {offsetof(struct stringpool_t, stringpool_str389), CSSPropertyMaxHeight},
    {offsetof(struct stringpool_t, stringpool_str390), CSSPropertyFlexShrink},
    {offsetof(struct stringpool_t, stringpool_str391), CSSPropertyWebkitBoxFlex},
    {offsetof(struct stringpool_t, stringpool_str392), CSSPropertyWebkitTextDecorationsInEffect},
    {offsetof(struct stringpool_t, stringpool_str393), CSSPropertyAliasWebkitFlexShrink},
    {offsetof(struct stringpool_t, stringpool_str394), CSSPropertyAliasEpubTextEmphasisColor},
    {offsetof(struct stringpool_t, stringpool_str395), CSSPropertyWebkitHighlight},
    {offsetof(struct stringpool_t, stringpool_str396), CSSPropertyJustifySelf},
    {offsetof(struct stringpool_t, stringpool_str397), CSSPropertyTextJustify},
    {offsetof(struct stringpool_t, stringpool_str398), CSSPropertyShapeImageThreshold},
    {offsetof(struct stringpool_t, stringpool_str399), CSSPropertyAliasWebkitShapeImageThreshold},
    {offsetof(struct stringpool_t, stringpool_str400), CSSPropertyWebkitBoxFlexGroup},
    {offsetof(struct stringpool_t, stringpool_str401), CSSPropertyWebkitTapHighlightColor},
    {offsetof(struct stringpool_t, stringpool_str402), CSSPropertyWebkitMaxLogicalHeight},
    {offsetof(struct stringpool_t, stringpool_str403), CSSPropertyWebkitTextEmphasisStyle},
    {offsetof(struct stringpool_t, stringpool_str404), CSSPropertyAliasEpubTextEmphasisStyle},
    {offsetof(struct stringpool_t, stringpool_str405), CSSPropertyWebkitHyphenateCharacter}
  };

static const short lookup[] =
  {
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,   0,  -1,   1,  -1,  -1,  -1,  -1,   2,
     -1,  -1,  -1,  -1,   3,  -1,  -1,   4,  -1,  -1,
      5,  -1,  -1,   6,  -1,  -1,   7,  -1,   8,   9,
     10,  -1,  -1,  11,  -1,  -1,  12,  -1,  13,  -1,
     -1,  14,  15,  -1,  16,  -1,  17,  18,  19,  -1,
     20,  -1,  21,  -1,  -1,  -1,  -1,  -1,  22,  23,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  24,  -1,  -1,
     -1,  25,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  26,
     -1,  -1,  27,  28,  -1,  29,  -1,  -1,  -1,  30,
     -1,  -1,  -1,  31,  32,  -1,  33,  34,  -1,  -1,
     -1,  -1,  35,  36,  -1,  -1,  -1,  -1,  37,  -1,
     -1,  -1,  38,  39,  40,  -1,  41,  42,  -1,  -1,
     -1,  43,  44,  45,  -1,  -1,  -1,  -1,  -1,  46,
     -1,  -1,  -1,  47,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  48,  -1,  -1,  49,
     -1,  -1,  -1,  -1,  50,  -1,  51,  -1,  52,  -1,
     -1,  53,  -1,  -1,  -1,  -1,  54,  -1,  -1,  -1,
     55,  56,  -1,  -1,  -1,  -1,  -1,  -1,  57,  -1,
     -1,  -1,  -1,  58,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  59,  -1,  60,  61,  -1,
     62,  -1,  63,  -1,  64,  -1,  -1,  -1,  -1,  -1,
     65,  -1,  66,  67,  -1,  68,  -1,  69,  -1,  -1,
     -1,  70,  -1,  -1,  71,  72,  -1,  73,  -1,  -1,
     -1,  -1,  74,  75,  -1,  -1,  -1,  76,  -1,  77,
     -1,  78,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  79,  -1,  -1,  -1,  -1,  80,  81,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  82,  -1,
     83,  -1,  84,  -1,  -1,  85,  -1,  86,  87,  -1,
     -1,  88,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  89,  -1,  -1,  -1,  -1,  90,  -1,  -1,  -1,
     -1,  91,  -1,  -1,  -1,  -1,  -1,  92,  93,  -1,
     -1,  -1,  94,  -1,  95,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  96,  -1,  -1,  97,  -1,  -1,  -1,
     -1,  98,  -1,  -1,  -1,  -1,  99, 100, 101, 102,
    103,  -1,  -1, 104,  -1, 105,  -1,  -1,  -1, 106,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 107, 108,
     -1, 109,  -1,  -1, 110,  -1,  -1,  -1,  -1, 111,
     -1,  -1,  -1, 112,  -1,  -1, 113,  -1, 114,  -1,
     -1, 115,  -1, 116,  -1,  -1,  -1, 117,  -1,  -1,
    118,  -1,  -1, 119,  -1,  -1,  -1, 120,  -1,  -1,
     -1,  -1,  -1,  -1, 121,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 122, 123,  -1,  -1,  -1,  -1,
    124, 125, 126,  -1,  -1, 127,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 128,  -1, 129,  -1,
     -1, 130,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 131,
     -1, 132,  -1, 133,  -1, 134,  -1,  -1, 135,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    136,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 137,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    138,  -1,  -1,  -1, 139,  -1,  -1, 140,  -1,  -1,
    141,  -1, 142,  -1, 143,  -1,  -1, 144,  -1, 145,
     -1,  -1,  -1,  -1, 146,  -1, 147,  -1,  -1,  -1,
     -1, 148,  -1,  -1, 149,  -1, 150,  -1,  -1,  -1,
     -1, 151,  -1,  -1, 152,  -1, 153, 154,  -1,  -1,
    155,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 156,  -1,
     -1, 157,  -1,  -1, 158,  -1, 159,  -1,  -1,  -1,
    160,  -1, 161,  -1,  -1, 162, 163, 164,  -1,  -1,
    165,  -1,  -1,  -1,  -1,  -1, 166,  -1,  -1,  -1,
     -1,  -1,  -1, 167, 168,  -1,  -1, 169,  -1, 170,
    171,  -1,  -1,  -1,  -1,  -1,  -1, 172,  -1,  -1,
    173,  -1,  -1, 174, 175, 176, 177,  -1, 178,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1, 179,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 180,  -1, 181,  -1,
     -1,  -1,  -1, 182, 183,  -1,  -1,  -1,  -1,  -1,
     -1, 184,  -1,  -1,  -1, 185, 186,  -1,  -1,  -1,
    187,  -1,  -1, 188,  -1, 189,  -1,  -1,  -1,  -1,
    190, 191,  -1, 192,  -1,  -1,  -1,  -1,  -1,  -1,
    193,  -1,  -1,  -1,  -1,  -1, 194, 195, 196,  -1,
     -1,  -1, 197, 198,  -1,  -1,  -1,  -1,  -1,  -1,
     -1, 199,  -1,  -1,  -1,  -1,  -1, 200,  -1,  -1,
     -1,  -1,  -1, 201,  -1,  -1,  -1,  -1,  -1,  -1,
    202,  -1,  -1, 203,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 204,  -1,  -1,  -1,  -1, 205,  -1,
     -1,  -1,  -1,  -1, 206,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1, 207, 208, 209,
    210,  -1, 211,  -1, 212, 213, 214,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    215,  -1,  -1, 216,  -1,  -1, 217, 218,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 219,
     -1,  -1, 220, 221,  -1,  -1, 222,  -1, 223,  -1,
     -1,  -1,  -1,  -1, 224,  -1,  -1,  -1, 225,  -1,
    226, 227, 228,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    229, 230,  -1,  -1, 231, 232,  -1,  -1, 233, 234,
    235,  -1, 236,  -1, 237,  -1,  -1,  -1,  -1, 238,
    239, 240, 241, 242,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 243,  -1,  -1,  -1,
     -1, 244,  -1,  -1,  -1, 245, 246,  -1,  -1, 247,
    248,  -1,  -1, 249,  -1,  -1,  -1,  -1,  -1,  -1,
    250,  -1,  -1,  -1,  -1,  -1, 251,  -1, 252, 253,
     -1, 254,  -1,  -1, 255,  -1,  -1,  -1,  -1,  -1,
    256,  -1, 257,  -1,  -1,  -1,  -1,  -1,  -1, 258,
     -1,  -1,  -1, 259,  -1,  -1,  -1, 260, 261, 262,
     -1, 263,  -1,  -1,  -1, 264,  -1,  -1, 265,  -1,
     -1, 266,  -1,  -1,  -1,  -1, 267,  -1,  -1,  -1,
     -1,  -1,  -1, 268,  -1,  -1, 269, 270,  -1,  -1,
    271,  -1, 272,  -1,  -1,  -1,  -1,  -1, 273,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 274,
     -1,  -1, 275,  -1,  -1, 276,  -1,  -1,  -1, 277,
     -1,  -1, 278,  -1,  -1,  -1, 279,  -1,  -1,  -1,
     -1, 280, 281,  -1, 282, 283,  -1,  -1, 284, 285,
     -1, 286,  -1, 287, 288,  -1,  -1,  -1,  -1,  -1,
     -1,  -1, 289, 290,  -1,  -1,  -1,  -1,  -1,  -1,
     -1, 291,  -1,  -1, 292,  -1,  -1,  -1,  -1, 293,
     -1,  -1,  -1, 294,  -1, 295, 296, 297,  -1,  -1,
    298,  -1, 299, 300,  -1,  -1,  -1,  -1, 301,  -1,
     -1,  -1, 302,  -1, 303,  -1,  -1,  -1,  -1,  -1,
     -1,  -1, 304,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 305,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    306, 307,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    308,  -1, 309,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    310, 311, 312, 313,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 314, 315,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 316,  -1,  -1,  -1,  -1,
     -1, 317,  -1, 318, 319, 320, 321,  -1,  -1,  -1,
     -1,  -1,  -1, 322,  -1,  -1,  -1,  -1,  -1,  -1,
    323,  -1, 324,  -1,  -1,  -1, 325,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 326,
     -1,  -1,  -1,  -1, 327,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 328,  -1,  -1,  -1, 329,  -1,  -1,
     -1,  -1,  -1, 330,  -1,  -1,  -1, 331, 332, 333,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 334,
     -1, 335, 336,  -1,  -1,  -1, 337,  -1,  -1, 338,
     -1,  -1,  -1, 339,  -1, 340,  -1,  -1,  -1, 341,
     -1, 342,  -1, 343,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 344, 345,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 346,  -1,  -1, 347,  -1,  -1, 348,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1, 349, 350,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 351, 352,
     -1,  -1,  -1,  -1, 353,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 354,  -1,  -1,  -1,  -1,
     -1,  -1, 355,  -1,  -1,  -1,  -1,  -1, 356, 357,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 358,  -1,
    359, 360,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1, 361,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 362,  -1,  -1,  -1,  -1,  -1, 363,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 364, 365,
    366,  -1,  -1, 367,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 368, 369,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 370,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 371,  -1, 372,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 373,
     -1,  -1,  -1,  -1,  -1,  -1,  -1, 374,  -1,  -1,
    375,  -1,  -1,  -1,  -1,  -1, 376,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 377,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1, 378,  -1,  -1,  -1,  -1,  -1, 379,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 380,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
    381,  -1,  -1, 382,  -1,  -1,  -1, 383,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1, 384,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 385,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 386,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 387,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 388,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1, 389,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 390,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 391,  -1,  -1,  -1,  -1,
     -1, 392,  -1,  -1,  -1,  -1,  -1, 393,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 394,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 395,
     -1,  -1,  -1,  -1,  -1,  -1,  -1, 396,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 397,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1, 398,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1, 399,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1, 400,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 401,  -1,
     -1,  -1,  -1,  -1,  -1, 402,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1, 403,  -1,  -1,  -1,  -1,
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
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 404,  -1,
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
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 405
  };

const struct Property *
CSSPropertyNamesHash::findPropertyImpl (register const char *str, register unsigned int len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = property_hash_function (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = property_word_list[index].nameOffset + stringpool;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
                return &property_word_list[index];
            }
        }
    }
  return 0;
}

const Property* findProperty(register const char* str, register unsigned int len)
{
    return CSSPropertyNamesHash::findPropertyImpl(str, len);
}

const char* getPropertyName(CSSPropertyID id)
{
    ASSERT(id >= firstCSSProperty && id <= lastUnresolvedCSSProperty);
    int index = id - firstCSSProperty;
    return propertyNameStringsPool + propertyNameStringsOffsets[index];
}

const AtomicString& getPropertyNameAtomicString(CSSPropertyID id)
{
    ASSERT(id >= firstCSSProperty && id <= lastUnresolvedCSSProperty);
    int index = id - firstCSSProperty;
    static AtomicString* propertyStrings = new AtomicString[lastUnresolvedCSSProperty]; // Intentionally never destroyed.
    AtomicString& propertyString = propertyStrings[index];
    if (propertyString.isNull()) {
        const char* propertyName = propertyNameStringsPool + propertyNameStringsOffsets[index];
        propertyString = AtomicString(propertyName, strlen(propertyName), AtomicString::ConstructFromLiteral);
    }
    return propertyString;
}

String getPropertyNameString(CSSPropertyID id)
{
    // We share the StringImpl with the AtomicStrings.
    return getPropertyNameAtomicString(id).string();
}

String getJSPropertyName(CSSPropertyID id)
{
    char result[maxCSSPropertyNameLength + 1];
    const char* cssPropertyName = getPropertyName(id);
    const char* propertyNamePointer = cssPropertyName;
    if (!propertyNamePointer)
        return emptyString();

    char* resultPointer = result;
    while (char character = *propertyNamePointer++) {
        if (character == '-') {
            char nextCharacter = *propertyNamePointer++;
            if (!nextCharacter)
                break;
            character = (propertyNamePointer - 2 != cssPropertyName) ? toASCIIUpper(nextCharacter) : nextCharacter;
        }
        *resultPointer++ = character;
    }
    *resultPointer = '\0';
    return String(result);
}

CSSPropertyID cssPropertyID(const String& string)
{
    return resolveCSSPropertyID(unresolvedCSSPropertyID(string));
}

} // namespace blink

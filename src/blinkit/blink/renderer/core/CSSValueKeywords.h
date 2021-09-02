
// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSValueKeywords_h
#define CSSValueKeywords_h

#include "core/css/parser/CSSParserMode.h"
#include <string.h>

namespace blink {

enum CSSValueID {
    CSSValueInvalid = 0,
    CSSValueInherit = 1,
    CSSValueInitial = 2,
    CSSValueNone = 3,
    CSSValueHidden = 4,
    CSSValueInset = 5,
    CSSValueGroove = 6,
    CSSValueOutset = 7,
    CSSValueRidge = 8,
    CSSValueDotted = 9,
    CSSValueDashed = 10,
    CSSValueSolid = 11,
    CSSValueDouble = 12,
    CSSValueCaption = 13,
    CSSValueIcon = 14,
    CSSValueMenu = 15,
    CSSValueMessageBox = 16,
    CSSValueSmallCaption = 17,
    CSSValueWebkitMiniControl = 18,
    CSSValueWebkitSmallControl = 19,
    CSSValueWebkitControl = 20,
    CSSValueStatusBar = 21,
    CSSValueItalic = 22,
    CSSValueOblique = 23,
    CSSValueAll = 24,
    CSSValueSmallCaps = 25,
    CSSValueCommonLigatures = 26,
    CSSValueNoCommonLigatures = 27,
    CSSValueDiscretionaryLigatures = 28,
    CSSValueNoDiscretionaryLigatures = 29,
    CSSValueHistoricalLigatures = 30,
    CSSValueNoHistoricalLigatures = 31,
    CSSValueContextual = 32,
    CSSValueNoContextual = 33,
    CSSValueNormal = 34,
    CSSValueBold = 35,
    CSSValueBolder = 36,
    CSSValueLighter = 37,
    CSSValue100 = 38,
    CSSValue200 = 39,
    CSSValue300 = 40,
    CSSValue400 = 41,
    CSSValue500 = 42,
    CSSValue600 = 43,
    CSSValue700 = 44,
    CSSValue800 = 45,
    CSSValue900 = 46,
    CSSValueUltraCondensed = 47,
    CSSValueExtraCondensed = 48,
    CSSValueCondensed = 49,
    CSSValueSemiCondensed = 50,
    CSSValueSemiExpanded = 51,
    CSSValueExpanded = 52,
    CSSValueExtraExpanded = 53,
    CSSValueUltraExpanded = 54,
    CSSValueXxSmall = 55,
    CSSValueXSmall = 56,
    CSSValueSmall = 57,
    CSSValueMedium = 58,
    CSSValueLarge = 59,
    CSSValueXLarge = 60,
    CSSValueXxLarge = 61,
    CSSValueWebkitXxxLarge = 62,
    CSSValueSmaller = 63,
    CSSValueLarger = 64,
    CSSValueSerif = 65,
    CSSValueSansSerif = 66,
    CSSValueCursive = 67,
    CSSValueFantasy = 68,
    CSSValueMonospace = 69,
    CSSValueWebkitBody = 70,
    CSSValueWebkitPictograph = 71,
    CSSValueSwap = 72,
    CSSValueFallback = 73,
    CSSValueOptional = 74,
    CSSValueAqua = 75,
    CSSValueBlack = 76,
    CSSValueBlue = 77,
    CSSValueFuchsia = 78,
    CSSValueGray = 79,
    CSSValueGreen = 80,
    CSSValueLime = 81,
    CSSValueMaroon = 82,
    CSSValueNavy = 83,
    CSSValueOlive = 84,
    CSSValueOrange = 85,
    CSSValuePurple = 86,
    CSSValueRed = 87,
    CSSValueSilver = 88,
    CSSValueTeal = 89,
    CSSValueWhite = 90,
    CSSValueYellow = 91,
    CSSValueTransparent = 92,
    CSSValueWebkitLink = 93,
    CSSValueWebkitActivelink = 94,
    CSSValueActiveborder = 95,
    CSSValueActivecaption = 96,
    CSSValueAppworkspace = 97,
    CSSValueBackground = 98,
    CSSValueButtonface = 99,
    CSSValueButtonhighlight = 100,
    CSSValueButtonshadow = 101,
    CSSValueButtontext = 102,
    CSSValueCaptiontext = 103,
    CSSValueGraytext = 104,
    CSSValueHighlight = 105,
    CSSValueHighlighttext = 106,
    CSSValueInactiveborder = 107,
    CSSValueInactivecaption = 108,
    CSSValueInactivecaptiontext = 109,
    CSSValueInfobackground = 110,
    CSSValueInfotext = 111,
    CSSValueMenutext = 112,
    CSSValueScrollbar = 113,
    CSSValueThreeddarkshadow = 114,
    CSSValueThreedface = 115,
    CSSValueThreedhighlight = 116,
    CSSValueThreedlightshadow = 117,
    CSSValueThreedshadow = 118,
    CSSValueWindow = 119,
    CSSValueWindowframe = 120,
    CSSValueWindowtext = 121,
    CSSValueInternalActiveListBoxSelection = 122,
    CSSValueInternalActiveListBoxSelectionText = 123,
    CSSValueInternalInactiveListBoxSelection = 124,
    CSSValueInternalInactiveListBoxSelectionText = 125,
    CSSValueWebkitFocusRingColor = 126,
    CSSValueCurrentcolor = 127,
    CSSValueGrey = 128,
    CSSValueWebkitText = 129,
    CSSValueRepeat = 130,
    CSSValueRepeatX = 131,
    CSSValueRepeatY = 132,
    CSSValueNoRepeat = 133,
    CSSValueClear = 134,
    CSSValueCopy = 135,
    CSSValueSourceOver = 136,
    CSSValueSourceIn = 137,
    CSSValueSourceOut = 138,
    CSSValueSourceAtop = 139,
    CSSValueDestinationOver = 140,
    CSSValueDestinationIn = 141,
    CSSValueDestinationOut = 142,
    CSSValueDestinationAtop = 143,
    CSSValueXor = 144,
    CSSValuePlusLighter = 145,
    CSSValueBaseline = 146,
    CSSValueMiddle = 147,
    CSSValueSub = 148,
    CSSValueSuper = 149,
    CSSValueTextTop = 150,
    CSSValueTextBottom = 151,
    CSSValueTop = 152,
    CSSValueBottom = 153,
    CSSValueWebkitBaselineMiddle = 154,
    CSSValueWebkitAuto = 155,
    CSSValueLeft = 156,
    CSSValueRight = 157,
    CSSValueCenter = 158,
    CSSValueJustify = 159,
    CSSValueWebkitLeft = 160,
    CSSValueWebkitRight = 161,
    CSSValueWebkitCenter = 162,
    CSSValueWebkitMatchParent = 163,
    CSSValueInterWord = 164,
    CSSValueDistribute = 165,
    CSSValueOutside = 166,
    CSSValueInside = 167,
    CSSValueDisc = 168,
    CSSValueCircle = 169,
    CSSValueSquare = 170,
    CSSValueDecimal = 171,
    CSSValueDecimalLeadingZero = 172,
    CSSValueArabicIndic = 173,
    CSSValueBengali = 174,
    CSSValueCambodian = 175,
    CSSValueKhmer = 176,
    CSSValueDevanagari = 177,
    CSSValueGujarati = 178,
    CSSValueGurmukhi = 179,
    CSSValueKannada = 180,
    CSSValueLao = 181,
    CSSValueMalayalam = 182,
    CSSValueMongolian = 183,
    CSSValueMyanmar = 184,
    CSSValueOriya = 185,
    CSSValuePersian = 186,
    CSSValueUrdu = 187,
    CSSValueTelugu = 188,
    CSSValueTibetan = 189,
    CSSValueThai = 190,
    CSSValueLowerRoman = 191,
    CSSValueUpperRoman = 192,
    CSSValueLowerGreek = 193,
    CSSValueLowerAlpha = 194,
    CSSValueLowerLatin = 195,
    CSSValueUpperAlpha = 196,
    CSSValueUpperLatin = 197,
    CSSValueCjkEarthlyBranch = 198,
    CSSValueCjkHeavenlyStem = 199,
    CSSValueEthiopicHalehame = 200,
    CSSValueEthiopicHalehameAm = 201,
    CSSValueEthiopicHalehameTiEr = 202,
    CSSValueEthiopicHalehameTiEt = 203,
    CSSValueHangul = 204,
    CSSValueHangulConsonant = 205,
    CSSValueKoreanHangulFormal = 206,
    CSSValueKoreanHanjaFormal = 207,
    CSSValueKoreanHanjaInformal = 208,
    CSSValueHebrew = 209,
    CSSValueArmenian = 210,
    CSSValueLowerArmenian = 211,
    CSSValueUpperArmenian = 212,
    CSSValueGeorgian = 213,
    CSSValueCjkIdeographic = 214,
    CSSValueSimpChineseFormal = 215,
    CSSValueSimpChineseInformal = 216,
    CSSValueTradChineseFormal = 217,
    CSSValueTradChineseInformal = 218,
    CSSValueHiragana = 219,
    CSSValueKatakana = 220,
    CSSValueHiraganaIroha = 221,
    CSSValueKatakanaIroha = 222,
    CSSValueInline = 223,
    CSSValueBlock = 224,
    CSSValueListItem = 225,
    CSSValueInlineBlock = 226,
    CSSValueTable = 227,
    CSSValueInlineTable = 228,
    CSSValueTableRowGroup = 229,
    CSSValueTableHeaderGroup = 230,
    CSSValueTableFooterGroup = 231,
    CSSValueTableRow = 232,
    CSSValueTableColumnGroup = 233,
    CSSValueTableColumn = 234,
    CSSValueTableCell = 235,
    CSSValueTableCaption = 236,
    CSSValueWebkitBox = 237,
    CSSValueWebkitInlineBox = 238,
    CSSValueFlex = 239,
    CSSValueInlineFlex = 240,
    CSSValueGrid = 241,
    CSSValueInlineGrid = 242,
    CSSValueWebkitFlex = 243,
    CSSValueWebkitInlineFlex = 244,
    CSSValueAuto = 245,
    CSSValueCrosshair = 246,
    CSSValueDefault = 247,
    CSSValuePointer = 248,
    CSSValueMove = 249,
    CSSValueVerticalText = 250,
    CSSValueCell = 251,
    CSSValueContextMenu = 252,
    CSSValueAlias = 253,
    CSSValueProgress = 254,
    CSSValueNoDrop = 255,
    CSSValueNotAllowed = 256,
    CSSValueZoomIn = 257,
    CSSValueZoomOut = 258,
    CSSValueEResize = 259,
    CSSValueNeResize = 260,
    CSSValueNwResize = 261,
    CSSValueNResize = 262,
    CSSValueSeResize = 263,
    CSSValueSwResize = 264,
    CSSValueSResize = 265,
    CSSValueWResize = 266,
    CSSValueEwResize = 267,
    CSSValueNsResize = 268,
    CSSValueNeswResize = 269,
    CSSValueNwseResize = 270,
    CSSValueColResize = 271,
    CSSValueRowResize = 272,
    CSSValueText = 273,
    CSSValueWait = 274,
    CSSValueHelp = 275,
    CSSValueAllScroll = 276,
    CSSValueWebkitGrab = 277,
    CSSValueWebkitGrabbing = 278,
    CSSValueWebkitZoomIn = 279,
    CSSValueWebkitZoomOut = 280,
    CSSValueLtr = 281,
    CSSValueRtl = 282,
    CSSValueCapitalize = 283,
    CSSValueUppercase = 284,
    CSSValueLowercase = 285,
    CSSValueVisible = 286,
    CSSValueCollapse = 287,
    CSSValueA3 = 288,
    CSSValueA4 = 289,
    CSSValueA5 = 290,
    CSSValueAbove = 291,
    CSSValueAbsolute = 292,
    CSSValueAlways = 293,
    CSSValueAvoid = 294,
    CSSValueB4 = 295,
    CSSValueB5 = 296,
    CSSValueBelow = 297,
    CSSValueBidiOverride = 298,
    CSSValueBlink = 299,
    CSSValueBoth = 300,
    CSSValueCloseQuote = 301,
    CSSValueEmbed = 302,
    CSSValueFixed = 303,
    CSSValueHand = 304,
    CSSValueHide = 305,
    CSSValueIsolate = 306,
    CSSValueIsolateOverride = 307,
    CSSValuePlaintext = 308,
    CSSValueWebkitIsolate = 309,
    CSSValueWebkitIsolateOverride = 310,
    CSSValueWebkitPlaintext = 311,
    CSSValueLandscape = 312,
    CSSValueLedger = 313,
    CSSValueLegal = 314,
    CSSValueLetter = 315,
    CSSValueLineThrough = 316,
    CSSValueLocal = 317,
    CSSValueNoCloseQuote = 318,
    CSSValueNoOpenQuote = 319,
    CSSValueNowrap = 320,
    CSSValueOpenQuote = 321,
    CSSValueOverlay = 322,
    CSSValueOverline = 323,
    CSSValuePortrait = 324,
    CSSValuePre = 325,
    CSSValuePreLine = 326,
    CSSValuePreWrap = 327,
    CSSValueRelative = 328,
    CSSValueScroll = 329,
    CSSValueSeparate = 330,
    CSSValueShow = 331,
    CSSValueStatic = 332,
    CSSValueThick = 333,
    CSSValueThin = 334,
    CSSValueUnderline = 335,
    CSSValueWavy = 336,
    CSSValueWebkitNowrap = 337,
    CSSValueStretch = 338,
    CSSValueStart = 339,
    CSSValueEnd = 340,
    CSSValueClone = 341,
    CSSValueSlice = 342,
    CSSValueReverse = 343,
    CSSValueHorizontal = 344,
    CSSValueVertical = 345,
    CSSValueInlineAxis = 346,
    CSSValueBlockAxis = 347,
    CSSValueSingle = 348,
    CSSValueMultiple = 349,
    CSSValueFlexStart = 350,
    CSSValueFlexEnd = 351,
    CSSValueSpaceBetween = 352,
    CSSValueSpaceAround = 353,
    CSSValueSpaceEvenly = 354,
    CSSValueUnsafe = 355,
    CSSValueSafe = 356,
    CSSValueRow = 357,
    CSSValueRowReverse = 358,
    CSSValueColumn = 359,
    CSSValueColumnReverse = 360,
    CSSValueWrap = 361,
    CSSValueWrapReverse = 362,
    CSSValueDense = 363,
    CSSValueReadOnly = 364,
    CSSValueReadWrite = 365,
    CSSValueReadWritePlaintextOnly = 366,
    CSSValueElement = 367,
    CSSValueWebkitMinContent = 368,
    CSSValueWebkitMaxContent = 369,
    CSSValueWebkitFillAvailable = 370,
    CSSValueWebkitFitContent = 371,
    CSSValueMinContent = 372,
    CSSValueMaxContent = 373,
    CSSValueFitContent = 374,
    CSSValueClip = 375,
    CSSValueEllipsis = 376,
    CSSValueDiscard = 377,
    CSSValueBreakAll = 378,
    CSSValueKeepAll = 379,
    CSSValueBreakWord = 380,
    CSSValueSpace = 381,
    CSSValueLoose = 382,
    CSSValueStrict = 383,
    CSSValueAfterWhiteSpace = 384,
    CSSValueCheckbox = 385,
    CSSValueRadio = 386,
    CSSValuePushButton = 387,
    CSSValueSquareButton = 388,
    CSSValueButton = 389,
    CSSValueButtonBevel = 390,
    CSSValueInnerSpinButton = 391,
    CSSValueListbox = 392,
    CSSValueListitem = 393,
    CSSValueMediaEnterFullscreenButton = 394,
    CSSValueMediaExitFullscreenButton = 395,
    CSSValueMediaFullscreenVolumeSlider = 396,
    CSSValueMediaFullscreenVolumeSliderThumb = 397,
    CSSValueMediaMuteButton = 398,
    CSSValueMediaPlayButton = 399,
    CSSValueMediaOverlayPlayButton = 400,
    CSSValueMediaToggleClosedCaptionsButton = 401,
    CSSValueMediaSlider = 402,
    CSSValueMediaSliderthumb = 403,
    CSSValueMediaVolumeSliderContainer = 404,
    CSSValueMediaVolumeSlider = 405,
    CSSValueMediaVolumeSliderthumb = 406,
    CSSValueMediaControlsBackground = 407,
    CSSValueMediaControlsFullscreenBackground = 408,
    CSSValueMediaCurrentTimeDisplay = 409,
    CSSValueMediaTimeRemainingDisplay = 410,
    CSSValueInternalMediaCastOffButton = 411,
    CSSValueInternalMediaOverlayCastOffButton = 412,
    CSSValueMenulist = 413,
    CSSValueMenulistButton = 414,
    CSSValueMenulistText = 415,
    CSSValueMenulistTextfield = 416,
    CSSValueMeter = 417,
    CSSValueProgressBar = 418,
    CSSValueProgressBarValue = 419,
    CSSValueSliderHorizontal = 420,
    CSSValueSliderVertical = 421,
    CSSValueSliderthumbHorizontal = 422,
    CSSValueSliderthumbVertical = 423,
    CSSValueCaret = 424,
    CSSValueSearchfield = 425,
    CSSValueSearchfieldDecoration = 426,
    CSSValueSearchfieldResultsDecoration = 427,
    CSSValueSearchfieldCancelButton = 428,
    CSSValueTextfield = 429,
    CSSValueRelevancyLevelIndicator = 430,
    CSSValueContinuousCapacityLevelIndicator = 431,
    CSSValueDiscreteCapacityLevelIndicator = 432,
    CSSValueRatingLevelIndicator = 433,
    CSSValueTextarea = 434,
    CSSValueCapsLockIndicator = 435,
    CSSValueRound = 436,
    CSSValueBorder = 437,
    CSSValueBorderBox = 438,
    CSSValueContent = 439,
    CSSValueContentBox = 440,
    CSSValuePadding = 441,
    CSSValuePaddingBox = 442,
    CSSValueMarginBox = 443,
    CSSValueContain = 444,
    CSSValueCover = 445,
    CSSValueLogical = 446,
    CSSValueVisual = 447,
    CSSValueAlternate = 448,
    CSSValueAlternateReverse = 449,
    CSSValueForwards = 450,
    CSSValueBackwards = 451,
    CSSValueInfinite = 452,
    CSSValueRunning = 453,
    CSSValuePaused = 454,
    CSSValueFlat = 455,
    CSSValuePreserve3d = 456,
    CSSValueEase = 457,
    CSSValueLinear = 458,
    CSSValueEaseIn = 459,
    CSSValueEaseOut = 460,
    CSSValueEaseInOut = 461,
    CSSValueStepStart = 462,
    CSSValueStepMiddle = 463,
    CSSValueStepEnd = 464,
    CSSValueSteps = 465,
    CSSValueCubicBezier = 466,
    CSSValueDocument = 467,
    CSSValueReset = 468,
    CSSValueZoom = 469,
    CSSValueVisiblePainted = 470,
    CSSValueVisibleFill = 471,
    CSSValueVisibleStroke = 472,
    CSSValuePainted = 473,
    CSSValueFill = 474,
    CSSValueStroke = 475,
    CSSValueBoundingBox = 476,
    CSSValueSpellOut = 477,
    CSSValueDigits = 478,
    CSSValueLiteralPunctuation = 479,
    CSSValueNoPunctuation = 480,
    CSSValueAntialiased = 481,
    CSSValueSubpixelAntialiased = 482,
    CSSValueOptimizeSpeed = 483,
    CSSValueOptimizeLegibility = 484,
    CSSValueGeometricPrecision = 485,
    CSSValueEconomy = 486,
    CSSValueExact = 487,
    CSSValueLr = 488,
    CSSValueRl = 489,
    CSSValueTb = 490,
    CSSValueLrTb = 491,
    CSSValueRlTb = 492,
    CSSValueTbRl = 493,
    CSSValueHorizontalTb = 494,
    CSSValueVerticalRl = 495,
    CSSValueVerticalLr = 496,
    CSSValueAfter = 497,
    CSSValueBefore = 498,
    CSSValueOver = 499,
    CSSValueUnder = 500,
    CSSValueFilled = 501,
    CSSValueOpen = 502,
    CSSValueDot = 503,
    CSSValueDoubleCircle = 504,
    CSSValueTriangle = 505,
    CSSValueSesame = 506,
    CSSValueEllipse = 507,
    CSSValueClosestSide = 508,
    CSSValueClosestCorner = 509,
    CSSValueFarthestSide = 510,
    CSSValueFarthestCorner = 511,
    CSSValueMixed = 512,
    CSSValueSideways = 513,
    CSSValueSidewaysRight = 514,
    CSSValueUpright = 515,
    CSSValueVerticalRight = 516,
    CSSValueOn = 517,
    CSSValueOff = 518,
    CSSValueOptimizeQuality = 519,
    CSSValuePixelated = 520,
    CSSValueWebkitOptimizeContrast = 521,
    CSSValueNonzero = 522,
    CSSValueEvenodd = 523,
    CSSValueAt = 524,
    CSSValueAlphabetic = 525,
    CSSValueFullscreen = 526,
    CSSValueStandalone = 527,
    CSSValueMinimalUi = 528,
    CSSValueBrowser = 529,
    CSSValueSticky = 530,
    CSSValueCoarse = 531,
    CSSValueFine = 532,
    CSSValueOnDemand = 533,
    CSSValueHover = 534,
    CSSValueMultiply = 535,
    CSSValueScreen = 536,
    CSSValueDarken = 537,
    CSSValueLighten = 538,
    CSSValueColorDodge = 539,
    CSSValueColorBurn = 540,
    CSSValueHardLight = 541,
    CSSValueSoftLight = 542,
    CSSValueDifference = 543,
    CSSValueExclusion = 544,
    CSSValueHue = 545,
    CSSValueSaturation = 546,
    CSSValueColor = 547,
    CSSValueLuminosity = 548,
    CSSValueScaleDown = 549,
    CSSValueBalance = 550,
    CSSValueWebkitPagedX = 551,
    CSSValueWebkitPagedY = 552,
    CSSValueDrag = 553,
    CSSValueNoDrag = 554,
    CSSValueSpan = 555,
    CSSValueMinmax = 556,
    CSSValueEachLine = 557,
    CSSValueProgressive = 558,
    CSSValueInterlace = 559,
    CSSValueMarkers = 560,
    CSSValueInternalExtendToZoom = 561,
    CSSValuePanX = 562,
    CSSValuePanY = 563,
    CSSValuePanLeft = 564,
    CSSValuePanRight = 565,
    CSSValuePanUp = 566,
    CSSValuePanDown = 567,
    CSSValueManipulation = 568,
    CSSValueLastBaseline = 569,
    CSSValueSelfStart = 570,
    CSSValueSelfEnd = 571,
    CSSValueLegacy = 572,
    CSSValueSmooth = 573,
    CSSValueContents = 574,
    CSSValueScrollPosition = 575,
    CSSValueRevert = 576,
    CSSValueUnset = 577,
    CSSValueLinearGradient = 578,
    CSSValueRadialGradient = 579,
    CSSValueRepeatingLinearGradient = 580,
    CSSValueRepeatingRadialGradient = 581,
    CSSValueWebkitCrossFade = 582,
    CSSValueWebkitGradient = 583,
    CSSValueWebkitLinearGradient = 584,
    CSSValueWebkitRadialGradient = 585,
    CSSValueWebkitRepeatingLinearGradient = 586,
    CSSValueWebkitRepeatingRadialGradient = 587,
    CSSValueWebkitImageSet = 588,
    CSSValueFrom = 589,
    CSSValueTo = 590,
    CSSValueColorStop = 591,
    CSSValueRadial = 592,
    CSSValueAttr = 593,
    CSSValueCounter = 594,
    CSSValueCounters = 595,
    CSSValueRect = 596,
    CSSValuePolygon = 597,
    CSSValueFormat = 598,
    CSSValueInvert = 599,
    CSSValueGrayscale = 600,
    CSSValueSepia = 601,
    CSSValueSaturate = 602,
    CSSValueHueRotate = 603,
    CSSValueOpacity = 604,
    CSSValueBrightness = 605,
    CSSValueContrast = 606,
    CSSValueBlur = 607,
    CSSValueDropShadow = 608,
    CSSValueUrl = 609,
    CSSValueRgb = 610,
    CSSValueRgba = 611,
    CSSValueHsl = 612,
    CSSValueHsla = 613,
    CSSValueMatrix = 614,
    CSSValueMatrix3d = 615,
    CSSValuePerspective = 616,
    CSSValueRotate = 617,
    CSSValueRotateX = 618,
    CSSValueRotateY = 619,
    CSSValueRotateZ = 620,
    CSSValueRotate3d = 621,
    CSSValueScale = 622,
    CSSValueScaleX = 623,
    CSSValueScaleY = 624,
    CSSValueScaleZ = 625,
    CSSValueScale3d = 626,
    CSSValueSkew = 627,
    CSSValueSkewX = 628,
    CSSValueSkewY = 629,
    CSSValueTranslate = 630,
    CSSValueTranslateX = 631,
    CSSValueTranslateY = 632,
    CSSValueTranslateZ = 633,
    CSSValueTranslate3d = 634,
    CSSValuePath = 635,
    CSSValueCalc = 636,
    CSSValueWebkitCalc = 637,
    CSSValueMandatory = 638,
    CSSValueProximity = 639,
    CSSValueFromImage = 640,
    CSSValuePaint = 641,
    CSSValueStyle = 642,
    CSSValueLayout = 643,
    CSSValueVar = 644,
    CSSValueInternalVariableValue = 645,
    CSSValueAliceblue = 646,
    CSSValueAntiquewhite = 647,
    CSSValueAquamarine = 648,
    CSSValueAzure = 649,
    CSSValueBeige = 650,
    CSSValueBisque = 651,
    CSSValueBlanchedalmond = 652,
    CSSValueBlueviolet = 653,
    CSSValueBrown = 654,
    CSSValueBurlywood = 655,
    CSSValueCadetblue = 656,
    CSSValueChartreuse = 657,
    CSSValueChocolate = 658,
    CSSValueCoral = 659,
    CSSValueCornflowerblue = 660,
    CSSValueCornsilk = 661,
    CSSValueCrimson = 662,
    CSSValueCyan = 663,
    CSSValueDarkblue = 664,
    CSSValueDarkcyan = 665,
    CSSValueDarkgoldenrod = 666,
    CSSValueDarkgray = 667,
    CSSValueDarkgreen = 668,
    CSSValueDarkgrey = 669,
    CSSValueDarkkhaki = 670,
    CSSValueDarkmagenta = 671,
    CSSValueDarkolivegreen = 672,
    CSSValueDarkorange = 673,
    CSSValueDarkorchid = 674,
    CSSValueDarkred = 675,
    CSSValueDarksalmon = 676,
    CSSValueDarkseagreen = 677,
    CSSValueDarkslateblue = 678,
    CSSValueDarkslategray = 679,
    CSSValueDarkslategrey = 680,
    CSSValueDarkturquoise = 681,
    CSSValueDarkviolet = 682,
    CSSValueDeeppink = 683,
    CSSValueDeepskyblue = 684,
    CSSValueDimgray = 685,
    CSSValueDimgrey = 686,
    CSSValueDodgerblue = 687,
    CSSValueFirebrick = 688,
    CSSValueFloralwhite = 689,
    CSSValueForestgreen = 690,
    CSSValueGainsboro = 691,
    CSSValueGhostwhite = 692,
    CSSValueGold = 693,
    CSSValueGoldenrod = 694,
    CSSValueGreenyellow = 695,
    CSSValueHoneydew = 696,
    CSSValueHotpink = 697,
    CSSValueIndianred = 698,
    CSSValueIndigo = 699,
    CSSValueIvory = 700,
    CSSValueKhaki = 701,
    CSSValueLavender = 702,
    CSSValueLavenderblush = 703,
    CSSValueLawngreen = 704,
    CSSValueLemonchiffon = 705,
    CSSValueLightblue = 706,
    CSSValueLightcoral = 707,
    CSSValueLightcyan = 708,
    CSSValueLightgoldenrodyellow = 709,
    CSSValueLightgray = 710,
    CSSValueLightgreen = 711,
    CSSValueLightgrey = 712,
    CSSValueLightpink = 713,
    CSSValueLightsalmon = 714,
    CSSValueLightseagreen = 715,
    CSSValueLightskyblue = 716,
    CSSValueLightslategray = 717,
    CSSValueLightslategrey = 718,
    CSSValueLightsteelblue = 719,
    CSSValueLightyellow = 720,
    CSSValueLimegreen = 721,
    CSSValueLinen = 722,
    CSSValueMagenta = 723,
    CSSValueMediumaquamarine = 724,
    CSSValueMediumblue = 725,
    CSSValueMediumorchid = 726,
    CSSValueMediumpurple = 727,
    CSSValueMediumseagreen = 728,
    CSSValueMediumslateblue = 729,
    CSSValueMediumspringgreen = 730,
    CSSValueMediumturquoise = 731,
    CSSValueMediumvioletred = 732,
    CSSValueMidnightblue = 733,
    CSSValueMintcream = 734,
    CSSValueMistyrose = 735,
    CSSValueMoccasin = 736,
    CSSValueNavajowhite = 737,
    CSSValueOldlace = 738,
    CSSValueOlivedrab = 739,
    CSSValueOrangered = 740,
    CSSValueOrchid = 741,
    CSSValuePalegoldenrod = 742,
    CSSValuePalegreen = 743,
    CSSValuePaleturquoise = 744,
    CSSValuePalevioletred = 745,
    CSSValuePapayawhip = 746,
    CSSValuePeachpuff = 747,
    CSSValuePeru = 748,
    CSSValuePink = 749,
    CSSValuePlum = 750,
    CSSValuePowderblue = 751,
    CSSValueRebeccapurple = 752,
    CSSValueRosybrown = 753,
    CSSValueRoyalblue = 754,
    CSSValueSaddlebrown = 755,
    CSSValueSalmon = 756,
    CSSValueSandybrown = 757,
    CSSValueSeagreen = 758,
    CSSValueSeashell = 759,
    CSSValueSienna = 760,
    CSSValueSkyblue = 761,
    CSSValueSlateblue = 762,
    CSSValueSlategray = 763,
    CSSValueSlategrey = 764,
    CSSValueSnow = 765,
    CSSValueSpringgreen = 766,
    CSSValueSteelblue = 767,
    CSSValueTan = 768,
    CSSValueThistle = 769,
    CSSValueTomato = 770,
    CSSValueTurquoise = 771,
    CSSValueViolet = 772,
    CSSValueWheat = 773,
    CSSValueWhitesmoke = 774,
    CSSValueYellowgreen = 775,
    CSSValueAlpha = 776,
    CSSValueLuminance = 777,
    CSSValueAccumulate = 778,
    CSSValueNew = 779,
    CSSValueSRGB = 780,
    CSSValueLinearRGB = 781,
    CSSValueCrispEdges = 782,
    CSSValueButt = 783,
    CSSValueMiter = 784,
    CSSValueBevel = 785,
    CSSValueBeforeEdge = 786,
    CSSValueAfterEdge = 787,
    CSSValueCentral = 788,
    CSSValueTextBeforeEdge = 789,
    CSSValueTextAfterEdge = 790,
    CSSValueIdeographic = 791,
    CSSValueHanging = 792,
    CSSValueMathematical = 793,
    CSSValueUseScript = 794,
    CSSValueNoChange = 795,
    CSSValueResetSize = 796,
    CSSValueDynamic = 797,
    CSSValueNonScalingStroke = 798,
};

const int numCSSValueKeywords = 799;
const size_t maxCSSValueKeywordLength = 42;

const char* getValueName(CSSValueID);
bool isValueAllowedInMode(unsigned short id, CSSParserMode mode);

} // namespace blink

#endif // CSSValueKeywords_h
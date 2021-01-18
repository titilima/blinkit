// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element_factory.cpp
// Description: HTMLDocument Class
//      Author: Ziming Li
//     Created: 2020-11-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "html_document.h"

#include <unordered_map>
#include "third_party/blink/renderer/core/html/forms/html_button_element.h"
#include "third_party/blink/renderer/core/html/forms/html_data_list_element.h"
#include "third_party/blink/renderer/core/html/forms/html_field_set_element.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/html_body_element.h"
#include "third_party/blink/renderer/core/html/html_br_element.h"
#include "third_party/blink/renderer/core/html/html_div_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/html/html_hr_element.h"
#include "third_party/blink/renderer/core/html/html_html_element.h"
#include "third_party/blink/renderer/core/html/html_image_element.h"
#include "third_party/blink/renderer/core/html/html_li_element.h"
#include "third_party/blink/renderer/core/html/html_link_element.h"
#include "third_party/blink/renderer/core/html/html_meta_element.h"
#include "third_party/blink/renderer/core/html/html_olist_element.h"
#include "third_party/blink/renderer/core/html/html_paragraph_element.h"
#include "third_party/blink/renderer/core/html/html_slot_element.h"
#include "third_party/blink/renderer/core/html/html_span_element.h"
#include "third_party/blink/renderer/core/html/html_style_element.h"
#include "third_party/blink/renderer/core/html/html_table_cell_element.h"
#include "third_party/blink/renderer/core/html/html_table_element.h"
#include "third_party/blink/renderer/core/html/html_table_row_element.h"
#include "third_party/blink/renderer/core/html/html_table_section_element.h"
#include "third_party/blink/renderer/core/html/html_title_element.h"
#include "third_party/blink/renderer/core/html/html_ulist_element.h"

#define DEFINE_CONSTRUCTOR(Name)    \
    static HTMLElement* Name ## Constructor(Document &document, const CreateElementFlags) { \
        return HTML ## Name ## Element::Create(document);                                   \
    }
#define DEFINE_CONSTRUCTOR_WITH_FLAGS(Name) \
    static HTMLElement* Name ## Constructor(Document &document, const CreateElementFlags flags) {   \
        return HTML ## Name ## Element::Create(document, flags);                                    \
    }
#define DEFINE_CONSTRUCTOR_WITH_TAG(Name, ClassName, Tag)   \
    static HTMLElement* Name ## Constructor(Document &document, const CreateElementFlags)  {    \
        return ClassName::Create(html_names:: ## Tag, document);                                \
    }

#define CONSTRUCTOR_ENTRY(Tag, Name)    \
    { Tag, Name ## Constructor }

namespace blink {

using HTMLElementCreators = std::unordered_map<AtomicString, HTMLElement::Creator>;

DEFINE_CONSTRUCTOR(Button)
DEFINE_CONSTRUCTOR(DataList)
DEFINE_CONSTRUCTOR(FieldSet)
DEFINE_CONSTRUCTOR(Slot)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Body)
DEFINE_CONSTRUCTOR_WITH_FLAGS(BR)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Div)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Head)
DEFINE_CONSTRUCTOR_WITH_FLAGS(HR)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Html)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Image)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Input)
DEFINE_CONSTRUCTOR_WITH_FLAGS(LI)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Link)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Meta)
DEFINE_CONSTRUCTOR_WITH_FLAGS(OList)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Paragraph)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Span)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Style)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Table)
DEFINE_CONSTRUCTOR_WITH_FLAGS(TableRow)
DEFINE_CONSTRUCTOR_WITH_FLAGS(Title)
DEFINE_CONSTRUCTOR_WITH_FLAGS(UList)
DEFINE_CONSTRUCTOR_WITH_TAG(Tbody, HTMLTableSectionElement, kTbodyTag)
DEFINE_CONSTRUCTOR_WITH_TAG(Td, HTMLTableCellElement, kTdTag)

static void FillElementCreators(HTMLElementCreators &dst)
{
    using namespace html_names;

    ASSERT(dst.empty());
    struct {
        const QualifiedName &tag;
        HTMLElement::Creator creator;
    } data[] = {
        CONSTRUCTOR_ENTRY(kBodyTag,     Body),
        CONSTRUCTOR_ENTRY(kBrTag,       BR),
        CONSTRUCTOR_ENTRY(kButtonTag,   Button),
        CONSTRUCTOR_ENTRY(kDatalistTag, DataList),
        CONSTRUCTOR_ENTRY(kDivTag,      Div),
        CONSTRUCTOR_ENTRY(kFieldsetTag, FieldSet),
        CONSTRUCTOR_ENTRY(kHeadTag,     Head),
        CONSTRUCTOR_ENTRY(kHrTag,       HR),
        CONSTRUCTOR_ENTRY(kHTMLTag,     Html),
        CONSTRUCTOR_ENTRY(kImgTag,      Image),
        CONSTRUCTOR_ENTRY(kInputTag,    Input),
        CONSTRUCTOR_ENTRY(kLiTag,       LI),
        CONSTRUCTOR_ENTRY(kLinkTag,     Link),
        CONSTRUCTOR_ENTRY(kMetaTag,     Meta),
        CONSTRUCTOR_ENTRY(kOlTag,       OList),
        CONSTRUCTOR_ENTRY(kPTag,        Paragraph),
        CONSTRUCTOR_ENTRY(kSlotTag,     Slot),
        CONSTRUCTOR_ENTRY(kSpanTag,     Span),
        CONSTRUCTOR_ENTRY(kStyleTag,    Style),
        CONSTRUCTOR_ENTRY(kTableTag,    Table),
        CONSTRUCTOR_ENTRY(kTbodyTag,    Tbody),
        CONSTRUCTOR_ENTRY(kTdTag,       Td),
        CONSTRUCTOR_ENTRY(kTrTag,       TableRow),
        CONSTRUCTOR_ENTRY(kTitleTag,    Title),
        CONSTRUCTOR_ENTRY(kUlTag,       UList)
    };
    for (const auto &e : data)
    {
        dst.insert({ e.tag.LocalName(), e.creator });
    }
}

Element* HTMLDocument::CreateElement(const AtomicString &localName, CreateElementFlags flags)
{
    static std::unordered_map<AtomicString, HTMLElement::Creator> s_creators;
    if (s_creators.empty())
        FillElementCreators(s_creators);

    auto it = s_creators.find(localName);
    if (std::end(s_creators) == it)
    {
        BKLOG("Unknown tag name: %s", localName.StdUtf8().c_str());
        ASSERT(std::end(s_creators) != it);
        return nullptr;
    }

    return it->second(*this, flags);
}

} // namespace blink

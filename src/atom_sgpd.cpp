/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is MPEG4IP.
 *
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie     dmackie@cisco.com
 *
 * sgpd - Sample Group Description Box (ISO/IEC 14496-12 8.9.3).
 * Ported from the SublerApp/mp4v2 fork. Difference: only version 1 is parsed
 * into typed properties (that is what Generate() writes, e.g. the AAC 'roll'
 * group). Any other version keeps its payload as raw bytes, so that rewriting
 * a file (MP4Modify) never alters a sample group it does not understand.
 */

#include "src/impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

/*
 * SizeTableProperty is a special version of the MP4TableProperty -
 * the BytesProperty will need to set the value before it can read
 * from the file
 */
class SgpdSizeTableProperty : public MP4TableProperty
{
public:
    SgpdSizeTableProperty(MP4Atom& parentAtom, const char *name, MP4IntegerProperty *pCountProperty) :
            MP4TableProperty(parentAtom, name, pCountProperty) {};
protected:
    void ReadEntry(MP4File& file, uint32_t index) {
        // Each entry has a length, followed by the description bytes
        // first, read the length
        m_pProperties[0]->Read(file, index);
        MP4IntegerProperty *pIntProp = (MP4IntegerProperty *)m_pProperties[0];
        // set the size in the bytes property
        MP4BytesProperty *pBytesProp = (MP4BytesProperty *)m_pProperties[1];
        pBytesProp->SetValueSize(pIntProp->GetValue(index), index);
        // And read the bytes
        m_pProperties[1]->Read(file, index);
    };
private:
    SgpdSizeTableProperty();
    SgpdSizeTableProperty ( const SgpdSizeTableProperty &src );
    SgpdSizeTableProperty &operator= ( const SgpdSizeTableProperty &src );
};

MP4SgpdAtom::MP4SgpdAtom(MP4File &file)
        : MP4Atom(file, "sgpd")
{
    AddVersionAndFlags();
    AddProperty( /* 2 */ new MP4Integer32Property(*this,"groupingType"));
}

// version 1 only; property 3 "defaultLength" must already exist
void MP4SgpdAtom::AddTableProperties(uint32_t defaultLength)
{
    MP4Integer32Property* pCount = new MP4Integer32Property(*this, "entryCount");
    AddProperty( /* 4 */ pCount);

    MP4TableProperty* pTable;
    if (defaultLength == 0) {
        pTable = new SgpdSizeTableProperty(*this, "entries", pCount);
    }
    else {
        pTable = new MP4TableProperty(*this, "entries", pCount);
    }
    AddProperty( /* 5 */ pTable);

    // The entry property is called "groupingType" as in the Subler fork;
    // it holds the sample group description entry (for 'roll': roll_distance).
    if (defaultLength == 0) {
        pTable->AddProperty(new MP4Integer32Property(pTable->GetParentAtom(), "descriptionLength"));
        pTable->AddProperty(new MP4BytesProperty(pTable->GetParentAtom(), "groupingType"));
    }
    else if (defaultLength == 1) {
        pTable->AddProperty(new MP4Integer8Property(pTable->GetParentAtom(), "groupingType"));
    }
    else if (defaultLength == 2) {
        pTable->AddProperty(new MP4Integer16Property(pTable->GetParentAtom(), "groupingType"));
    }
    else {
        pTable->AddProperty(new MP4BytesProperty(pTable->GetParentAtom(), "groupingType", defaultLength));
    }
}

void MP4SgpdAtom::Generate()
{
    // version 1, default_length 2 : the layout of an AAC 'roll' group
    const uint32_t defaultLength = 2;
    SetVersion(1);
    MP4Integer32Property* pDefaultLength = new MP4Integer32Property(*this,"defaultLength");
    AddProperty( /* 3 */ pDefaultLength);
    pDefaultLength->SetValue(defaultLength);
    AddTableProperties(defaultLength);

    MP4Atom::Generate();
}

void MP4SgpdAtom::Read()
{
    /* read atom version, flags and grouping type */
    ReadProperties(0, 3);

    if (GetVersion() == 1) {
        /* read default length, then build the table for it */
        MP4Integer32Property* pDefaultLength = new MP4Integer32Property(*this,"defaultLength");
        AddProperty( /* 3 */ pDefaultLength);
        ReadProperties(3, 1);

        AddTableProperties(pDefaultLength->GetValue());

        /* entry count and entries */
        ReadProperties(4);
    }
    else {
        /* other versions: keep the payload as is, so a rewrite is lossless */
        uint64_t remaining = GetEnd() - m_File.GetPosition();
        if (remaining > 0) {
            AddProperty(new MP4BytesProperty(*this, "data", (uint32_t)remaining));
            ReadProperties(3);
        }
    }

    Skip(); // to end of atom
}

///////////////////////////////////////////////////////////////////////////////

}
} // namespace mp4v2::impl

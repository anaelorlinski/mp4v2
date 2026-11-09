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
 * sbgp - Sample To Group Box (ISO/IEC 14496-12 8.9.2).
 * Ported from the SublerApp/mp4v2 fork.
 */

#include "src/impl.h"

namespace mp4v2 {
namespace impl {

///////////////////////////////////////////////////////////////////////////////

MP4SbgpAtom::MP4SbgpAtom(MP4File &file)
        : MP4Atom(file, "sbgp")
{
    AddVersionAndFlags();
    AddProperty( /* 2 */ new MP4Integer32Property(*this,"groupingType"));
}

void MP4SbgpAtom::AddProperties(uint8_t version)
{
    if (version == 1) {
        AddProperty( /* 3 */ new MP4Integer32Property(*this,"groupingTypeParameter"));
    }

    MP4Integer32Property* pCount = new MP4Integer32Property(*this, "entryCount");
    AddProperty(pCount);

    MP4TableProperty* pTable = new MP4TableProperty(*this, "entries", pCount);
    AddProperty(pTable);

    pTable->AddProperty(new MP4Integer32Property(pTable->GetParentAtom(), "sampleCount"));
    pTable->AddProperty(new MP4Integer32Property(pTable->GetParentAtom(), "groupDescriptionIndex"));
}

void MP4SbgpAtom::Generate()
{
    SetVersion(0);
    AddProperties(GetVersion());

    MP4Atom::Generate();
}

void MP4SbgpAtom::Read()
{
    /* read atom version, flags and grouping type */
    ReadProperties(0, 3);

    if (GetVersion() <= 1) {
        /* need to create the properties based on the atom version */
        AddProperties(GetVersion());

        /* now we can read the remaining properties */
        ReadProperties(3);
    }
    else {
        /* unknown version: keep the payload as is, so a rewrite is lossless */
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

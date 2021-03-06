//
//  atom.h
//  mp4v2
//
//  Created by Hale Chan on 16/7/18.
//  Copyright © 2016年 Tips4app. All rights reserved.
//

#ifndef MP4V2_atom_h
#define MP4V2_atom_h

typedef void* MP4AtomHandle;
typedef void* MP4PropertyHandle;

MP4V2_EXPORT
MP4AtomHandle MP4GetRootAtom(MP4FileHandle hFile);

MP4V2_EXPORT
const char * MP4GetTypeOfAtom(MP4AtomHandle atom);

MP4V2_EXPORT
uint64_t MP4GetSizeOfAtom(MP4AtomHandle atom);

MP4V2_EXPORT
void * MP4CopyRawBytesOfAtom(MP4AtomHandle atom, size_t *count);

MP4V2_EXPORT
uint32_t MP4GetNumberOfChildAtoms(MP4AtomHandle atom);

MP4V2_EXPORT
MP4AtomHandle MP4GetChildAtomAtIndex(MP4AtomHandle atom, unsigned index);

MP4V2_EXPORT
MP4AtomHandle MP4GetParentAtom(MP4AtomHandle atom);

MP4V2_EXPORT
uint32_t MP4GetNumberOfPropertiesOfAtom(MP4AtomHandle atom);

MP4V2_EXPORT
MP4PropertyHandle MP4GetPropertyOfAtomAtIndex(MP4AtomHandle atom, unsigned index);

MP4V2_EXPORT
const char * MP4GetNameOfProperty(MP4PropertyHandle property);

MP4V2_EXPORT
char * MP4CopyDescriptionOfProperty(MP4PropertyHandle property, size_t *count);

MP4V2_EXPORT
MP4AtomHandle MP4FindTrackAtom(
    MP4FileHandle hFile,
   uint16_t      index,
   const char*   type);

MP4V2_EXPORT
MP4AtomHandle MP4FindAtom(
    MP4FileHandle hFile,
   const char*   type);

#endif /* atom_h */

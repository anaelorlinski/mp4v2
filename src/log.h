///////////////////////////////////////////////////////////////////////////////
//
//  The contents of this file are subject to the Mozilla Public License
//  Version 1.1 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.mozilla.org/MPL/
//
//  Software distributed under the License is distributed on an "AS IS"
//  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//  License for the specific language governing rights and limitations
//  under the License.
// 
//  The Original Code is MP4v2.
// 
//  The Initial Developer of the Original Code is David Byron.
//  Portions created by David Byron are Copyright (C) 2009, 2010, 2011.
//  All Rights Reserved.
//
//  Contributors:
//      David Byron, dbyron@dbyron.com
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MP4V2_IMPL_LOG_H
#define MP4V2_IMPL_LOG_H

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

/**
 * Handle logging either to standard out or to a callback
 * function
 */
class MP4V2_EXPORT MP4Log {
private:
    MP4LogLevel                 _verbosity;
    MP4LogCallback              _cb_func;
    void*                       _cb_ptr;
public:
    const MP4LogLevel&          verbosity;

public:
    MP4Log( MP4LogLevel = MP4_LOG_NONE );
    virtual ~MP4Log();

    void setLogCallback ( MP4LogCallback );
    void setLogPtr ( void* );

    void setVerbosity   ( MP4LogLevel );

    void errorf ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void warningf ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void infof ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void verbose1f ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void verbose2f ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void verbose3f ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);
    void verbose4f ( const char* format, ... ) MP4V2_WFORMAT_PRINTF(2,3);

    void dump ( uint8_t       indent,
                MP4LogLevel   verbosity_,
                const char*   format, ... ) MP4V2_WFORMAT_PRINTF(4,5);
    void vdump ( uint8_t       indent,
                 MP4LogLevel   verbosity_,
                 const char*   format, va_list ap );
    void printf ( MP4LogLevel   verbosity_,
                  const char*   format, ... ) MP4V2_WFORMAT_PRINTF(3,4);
    void vprintf ( MP4LogLevel  verbosity_,
                   const char*  format, va_list ap );

    void hexDump ( uint8_t              indent,
                   MP4LogLevel          verbosity_,
                   const uint8_t*       pBytes,
                   uint32_t             numBytes,
                   const char*          format, ... ) MP4V2_WFORMAT_PRINTF(6,7);

    void errorf ( const Exception&      x );

private:
    MP4Log ( const MP4Log &src );
    MP4Log &operator= ( const MP4Log &src );
};

/**
 * A global (at least to mp4v2) log object for code that
 * needs to log something but doesn't otherwise have access
 * to one
 */
// AO : replaced by instance in MP4File class. Keep global one in case cannot be replaced easily.
extern MP4Log log;
///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

#endif // MP4V2_IMPL_LOG_H

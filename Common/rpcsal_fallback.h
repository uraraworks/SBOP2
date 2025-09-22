// Ensures RPC SAL macros exist even when older headers are pulled in.
// Prefer real headers if available; otherwise, provide no-op fallbacks.
#pragma once

// Do NOT include Windows/RPC headers here to avoid ordering issues.
// This header only provides no-op macro fallbacks when the real
// rpcsal/sal annotations are unavailable.

#if defined(__has_include)
#if __has_include(<rpcsal.h>)
#define RPCSAL_FALLBACK_HAS_NATIVE 1
#endif
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
// Guard against double definitions even when __has_include is
// unavailable but rpcsal.h has already been included by toolchain
// headers bundled with Visual Studio 2008 or later.
#if !defined(RPCSAL_FALLBACK_HAS_NATIVE) && defined(_INC_RPCSAL)
#define RPCSAL_FALLBACK_HAS_NATIVE 1
#endif
#endif

#if !defined(RPCSAL_FALLBACK_HAS_NATIVE)

#ifndef __RPC__in
#define __RPC__in
#endif
#ifndef __RPC__in_opt
#define __RPC__in_opt
#endif
#ifndef __RPC__out
#define __RPC__out
#endif
#ifndef __RPC__out_opt
#define __RPC__out_opt
#endif
#ifndef __RPC__inout
#define __RPC__inout
#endif
#ifndef __RPC__inout_opt
#define __RPC__inout_opt
#endif

#ifndef __RPC__deref_in
#define __RPC__deref_in
#endif
#ifndef __RPC__deref_out
#define __RPC__deref_out
#endif
#ifndef __RPC__deref_inout
#define __RPC__deref_inout
#endif
#ifndef __RPC__deref_inout_opt
#define __RPC__deref_inout_opt
#endif

#ifndef __RPC__in_range
#define __RPC__in_range(...) 
#endif

#ifndef __RPC__in_xcount
#define __RPC__in_xcount(...) 
#endif
#ifndef __RPC__in_xcount_full
#define __RPC__in_xcount_full(...) 
#endif
#ifndef __RPC__out_xcount
#define __RPC__out_xcount(...) 
#endif
#ifndef __RPC__out_xcount_part
#define __RPC__out_xcount_part(...) 
#endif
#ifndef __RPC__inout_xcount
#define __RPC__inout_xcount(...) 
#endif

#ifndef __RPC__in_ecount
#define __RPC__in_ecount(...) 
#endif
#ifndef __RPC__in_ecount_full
#define __RPC__in_ecount_full(...) 
#endif
#ifndef __RPC__out_ecount
#define __RPC__out_ecount(...) 
#endif
#ifndef __RPC__out_ecount_part
#define __RPC__out_ecount_part(...)
#endif

#endif // !defined(RPCSAL_FALLBACK_HAS_NATIVE)

#include "config.h"
#include <wtf/darwin/OSLogPrintStream.h>

#if OS(DARWIN)
#include <cstdio>
#include <cstring>

namespace WTF {

OSLogPrintStream::OSLogPrintStream(os_log_t log, os_log_type_t logType)
    : m_log(log), m_logType(logType) { }

OSLogPrintStream::~OSLogPrintStream() = default;

std::unique_ptr<OSLogPrintStream> OSLogPrintStream::open(const char* subsystem, const char* category, os_log_type_t logType)
{
    os_log_t log = os_log_create(subsystem, category);
    return std::unique_ptr<OSLogPrintStream>(new OSLogPrintStream(log, logType));
}

void OSLogPrintStream::vprintf(const char* format, va_list args)
{
    Locker locker { m_stringLock };
    char buf[256];
    int len = vsnprintf(buf, sizeof(buf), format, args);
    if (len > 0)
        os_log_with_type(m_log, m_logType, "%{public}s", buf);
}

} // namespace WTF
#endif

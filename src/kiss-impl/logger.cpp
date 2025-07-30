import Kiss.SBI;

#include <karm-base/lock.h>
#include <karm-logger/_embed.h>

namespace Karm::Logger::_Embed {

static Lock _lock;

void loggerLock() {
    _lock.acquire();
}

void loggerUnlock() {
    _lock.release();
}

struct LoggerOut : Io::TextEncoderBase<> {
    Res<usize> write(Bytes bytes) override {
        for (auto b : bytes)
            Kiss::SBI::consolePutchar(b);
        return Ok(bytes.len());
    }
};

Io::TextWriter& loggerOut() {
    static LoggerOut out;
    return out;
}

} // namespace Karm::Logger::_Embed

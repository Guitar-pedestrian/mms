#include "Logging.h"

#include <QDir>
#include <QFile>
#include <QTime>

#include "Assert.h"
#include "Directory.h"
#include "SimUtilities.h"
#include "Time.h"

namespace mms {

QTextStream* Logging::STDOUT = nullptr;
QTextStream* Logging::STREAM = nullptr;

void Logging::init(const QString& runId) {

    ASSERT_TR(STDOUT == nullptr);
    ASSERT_TR(STREAM == nullptr);

    STDOUT = new QTextStream(stdout);

    QString dirPath(Directory::get()->getRunDirectory() + runId + "/");
    QDir().mkpath(dirPath);
    QString filePath(dirPath + "log.txt");
    QFile* file = new QFile(filePath);
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    STREAM = new QTextStream(file);

    qInstallMessageHandler(handler);
}

void Logging::handler(
        QtMsgType type,
        const QMessageLogContext& context,
        const QString& msg) {

    ASSERT_FA(STDOUT == nullptr);
    ASSERT_FA(STREAM == nullptr);
    
    static const QMap<QtMsgType, QString> mapping {
        {QtDebugMsg,    "DEBUG"   },
        {QtInfoMsg,     "INFO"    },
        {QtWarningMsg,  "WARN"    },
        {QtCriticalMsg, "CRITICAL"},
        {QtFatalMsg,    "FATAL"   },
    };

    QString formatted = QString("[ %1 | %2 | %3 ] - %4").arg(
        SimUtilities::formatDuration(Time::get()->elapsedRealTime()),
        SimUtilities::formatDuration(Time::get()->elapsedSimTime()),
        mapping.value(type),
        msg
    );

    *STDOUT << formatted << endl;
    *STREAM << formatted << endl;
}

} // namespace mms

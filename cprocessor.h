#ifndef CPROCESSOR_H
#define CPROCESSOR_H
#include <QString>
#include <QPluginLoader>
#include <QObject>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include <QTextStream>
#include "cgraph.hpp"
#include "ialgorithm.h"

class CProcessor {
private:
       QString m_code;
       QString m_plugin;
       void Prepare(const QString& dir_name, const QStringList& filenames);
public:
       CProcessor() {}
       void compile(const QStringList& filenames);
       IAlgorithm* load_algorithm();
       int apply_algorithm(CGraph* graph, IAlgorithm* algorithm);
};

#endif // CPROCESSOR_H

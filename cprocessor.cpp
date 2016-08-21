#include "cprocessor.h"

void CProcessor::Prepare(const QString& dir_name, const QStringList& filenames) {
       QDir dir(dir_name);
       QStringList source_list, headers_list;
       QString pro_file_content = "TEMPLATE = lib\n" +
                     "CONFIG += plugin\n" +
                     "QT -= gui\n" +
                     "DESTDIR = plugin\n" +
                     "SOURCES = ";
       QRegExp source_exp("(\.cpp|\.hpp)&");
       if(!dir.exists())
              if(!dir.mkdir(dir_name)) throw new ExDirCreationFailed;
       for(QString filename : filenames) {
              QFileInfo info(filename);
              QFile::copy(filename, dir_name + "/" + info.fileName());
              if(source_exp.exactMatch(info.fileName())) source_list.append(info.fileName());
              else headers_list.append(info.fileName());
       }
       for(QString filename : source_list) pro_file_content += filename + " ";
       pro_file_content += "\n HEADERS = ";
       for(QString filename : headers_list) pro_file_content += filename + " ";
       pro_file_content += "\n TARGET = algo_plugin";
       QFile pro_file(dir_name + "/algo_plugin.pro");
       if(!pro_file.open(QIODevice::WriteOnly)) throw new ExOpenFileFailed;
       QTextStream stream(&pro_file);
       stream << pro_file_content;
       pro_file.close();
}

void CProcessor::compile(const QStringList& filenames) {     //TODO
       Prepare("plugin", filenames);
}

IAlgorithm* CProcessor::load_algorithm() {
       QPluginLoader loader(m_plugin);
       IAlgorithm* algorithm = qobject_cast<IAlgorithm*>(loader.instance());
       if(!algorithm) throw new ExCastingFailed;
       return algorithm;
}

int CProcessor::apply_algorithm(CGraph* graph, IAlgorithm* algorithm) {
       QTime start = QTime::currentTime();
       algorithm->apply_algorithm(graph);
       return start.elapsed();
}

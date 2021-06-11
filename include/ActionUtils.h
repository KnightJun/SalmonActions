#pragma once
#include <QPixmap>
#include <QString>
#include <salmonactions_global.h>
bool SALMONACTIONS_EXPORT SaveImage(const QPixmap &pixmap, QString title = nullptr);
bool SALMONACTIONS_EXPORT SaveImage(const QImage& image, QString title = nullptr);
QString SALMONACTIONS_EXPORT GenerateFilename(QString prefix = "", QString suffix1 = "", QString suffix2 = "");
bool SALMONACTIONS_EXPORT ClipSetImage(const QImage image, QPoint* pos = nullptr);
bool SALMONACTIONS_EXPORT ClipSetFile(QString fileName);
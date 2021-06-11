#include <qfiledialog.h>
#include <qobject.h>
#include <salmonactions_global.h>
#include <QDateTime>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>
#include <qstring.h>

bool SALMONACTIONS_EXPORT SaveImage(const QImage& image, QString title) {
	static QString filetype;
	QString saveName;
	saveName = QFileDialog::getSaveFileName(nullptr,
		title.isEmpty() ? QObject::tr("Save image as") : title, "",
		"JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)",
		&filetype);
	if (saveName.isEmpty()) return false;
	return image.save(saveName);
}

bool SALMONACTIONS_EXPORT SaveImage(const QPixmap& pixmap, QString title) {
	QImage  image = pixmap.toImage();
	return SaveImage(image, title);
}

QString SALMONACTIONS_EXPORT GenerateFilename(QString prefix, QString suffix1, QString suffix2)
{
	static int i = 0;
	QDateTime time = QDateTime::currentDateTime();
	return prefix + time.toString("yyyyMMdd_hhmmss_zzz_") + QString::number(i++) + suffix1 + suffix2;
}

bool SALMONACTIONS_EXPORT ClipSetImage(const QImage image, QPoint *pos)
{
	QMimeData* data = new QMimeData;
	data->setImageData(image);
	if (pos) {
		QByteArray byteData;
		QString str = QString("%1 %2").arg(pos->x()).arg(pos->y());
		data->setData("PastePoint", str.toUtf8());
	}
	QClipboard* clip = QApplication::clipboard();
	clip->setMimeData(data);
	return true;
}

bool SALMONACTIONS_EXPORT ClipSetFile(QString fileName)
{
	QList<QUrl> copyfile;
	QUrl url = QUrl::fromLocalFile(fileName);    //待复制的文件
	if (url.isValid()) {
		copyfile.push_back(url);
	}
	else {
		return false;
	}
	QMimeData* data = new QMimeData;
	data->setUrls(copyfile);
	QClipboard* clip = QApplication::clipboard();
	clip->setMimeData(data);
	return true;
}

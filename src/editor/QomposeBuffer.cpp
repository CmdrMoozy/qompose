#include "QomposeBuffer.h"

QomposeBuffer::QomposeBuffer(QWidget *p)
	: QomposeEditor(p)
{
}

QomposeBuffer::~QomposeBuffer()
{
}

void QomposeBuffer::open(const QomposeFileDescriptor &f)
{
	
}

QString QomposeBuffer::getTitle() const
{
	return "Untitled";
}

QString QomposeBuffer::getPath() const
{
	return "";
}

/*!
 * This function returns whether or not this buffer has ever been saved, or if it
 * is a brand-new document (possibly with unsaved changes).
 */
bool QomposeBuffer::hasBeenSaved() const
{
	return false;
}

/*!
 * This function returns whether or not this buffer has any unsaved changes.
 */
bool QomposeBuffer::isModified() const
{
	return false;
}

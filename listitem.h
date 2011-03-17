#ifndef LISTITEM_H
#define LISTITEM_H

#include <QListWidgetItem>

class ListItem : public QListWidgetItem
{
public:
	enum DataRole
	{
		ItemTitle = 32,
		ItemDescription = 33,
		ItemIcon = 34,
		ItemData = 35
	};
	ListItem(const QString &title, const QString &description, const QString &icon = "", const QVariant &data = QVariant());
};

#endif

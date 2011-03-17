#include "listitem.h"

ListItem::ListItem(const QString &title, const QString &description, const QString &icon, const QVariant &data)
: QListWidgetItem(0, UserType)
{
	setData(ItemTitle, title);
	setData(ItemDescription, description);
	setData(ItemIcon, icon);
	setData(ItemData, data);
}

#include "listdelegate.h"
#include "listitem.h"
#include <algorithm>
#include <QModelIndex>
#include <QPainter>
#include <QApplication>
#include <QTextDocument>
#include <QIcon>

ListDelegate::ListDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString title = index.data(ListItem::ItemTitle).toString();
	QString description = index.data(ListItem::ItemDescription).toString();
	QIcon icon = QIcon::fromTheme(index.data(ListItem::ItemIcon).toString());
// 	QVariant data = index.data(ListItem::ItemData);

	painter->save();
	QPalette p;
	painter->fillRect(option.rect, p.brush((index.row() % 2 ) ? QPalette::Base : QPalette::AlternateBase));
	QStyle *style = QApplication::style();
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);
	
		QTextDocument doc;
		painter->translate(option.rect.topLeft());
		if (option.state & QStyle::State_Selected)
		{
		  painter->setPen(option.palette.color(QPalette::Normal, QPalette::HighlightedText));
		  doc.setDefaultStyleSheet("* { color: "+option.palette.color(QPalette::HighlightedText).name()+"; }");
		}
		else
		{
		  painter->setPen(option.palette.color(QPalette::Normal, QPalette::Text));
		  doc.setDefaultStyleSheet("* { color: "+option.palette.color(QPalette::Text).name()+"; }");
		}
		
		if(!icon.isNull())
		{
		  painter->drawPixmap(5,5,64,64, icon.pixmap(QSize(64,64)));
		}
		
		QFont f = painter->font();
		f.setBold(true);
		painter->setFont(f);
		painter->drawText(74,20, title);
		f.setBold(false);
		
		doc.setUndoRedoEnabled(false);
		doc.setDocumentMargin(0);
		doc.setTextWidth(option.rect.width()-79);
		doc.setUseDesignMetrics(true);
		doc.setHtml("<p>"+description+"</p>");
		QRectF rect = QRectF(QPoint(0,0),doc.size());
		painter->translate(74,20+QFontMetrics::QFontMetrics(f).height());
		doc.drawContents(painter, rect);
		painter->translate(-74,-20-QFontMetrics::QFontMetrics(f).height());

	painter->restore();
}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QString description = index.data(ListItem::ItemDescription).toString();
  QFont f = QApplication::font();
  QTextDocument doc;
  doc.setUndoRedoEnabled(false);
  doc.setDocumentMargin(0);
  doc.setTextWidth(option.rect.width()-79);
  doc.setUseDesignMetrics(true);
  doc.setHtml("<p>"+description+"</p>");
  return QSize(option.rect.width()-10,std::max(69, int(25+QFontMetrics::QFontMetrics(f).height()+doc.size().height())));
}


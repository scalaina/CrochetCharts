/*************************************************\
| Copyright (c) 2011 Stitch Works Software        |
| Brian C. Milco <brian@stitchworkssoftware.com>  |
\*************************************************/
#ifndef INDICATOR_H
#define INDICATOR_H

#include <QGraphicsTextItem>

#include <QLineEdit>

class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QFocusEvent;
class QKeyEvent;

class Indicator : public QGraphicsTextItem
{
    Q_OBJECT
    friend class SaveFile;
public:
    enum {Type = UserType + 15 };

    Indicator(QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    ~Indicator();

    QRectF boundingRect() const;
    int type() const { return Type; }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    QString text() { return toHtml(); }
    void setText(QString t) { setHtml(t); }

    QColor bgColor() { return mBgColor; }
    void setBgColor(QColor c) { mBgColor = c; }

    QColor textColor() { return mTextColor; }
    void setTextColor(QColor c) { mTextColor = c; }

    QString style() { return mStyle; }
    void setStyle(QString style) { mStyle = style; update();}

    QPainterPath shape() const;

    bool highlight;

signals:
    void lostFocus(Indicator *item);
    void gotFocus(Indicator *item);

protected:
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:

    QColor mBgColor;
    QColor mTextColor;

    QString mStyle;

};
#endif //INDICATOR_H

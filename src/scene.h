/*************************************************\
| Copyright (c) 2010 Stitch Works Software        |
| Brian C. Milco <brian@stitchworkssoftware.com>  |
\*************************************************/
#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

#include "cell.h"

#include <QUndoStack>
#include <QRubberBand>

#include "indicator.h"

class QKeyEvent;

class Scene : public QGraphicsScene
{
    Q_OBJECT
    friend class SaveFile;
    friend class SaveThread;
    friend class RowEditDialog;
    friend class TextView;

public:

    enum EditMode {
        StitchEdit = 10, //place stitches on the chart.
        ColorEdit = 11,       //place colors behind stitches.
        RowEdit = 12,
        RotationEdit = 14,       //adjust the angle of the
        ScaleEdit = 15,       //scale the stitches.
        IndicatorEdit = 16
    };

    enum ChartStyle {
        Rows = 100, // flat chart rows calc'ed based on grid.
        Rounds,     // round chart rows clac'ed with trig.
        Blank       // start w/no sts and allow anything.
    };
    
    Scene(QObject* parent = 0);
    ~Scene();

    //if you have the position in x, y use the overload function
    Cell* cell(int row, int column);
    //convert x,y to rows, columns.
    Cell* cell(QPoint position);

    /**
     * find the x,y positions on the grid for a given cell;
     * return QPoint(column, row);
     * if return = -1,-1 isVoid.
     */
    QPoint indexOf(Cell* c);
    
    int rowCount();
    int columnCount(int row);
    int maxColumnCount();

    void setEditMode(EditMode mode);
    EditMode editMode() { return mMode; }

    void setEditStitch(QString stitch) { mEditStitch = stitch; }

    void setEditFgColor(QColor color) { mEditFgColor = color; }
    void setEditBgColor(QColor color) { mEditBgColor = color; }

    QUndoStack* undoStack() { return &mUndoStack; }
    
    QStringList modes();

    void moveRowUp(int row);
    void moveRowDown(int row);

    void removeRow(int row);

    void alignSelection(int alignmentStyle);
    void distributeSelection(int distributionStyle);
    void arrangeGrid(QSize grid, QSize alignment, QSize spacing, bool useSelection);
    
    /**
     * directions: left = 1
     *             right = 2
     *             up = 3
     *             down = 4
     */
    void mirror(int direction);
    void rotate(qreal degrees);

    void copy();
    void cut();
    void paste();

    void group();
    void ungroup();

    void createRowsChart(int rows, int cols, QString defStitch, QSizeF rowSize);

    void addItem(QGraphicsItem* item);
    void removeItem(QGraphicsItem* item);
    
protected:
    void copyRecursively(QDataStream &stream, QList<QGraphicsItem*> items);
    void pasteRecursively(QDataStream &stream, QList<QGraphicsItem*> *group);

    /**
     * This function removes a cell from the 'grid'. if the row is empty it removes the row too.
     */
    void removeFromRows(Cell* c);
    
public slots:
    void createRow();
    void updateRow(int row);
    
    /**
     * highlight (select) all the stitches in row @param row.
     */
    void highlightRow(int row);
    void drawRowLines(int row);

    void highlightIndicators(bool state);

    void updateRubberBand(int dx, int dy);

    void editorLostFocus(Indicator *item);
    void editorGotFocus(Indicator *item);
    
signals:
    void stitchChanged(QString oldSt, QString newSt);
    void colorChanged(QString oldColor, QString newColor);

    void rowSelected();
    
protected:
/*
    virtual void    contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent )
    virtual void    helpEvent ( QGraphicsSceneHelpEvent * helpEvent )
*/
    void keyReleaseEvent(QKeyEvent* keyEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    QList<Indicator*> indicators() { return mIndicators; }
        
    void initDemoBackground();

    /**
     * vertial:    0 - don't change alignment
     *             1 - align top
     *             2 - align center v
     *             3 - align bottom
     * horizontal: 0 - don't change alignment
     *             1 - align left
     *             2 - align center h
     *             3 - align right
     */
    void align(int vertical, int horizontal);
    /**
     * vertial:    0 - don't change distribution
     *             1 - distribute top
     *             2 - distribute center v
     *             3 - distribute bottom
     * horizontal: 0 - don't change distribution
     *             1 - distribute left
     *             2 - distribute center h
     *             3 - distribute right
     */
    void distribute(int vertical, int horizontal);
    
    void alignToPath();
    void distributeToPath();

    /**
     * Because of the fact that the group doesn't return the correct scene co-ordinates
     * this function will correct them.
     */
    QPointF calcGroupPos(QGraphicsItem* group, QPointF newScenePos);

public:
    QGraphicsItemGroup* group(QList<QGraphicsItem*> items, QGraphicsItemGroup* g = 0);
    void ungroup(QGraphicsItemGroup* group);

    QRectF selectedItemsBoundingRect(QList<QGraphicsItem*> items);

    void rotateSelection(qreal degrees, QList<QGraphicsItem*> items, QPointF pivotPoint);

    /**
     * This function sets the sceneRect to the outside edges of all objects on the scene.
     */
    void setSceneRectToItems();
    
protected:
    void colorModeMouseMove(QGraphicsSceneMouseEvent* e);
    void colorModeMouseRelease(QGraphicsSceneMouseEvent* e);
    
    void indicatorModeMouseMove(QGraphicsSceneMouseEvent* e);
    void indicatorModeMouseRelease(QGraphicsSceneMouseEvent* e);

    void angleModeMousePress(QGraphicsSceneMouseEvent* e);
    void angleModeMouseMove(QGraphicsSceneMouseEvent* e);
    void angleModeMouseRelease(QGraphicsSceneMouseEvent* e);

    void scaleModeMousePress(QGraphicsSceneMouseEvent* e);
    void scaleModeMouseMove(QGraphicsSceneMouseEvent* e);
    void scaleModeMouseRelease(QGraphicsSceneMouseEvent* e);

    void rowEditMousePress(QGraphicsSceneMouseEvent* e);
    void rowEditMouseMove(QGraphicsSceneMouseEvent* e);
    void rowEditMouseRelease(QGraphicsSceneMouseEvent* e);
    
    void stitchModeMouseMove(QGraphicsSceneMouseEvent* e);
    void stitchModeMousePress(QGraphicsSceneMouseEvent* e);
    void stitchModeMouseRelease(QGraphicsSceneMouseEvent* e);

    QSizeF defaultSize() const { return mDefaultSize; }

    void updateStitchRenderer();

    void hideRowLines();

    
    
private:
    /**
     * Used in the mouse*Event()s to keep the mouse movements on the same cell.
     */
    QGraphicsItem* mCurItem;
    Cell* mCurCell;
    QPointF mCellStartPos;
    QPointF mLeftButtonDownPos;
    
    Indicator* mCurIndicator;
    
    /**
     * The difference between where the user clicked on the object and the (x,y) of the object.
     */
    QSizeF mDiff;
    qreal mOldAngle;

    QRubberBand* mRubberBand;
    QPointF mRubberBandStart;

    QMap<QGraphicsItem*, QPointF> mOldPositions;
    
    //Is the user moving an object.
    bool mMoving;
    bool mIsRubberband;
    bool mHasSelection;
    bool mSnapTo;
    
    EditMode mMode;
    
    QString mEditStitch;
    QColor mEditFgColor;
    QColor mEditBgColor;

    QPointF mOldScale;
    
    qreal mAngle;
    QPointF mPivotPt;
    QPointF mOrigin;

    //rows keeps track of the st order for individual rows;
    QList< QList<Cell*> > grid;
    
    qreal scenePosToAngle(QPointF pt);

    int mRowSpacing;
    QSizeF mDefaultSize;
    QString mDefaultStitch;
    

    QList<QGraphicsItem*> mRowSelection;
    
    QUndoStack mUndoStack;
    
    QList<Indicator*> mIndicators;

    Cell* mStartCell;
    Cell* mEndCell;
    Cell* mPreviousCell;
    QGraphicsLineItem* mRowLine;
    QList<QGraphicsLineItem*> mRowLines;
    
    QList<QGraphicsItem*> mDemoItems;

    QList<QGraphicsItemGroup*> mGroups;


/***
 *
 * Rounds specific functions:
 */
public:
    bool showChartCenter() { return mShowChartCenter; }

    void createRoundsChart(int rows, int cols, QString stitch, QSizeF rowSize);
    void createRow(int row, int columns, QString stitch);

    bool hasChartCenter() { return (mCenterSymbol ? true : false); }
    QGraphicsItem* chartCenter() { return mCenterSymbol; }
    
public slots:
    void setShowChartCenter(bool state);

protected:
    void setCellPosition(int row, int column, Cell* c, int columns);

private:
    QPointF calcPoint(double radius, double angleInDegrees, QPointF origin);

    QGraphicsItem* mCenterSymbol;
    bool mShowChartCenter;
};

#endif //SCENE_H

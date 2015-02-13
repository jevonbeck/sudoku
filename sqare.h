#ifndef SQARE_H
#define SQARE_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QFrame>
#include <QImage>
#include <QIcon>
#include <QColor>
#include <QPainter>
#include <QBrush>
#include <QPainterPath>
#include <QRect>
#include <QPoint>
#include <QSize>
#include <QVariant>
#include <QMouseEvent>
#include <QDragLeaveEvent>
#include <QAction>
#include <QMenu>
#include <QApplication>

class square : public QLabel
{
	Q_OBJECT

public:
	square(QWidget *parent = 0);		
	void setSquareColour(QColor new_colour);
	void setErrorColour(QColor new_colour);
	void setSolveColour(QColor new_colour);
	void setFontColour(QColor new_colour);
	void setDisableColour(QColor new_colour);
	void setNormalColour(QColor new_colour);
        QColor SolveColour(){return solveColour;}
        QColor NormalColour(){return normalColour;}
        QColor ErrorColour(){return errorColour;}
        QColor SquareColour(){return squareColour;}
	void setValue(int new_value);
	void setHint(QString new_hint);
	void disableSquare(bool disable);
        int Value(){return value;}
        QString Hint(){return hint;}
	void DisplayValue();
        bool editorEnabled(){return editorEnable;}
	

protected:  
	void mouseReleaseEvent(QMouseEvent *event);
	void changeEvent(QEvent *event);

private slots:	
	void changeValue();
	void clearSquare();
	void enableEditor();	
	
private:	
	void createMenu();	
	
	int value;
	QString hint;
	QColor squareColour;
	QColor normalColour,errorColour,solveColour,fontColour,disableColour;
	QMenu *menu;
	bool editorEnable;
};

#endif // SQARE_H

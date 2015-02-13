#include "sqare.h"

square::square(QWidget *parent)
	: QLabel(parent)
{	
    setAutoFillBackground(true);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setAlignment(Qt::AlignCenter);

    QSize block_size=QSize(40,40);
    resize(block_size);
    setMaximumSize(block_size);
    createMenu();

    setNormalColour(QColor(Qt::white));
    setErrorColour(QColor(Qt::red));
    setSolveColour(QColor(Qt::green));
    setFontColour(QColor(Qt::black));
    setDisableColour(QColor(Qt::lightGray));

    setSquareColour(normalColour);
    setValue(0);
    DisplayValue();
    editorEnable=false;
}

// This function displays the value of a square along with its appropriate background colour.

void square::DisplayValue(){
    setText(QVariant(value).toString());
    QImage image=QImage(width(),height(),QImage::Format_ARGB32_Premultiplied);
    if(value!=0){
        if(editorEnable){
            image.fill(disableColour.rgb());
            setSquareColour(normalColour);
        }
        else{
            image.fill(squareColour.rgb());
        }

        QPainter painter;
        painter.begin(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        QBrush brush=QBrush(fontColour);
        painter.setBrush(brush);
        painter.setPen(fontColour);
        QPainterPath path;
        QFont timesFont("Times", 24);
        path.addText(12, 30, timesFont, text());
        painter.drawPath(path);
        painter.end();
    }
    else{
        image.fill(squareColour.rgb());
    }
    setPixmap(QPixmap::fromImage(image));
}

// This function enables a square to display in Puzzle Editor Mode.

void square::enableEditor(){
	editorEnable=true;
}

// This function resets the square to its default state.

void square::clearSquare(){
	setValue(0);
	setSquareColour(NormalColour());
	DisplayValue();
	disableSquare(false);
}

// This function allows the value in a square to increment when the square is clicked.
// The new value is instantly displayed.

void square::mouseReleaseEvent(QMouseEvent *event){	
	if(event->button()==Qt::LeftButton){
		if(event->type()==QEvent::MouseButtonRelease){
			++value;
			if(value==10){
				value=0;
			}
		}		
	}
	else {
		if(event->button()==Qt::RightButton){			
			menu->exec(event->globalPos());
		}
	}
	DisplayValue();	
	QWidget::mouseReleaseEvent(event);
}

// This function creates the menu that allows the user to select a value for a square.

void square::createMenu(){
	menu=new QMenu(this);
	QAction *act;
	for(int x=1; x!=10; ++x){
		act=new QAction(QVariant(x).toString(),this);		
		connect(act,SIGNAL(triggered()),this,SLOT(changeValue()));
		menu->addAction(act);
	}
	act=new QAction(tr(" "),this);
	connect(act,SIGNAL(triggered()),this,SLOT(changeValue()));
	menu->addAction(act);
}

// This is a function that changes the value of a square.
// It is only executed when the user wants to place a value in a square.

void square::changeValue(){
	QObject *obj=sender();
	QAction *act=qobject_cast<QAction *>(obj);
	setValue(QVariant(act->text()).toInt());
	DisplayValue();
}

// This function changes the background colour of a square.

void square::setSquareColour(QColor new_colour){
	squareColour=new_colour;	
}

// This function changes the colour of a square when there is an error.

void square::setErrorColour(QColor new_colour){
	errorColour=new_colour;
}

// This function changes the background colour of a square when the solution is generated.

void square::setSolveColour(QColor new_colour){
	solveColour=new_colour;
}

// This function changes a square's font colour.

void square::setFontColour(QColor new_colour){
	fontColour=new_colour;
}

// This function changes the colour of when a square is disabled

void square::setDisableColour(QColor new_colour){
	disableColour=new_colour;
}

// This function changes the colour of an ordinary square

void square::setNormalColour(QColor new_colour){
	normalColour=new_colour;
}


// This function changes the value of a square.

void square::setValue(int new_value){
		value=new_value;		
	}

// This function sets the contents of the hint.

void square::setHint(QString new_hint){
	hint=new_hint;
}

// This function disables or enables a square.

void square::disableSquare(bool disable){
	setDisabled(disable);
}

// This function automtically changes the appearance of a square 
// according to whether the square is enabled or not.

void square::changeEvent(QEvent *event){
	if(event->type()== QEvent::EnabledChange){
		if(!isEnabled()){
			setSquareColour(disableColour);
		}
		else{
			setSquareColour(normalColour);
		}
		DisplayValue();		
	}
	QWidget::changeEvent(event);
}

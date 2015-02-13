#include "timer.h"
#include "grid.h"

 Timer::Timer(QWidget *parent)
     : QWidget(parent)
 {	   
     resetTime();
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
     timer->start(1000);
		      		 
		 left=new QLCDNumber;
		 right=new QLCDNumber;
		 left->setSegmentStyle(QLCDNumber::Filled);
		 left->setFrameShape(QFrame::NoFrame);
		 right->setSegmentStyle(QLCDNumber::Filled);
		 right->setFrameShape(QFrame::NoFrame);
		 QHBoxLayout *hbox=new QHBoxLayout;
		 hbox->addWidget(left);
		 hbox->addWidget(right);
		 hbox->setSpacing(0);
		 setLayout(hbox);
		 
		 showTime();		 
 }

 // This function resets the time and stops the timer.

 void Timer::resetTime(){
	 time=QTime(0,0,0);		 
	 day=0;
	 time_enable=false;
 }

 // This function starts the timer and resets the time according 
 // to which action requested that the timer be started

 void Timer::startTime(){
	 QObject *obj=sender();
	 QAction *act=qobject_cast<QAction *>(obj);
	 if(act !=0 && (act->text()=="&New Game" || act->text()=="&Load")){
		 resetTime();
	 }
	 else{
		 Grid *grid=qobject_cast<Grid *>(obj);
		 if(grid){
		   resetTime();
	   }
	 }
	 time_enable=true;
 }

 // This function displays the time in seconds, minutes, hours and days

 void Timer::showTime()
 {
	 if(time_enable){
	   time=time.addSecs(1);
	 }

	 QString text;
	 if(day==0){
		 text="";
	 }
	 else{		 
		 QVariant hold(day);
		 text=hold.toString()+" ";
	 }

	 text+=time.toString("hh mm ss");
	 
	 if(text.toStdString().substr(text.size()-8,8)=="23 59 59"){
			 ++day;
		 }	   
		 right->display(text);
		 time_string=text;
		 setWindowTitle(time_string);
		 text=text.fromStdString(text.toStdString().substr(0,text.size()-6));		 
		 left->display(text);		 
 } 

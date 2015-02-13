#ifndef TIMER_H
#define TIMER_H

#include <QLCDNumber>
#include <QTime>
#include <QTimer>
#include <QVariant>
#include <QHBoxLayout>
#include <QAction>
#include <string>

using std::string;

class Timer : public QWidget
{
  Q_OBJECT

public:
  Timer(QWidget *parent = 0);
        QTime Time(){return time;}
        int Day(){return day;}
        bool isTiming(){return time_enable;}
        QString TimeString(){return time_string;}

private:
	int day;
	QTime time;
	bool time_enable;
	QLCDNumber *left,*right;
	QString time_string;

private slots:
  void showTime();
	void startTime();
        void stopTime(){time_enable=false;}
	void resetTime();
};

#endif 

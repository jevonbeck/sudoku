#ifndef GRID_H
#define GRID_H

#include <QGridLayout>
#include <QToolTip>
#include <QVector>
#include <QList>
#include <QTime>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <QColorDialog>
#include <QMessageBox>
#include "sqare.h"


struct square_info {
  int value;
  int row;
  int col;
  int enabled;
};

/*struct solve_store{
	QList<int> nodes;
	QList<square_info> grid;
	int row;
	int col;
};*/

class Grid : public QWidget
{
	Q_OBJECT

public:
	Grid(QWidget *parent=0);
        void toEditor(){emit makeEditor();}
	QList<square_info> currentGrid();
	void populauteGrid(const QList<square_info>& square_values);
	
protected:
	bool event(QEvent *event);

signals:
	void resetSquare();
	void makeEditor();
	void puzzleSolved();
	void gridFull();
	void resetTimer();

private:
	square *itemAt(const QPoint &pos);
	bool row_has_num(int row_num, int number);
	bool col_has_num(int col_num, int number);
	bool blk_has_num(int blk_row, int blk_col, int number);
	QList<int> not_in_row(int row_num);
	QList<int> not_in_col(int col_num);
	QList<int> not_in_blk(int blk_row, int blk_col);
  QList<int> hintListAt(int row, int col);
	QString makeHint(const QList<int> &hints);
	void make_hintAt(int row, int col);	
	QList<square_info> makeFileList(QString directory,int fieldSize);
	void saveFile(QString directory, QString file_ext);
	void solve_attempt();
	bool full_grid();
	void puzzle_gen();
	bool isSolved();

	square *reference[9][9];
	bool hintEnable, errorEnable;
	QString time_saved;
			
private slots:
	void easy();
	void intermediate();
	void hard();
	void openGame();
	void saveGame(QString time);
	void openPuzzle();
	void savePuzzle();
	void validate();
	void solve();
	void enableHints();
	void enableErrors(bool);
	void changeColours();
	void highlight_errors();
	void error_check();
        void clearGrid(){emit resetSquare();}
	void puzzleSubmitted();
};

#endif // GRID_H
